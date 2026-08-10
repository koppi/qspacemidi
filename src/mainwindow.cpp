/*
 * mainwindow.cpp - main window of qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "mainwindow.h"

#include "aboutdialog.h"
#include "autostart.h"
#include "midiengine.h"
#include "settings.h"
#include "widgets/axisrow.h"

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStyle>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>

namespace qspacemidi {

namespace {

/*! CC counts sent per tick at full deflection and 1.0x sensitivity. */
constexpr double RelativeStepsPerTick = 6.0;

/*! How often a missing device is looked for again, in milliseconds. */
constexpr int DeviceRetryInterval = 3000;

AxisRow::ModeList axisModes()
{
    return {
        {ccModeName(CcMode::Absolute), static_cast<int>(CcMode::Absolute)},
        {ccModeName(CcMode::RelativeBinaryOffset),
         static_cast<int>(CcMode::RelativeBinaryOffset)},
        {ccModeName(CcMode::RelativeTwosComplement),
         static_cast<int>(CcMode::RelativeTwosComplement)},
        {ccModeName(CcMode::RelativeSignedBit),
         static_cast<int>(CcMode::RelativeSignedBit)},
    };
}

AxisRow::ModeList buttonModes()
{
    return {
        {buttonModeName(ButtonMode::Momentary), static_cast<int>(ButtonMode::Momentary)},
        {buttonModeName(ButtonMode::Toggle), static_cast<int>(ButtonMode::Toggle)},
    };
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    m_config = loadConfig();
    m_lastAbsolute.fill(-1);

    setWindowTitle(tr("QSpaceMIDI"));
    setWindowIcon(QIcon(QStringLiteral(":/icons/qspacemidi.png")));

    m_midi = new MidiEngine(this);
    connect(m_midi, &MidiEngine::error, this,
            [this](const QString &message) { setStatusMessage(message, true); });
    connect(m_midi, &MidiEngine::connectionChanged, this, &MainWindow::updateStatus);

    m_device = new SpaceNavigator(this);
    connect(m_device, &SpaceNavigator::axesNormChanged, this, &MainWindow::onAxesNormChanged);
    connect(m_device, &SpaceNavigator::buttonChanged, this, &MainWindow::onButtonChanged);
    connect(m_device, &SpaceNavigator::deviceOpened, this, &MainWindow::onDeviceOpened);
    connect(m_device, &SpaceNavigator::deviceClosed, this, &MainWindow::onDeviceClosed);

    buildUi();
    buildTrayIcon();
    applyConfigToUi();

    refreshPortList();
    openConfiguredPort();

    m_tick = new QTimer(this);
    m_tick->setTimerType(Qt::PreciseTimer);
    m_tick->setInterval(1000 / std::clamp(m_config.updateRate, 10, 200));
    connect(m_tick, &QTimer::timeout, this, &MainWindow::onTick);
    m_tick->start();

    m_deviceRetry = new QTimer(this);
    m_deviceRetry->setInterval(DeviceRetryInterval);
    connect(m_deviceRetry, &QTimer::timeout, this, &MainWindow::connectDevice);

    connectDevice();
    updateStatus();

    const QByteArray geometry = loadWindowGeometry();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

MainWindow::~MainWindow()
{
    m_quitting = true;
    saveConfig(m_config);

    // The child objects are torn down after this destructor body, so stop
    // everything that could still call back into the window first.
    m_tick->stop();
    m_deviceRetry->stop();
    m_device->disconnect(this);
    m_midi->disconnect(this);
    m_device->close();
    m_midi->closePort();
}

void MainWindow::buildUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(10);

    const auto makeGroup = [this, layout](const QString &title) {
        auto *box = new QGroupBox(title, this);
        auto *boxLayout = new QVBoxLayout(box);
        boxLayout->setContentsMargins(6, 8, 6, 8);
        boxLayout->setSpacing(2);
        layout->addWidget(box);
        return boxLayout;
    };

    // Translate: X Y Z.
    QVBoxLayout *translate = makeGroup(tr("Translate"));
    for (int i = AxisX; i <= AxisZ; ++i) {
        auto *row = new AxisRow(axisName(i), this);
        row->setModes(axisModes());
        row->setToolTip(axisDescription(i));
        translate->addWidget(row);
        m_axisRows[i] = row;
    }

    // Rotate: RX RY RZ, labelled X Y Z like the original.
    QVBoxLayout *rotate = makeGroup(tr("Rotate"));
    for (int i = AxisRX; i <= AxisRZ; ++i) {
        auto *row = new AxisRow(axisName(i), this);
        row->setModes(axisModes());
        row->setToolTip(axisDescription(i));
        rotate->addWidget(row);
        m_axisRows[i] = row;
    }

    // Buttons: the two caps on the side of the device.
    QVBoxLayout *buttons = makeGroup(tr("Buttons"));
    for (int i = 0; i < NumButtons; ++i) {
        auto *row = new AxisRow(QString::number(i + 1), this);
        row->setModes(buttonModes());
        row->setSensitivityEnabled(false);
        row->setInvertEnabled(false);
        row->setToolTip(tr("Hardware button %1").arg(i + 1));
        row->setLevel(-1.0);
        buttons->addWidget(row);
        m_buttonRows[i] = row;
    }

    for (int i = 0; i < NumAxes; ++i) {
        AxisRow *row = m_axisRows[i];
        connect(row, &AxisRow::modeChanged, this, [this, i](int mode) {
            m_config.axes[i].mode = static_cast<CcMode>(mode);
            m_accumulator[i] = 0.0;
            m_lastAbsolute[i] = -1;
            saveConfig(m_config);
        });
        connect(row, &AxisRow::ccChanged, this, [this, i](int cc) {
            m_config.axes[i].cc = cc;
            m_lastAbsolute[i] = -1;
            saveConfig(m_config);
        });
        connect(row, &AxisRow::sensitivityChanged, this, [this, i](int sensitivity) {
            m_config.axes[i].sensitivity = sensitivity;
            saveConfig(m_config);
        });
        connect(row, &AxisRow::mutedChanged, this, [this, i](bool muted) {
            m_config.axes[i].enabled = !muted;
            m_accumulator[i] = 0.0;
            m_lastAbsolute[i] = -1;
            saveConfig(m_config);
        });
        connect(row, &AxisRow::invertedChanged, this, [this, i](bool inverted) {
            m_config.axes[i].invert = inverted;
            saveConfig(m_config);
        });
    }

    for (int i = 0; i < NumButtons; ++i) {
        AxisRow *row = m_buttonRows[i];
        connect(row, &AxisRow::modeChanged, this, [this, i](int mode) {
            m_config.buttons[i].mode = static_cast<ButtonMode>(mode);
            m_buttonLatch[i] = false;
            saveConfig(m_config);
        });
        connect(row, &AxisRow::ccChanged, this, [this, i](int cc) {
            m_config.buttons[i].cc = cc;
            saveConfig(m_config);
        });
        connect(row, &AxisRow::mutedChanged, this, [this, i](bool muted) {
            m_config.buttons[i].enabled = !muted;
            saveConfig(m_config);
        });
    }

    // MIDI output and global input settings.
    auto *output = new QGroupBox(tr("MIDI Output"), this);
    auto *grid = new QGridLayout(output);
    grid->setContentsMargins(10, 8, 10, 8);
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(6);

    m_portCombo = new QComboBox(output);
    m_portCombo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    m_portCombo->setMinimumWidth(220);
    connect(m_portCombo, &QComboBox::activated, this, [this](int) {
        const QVariant data = m_portCombo->currentData();
        m_config.useVirtualPort = data.toString().isEmpty();
        m_config.portName = data.toString();
        saveConfig(m_config);
        openConfiguredPort();
    });

    auto *rescan = new QPushButton(tr("Rescan"), output);
    rescan->setToolTip(tr("Look for MIDI ports and 3D mice again"));
    connect(rescan, &QPushButton::clicked, this, [this] {
        refreshPortList();
        openConfiguredPort();
        connectDevice();
    });

    m_channelSpin = new QSpinBox(output);
    m_channelSpin->setRange(1, 16);
    m_channelSpin->setFixedWidth(58);
    connect(m_channelSpin, &QSpinBox::valueChanged, this, [this](int channel) {
        m_config.channel = channel;
        saveConfig(m_config);
    });

    grid->addWidget(new QLabel(tr("Port:"), output), 0, 0);
    grid->addWidget(m_portCombo, 0, 1);
    grid->addWidget(rescan, 0, 2);
    grid->addWidget(new QLabel(tr("Channel:"), output), 1, 0);
    grid->addWidget(m_channelSpin, 1, 1, Qt::AlignLeft);
    grid->setColumnStretch(1, 1);

    layout->addWidget(output);

    // Everything that is not about the port itself.
    auto *options = new QGroupBox(tr("Options"), this);
    auto *optionsGrid = new QGridLayout(options);
    optionsGrid->setContentsMargins(10, 8, 10, 8);
    optionsGrid->setHorizontalSpacing(8);
    optionsGrid->setVerticalSpacing(6);

    m_deadZoneSpin = new QSpinBox(options);
    m_deadZoneSpin->setRange(0, 50);
    m_deadZoneSpin->setSuffix(QStringLiteral(" %"));
    m_deadZoneSpin->setFixedWidth(66);
    m_deadZoneSpin->setToolTip(tr("Deflection ignored around the centre position"));
    connect(m_deadZoneSpin, &QSpinBox::valueChanged, this, [this](int deadZone) {
        m_config.deadZone = deadZone;
        saveConfig(m_config);
    });

    m_axisLockCheck = new QCheckBox(tr("Axis lock"), options);
    m_axisLockCheck->setToolTip(
        tr("Send only the axis that is deflected the most, so that parameters "
           "do not bleed into each other while MIDI mapping"));
    connect(m_axisLockCheck, &QCheckBox::toggled, this, [this](bool checked) {
        m_config.axisLock = checked;
        m_lockedAxis = -1;
        saveConfig(m_config);
    });

    m_autostartCheck = new QCheckBox(tr("Start at login"), options);
    m_autostartCheck->setToolTip(tr("Start QSpaceMIDI minimised to the system tray "
                                    "when the desktop session begins (%1)")
                                    .arg(autostart::location()));
    m_autostartCheck->setEnabled(autostart::isSupported());
    connect(m_autostartCheck, &QCheckBox::toggled, this, &MainWindow::setAutostartEnabled);

    auto *about = new QPushButton(tr("About"), options);
    connect(about, &QPushButton::clicked, this, &MainWindow::showAbout);

    optionsGrid->addWidget(new QLabel(tr("Dead zone:"), options), 0, 0);
    optionsGrid->addWidget(m_deadZoneSpin, 0, 1);
    optionsGrid->addWidget(m_axisLockCheck, 0, 2);
    optionsGrid->addWidget(m_autostartCheck, 0, 3);
    optionsGrid->addWidget(about, 0, 4);
    optionsGrid->setColumnStretch(3, 1);

    layout->addWidget(options);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName(QStringLiteral("statusLabel"));
    m_statusLabel->setWordWrap(true);
    layout->addWidget(m_statusLabel);
}

void MainWindow::buildTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }

    m_trayMenu = new QMenu(this);
    m_toggleWindowAction = m_trayMenu->addAction(tr("Hide Window"));
    connect(m_toggleWindowAction, &QAction::triggered, this, &MainWindow::toggleWindow);

    m_trayMenu->addSeparator();
    QAction *aboutAction = m_trayMenu->addAction(tr("About QSpaceMIDI..."));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);

    m_trayMenu->addSeparator();
    QAction *quitAction = m_trayMenu->addAction(tr("Quit"));
    connect(quitAction, &QAction::triggered, this, &MainWindow::quitApplication);

    m_tray = new QSystemTrayIcon(QIcon(QStringLiteral(":/icons/qspacemidi.png")), this);
    m_tray->setContextMenu(m_trayMenu);
    m_tray->setToolTip(tr("QSpaceMIDI"));
    connect(m_tray, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason) {
                onTrayActivated(static_cast<int>(reason));
            });
    m_tray->show();
}

void MainWindow::applyConfigToUi()
{
    for (int i = 0; i < NumAxes; ++i) {
        AxisRow *row = m_axisRows[i];
        const AxisMapping &axis = m_config.axes[i];
        row->setMode(static_cast<int>(axis.mode));
        row->setCc(axis.cc);
        row->setSensitivity(axis.sensitivity);
        row->setMuted(!axis.enabled);
        row->setInverted(axis.invert);
    }

    for (int i = 0; i < NumButtons; ++i) {
        AxisRow *row = m_buttonRows[i];
        const ButtonMapping &button = m_config.buttons[i];
        row->setMode(static_cast<int>(button.mode));
        row->setCc(button.cc);
        row->setMuted(!button.enabled);
    }

    QSignalBlocker channelBlocker(m_channelSpin);
    m_channelSpin->setValue(m_config.channel);
    QSignalBlocker deadZoneBlocker(m_deadZoneSpin);
    m_deadZoneSpin->setValue(m_config.deadZone);
    QSignalBlocker lockBlocker(m_axisLockCheck);
    m_axisLockCheck->setChecked(m_config.axisLock);
    QSignalBlocker autostartBlocker(m_autostartCheck);
    m_autostartCheck->setChecked(autostart::isSupported() && autostart::isEnabled());
}

void MainWindow::refreshPortList()
{
    const QStringList ports = m_midi->refreshPorts();

    QSignalBlocker blocker(m_portCombo);
    m_portCombo->clear();
    if (MidiEngine::supportsVirtualPorts()) {
        m_portCombo->addItem(tr("%1 (virtual port)").arg(MidiEngine::virtualPortName()),
                             QString());
    }
    for (const QString &port : ports) {
        m_portCombo->addItem(port, port);
    }

    int index = -1;
    if (!m_config.useVirtualPort && !m_config.portName.isEmpty()) {
        index = m_portCombo->findData(m_config.portName);
    }
    if (index < 0 && MidiEngine::supportsVirtualPorts()) {
        index = m_portCombo->findData(QString());
    }
    if (index < 0 && m_portCombo->count() > 0) {
        index = 0;
    }
    if (index >= 0) {
        m_portCombo->setCurrentIndex(index);
    }
}

void MainWindow::openConfiguredPort()
{
    const bool virtualPort = m_config.useVirtualPort && MidiEngine::supportsVirtualPorts();
    if (virtualPort) {
        m_midi->openVirtualPort();
        return;
    }

    QString port = m_config.portName;
    if (port.isEmpty()) {
        port = m_portCombo->currentData().toString();
    }
    if (port.isEmpty()) {
        m_midi->closePort();
        setStatusMessage(tr("No MIDI output port available."), true);
        return;
    }
    m_midi->openPort(port);
}

void MainWindow::connectDevice()
{
    if (m_device->isOpen()) {
        m_deviceRetry->stop();
        return;
    }
    if (!m_device->open()) {
        if (!m_deviceRetry->isActive()) {
            m_deviceRetry->start();
        }
        updateStatus();
    }
}

void MainWindow::onDeviceOpened()
{
    m_deviceRetry->stop();
    m_deflection.fill(0.0);
    m_accumulator.fill(0.0);
    m_lastAbsolute.fill(-1);
    updateStatus();
}

void MainWindow::onDeviceClosed()
{
    m_deflection.fill(0.0);
    for (AxisRow *row : m_axisRows) {
        row->setLevel(0.0);
    }
    if (!m_quitting && !m_deviceRetry->isActive()) {
        m_deviceRetry->start();
    }
    updateStatus();
}

void MainWindow::onAxesNormChanged(const SpaceNavigator::AxesNorm &axes)
{
    m_deflection[AxisX] = axes.x;
    m_deflection[AxisY] = axes.y;
    m_deflection[AxisZ] = axes.z;
    m_deflection[AxisRX] = axes.rx;
    m_deflection[AxisRY] = axes.ry;
    m_deflection[AxisRZ] = axes.rz;
}

void MainWindow::onButtonChanged(int button, bool pressed)
{
    if (button < 0 || button >= NumButtons) {
        return;
    }

    const ButtonMapping &mapping = m_config.buttons[button];
    bool state = pressed;

    if (mapping.mode == ButtonMode::Toggle) {
        if (!pressed) {
            return; // only act on the press edge
        }
        m_buttonLatch[button] = !m_buttonLatch[button];
        state = m_buttonLatch[button];
    }

    m_buttonRows[button]->setLevel(state ? 1.0 : -1.0);

    if (mapping.enabled) {
        m_midi->sendControlChange(m_config.channel, mapping.cc, state ? 127 : 0);
    }
}

void MainWindow::onTick()
{
    std::array<double, NumAxes> processed{};
    int dominant = -1;
    double dominantMagnitude = 0.0;

    for (int i = 0; i < NumAxes; ++i) {
        const AxisMapping &axis = m_config.axes[i];
        if (!axis.enabled) {
            processed[i] = 0.0;
            continue;
        }
        double value = applyDeadZone(m_deflection[i], m_config.deadZone);
        if (axis.invert) {
            value = -value;
        }
        processed[i] = value;

        const double magnitude = std::abs(value);
        if (magnitude > dominantMagnitude) {
            dominantMagnitude = magnitude;
            dominant = i;
        }
    }

    // Axis lock keeps transmitting the axis that is deflected the most, and
    // keeps it selected while everything is at rest so that the absolute
    // modes can still send their centre value on release.
    if (m_config.axisLock && dominant >= 0) {
        m_lockedAxis = dominant;
    }

    for (int i = 0; i < NumAxes; ++i) {
        const AxisMapping &axis = m_config.axes[i];
        const bool active = axis.enabled
                            && (!m_config.axisLock || m_lockedAxis < 0 || m_lockedAxis == i);

        const double value = active ? processed[i] : 0.0;
        m_axisRows[i]->setLevel(value);

        if (!active) {
            m_accumulator[i] = 0.0;
            continue;
        }

        const double scaled
            = std::clamp(value * sensitivityFactor(axis.sensitivity), -1.0, 1.0);

        if (axis.mode == CcMode::Absolute) {
            const int cc = absoluteCcValue(scaled);
            if (cc != m_lastAbsolute[i]) {
                m_lastAbsolute[i] = cc;
                m_midi->sendControlChange(m_config.channel, axis.cc, cc);
            }
        } else {
            m_accumulator[i] += scaled * RelativeStepsPerTick;
            const int steps = static_cast<int>(m_accumulator[i]);
            if (steps != 0) {
                m_accumulator[i] -= steps;
                m_midi->sendControlChange(m_config.channel, axis.cc,
                                          relativeCcValue(axis.mode, steps));
            }
        }
    }
}

void MainWindow::updateStatus()
{
    const QString device = m_device->isOpen()
                               ? tr("3D mouse: %1").arg(m_device->deviceName())
                               : tr("3D mouse: not found");
    const QString midi = m_midi->isOpen() ? tr("MIDI: %1").arg(m_midi->connectionName())
                                          : tr("MIDI: not connected");

    setStatusMessage(device + QStringLiteral("  -  ") + midi,
                     !m_device->isOpen() || !m_midi->isOpen());

    if (m_tray) {
        m_tray->setToolTip(QStringLiteral("QSpaceMIDI\n") + device + QLatin1Char('\n') + midi);
    }
}

void MainWindow::setStatusMessage(const QString &message, bool isError)
{
    if (!m_statusLabel) {
        return;
    }
    m_statusLabel->setText(message);
    m_statusLabel->setProperty("error", isError);
    m_statusLabel->style()->unpolish(m_statusLabel);
    m_statusLabel->style()->polish(m_statusLabel);
}

void MainWindow::applyDemoState()
{
    // A pose that makes every meter show something, for the screenshot.
    static const double pose[NumAxes] = {0.62, -0.35, 0.48, 0.25, -0.18, 0.72};
    for (int i = 0; i < NumAxes; ++i) {
        m_deflection[i] = pose[i];
        m_axisRows[i]->setLevel(pose[i]);
    }
    m_buttonRows[0]->setLevel(1.0);
    m_buttonRows[1]->setLevel(-1.0);
}

void MainWindow::showAndRaise()
{
    show();
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();
    activateWindow();
    if (m_toggleWindowAction) {
        m_toggleWindowAction->setText(tr("Hide Window"));
    }
}

void MainWindow::toggleWindow()
{
    if (isVisible() && !isMinimized()) {
        hide();
        if (m_toggleWindowAction) {
            m_toggleWindowAction->setText(tr("Show Window"));
        }
    } else {
        showAndRaise();
    }
}

void MainWindow::onTrayActivated(int reason)
{
    switch (static_cast<QSystemTrayIcon::ActivationReason>(reason)) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        toggleWindow();
        break;
    default:
        break;
    }
}

void MainWindow::setAutostartEnabled(bool enabled)
{
    if (autostart::setEnabled(enabled)) {
        setStatusMessage(enabled ? tr("QSpaceMIDI will start with your desktop session.")
                                 : tr("QSpaceMIDI will no longer start automatically."));
        QTimer::singleShot(4000, this, &MainWindow::updateStatus);
        return;
    }

    setStatusMessage(tr("Could not write the autostart entry (%1).")
                         .arg(autostart::location()),
                     true);
    QSignalBlocker blocker(m_autostartCheck);
    m_autostartCheck->setChecked(autostart::isEnabled());
}

void MainWindow::showAbout()
{
    AboutDialog dialog(isVisible() ? this : nullptr);
    dialog.exec();
}

void MainWindow::quitApplication()
{
    m_quitting = true;
    saveWindowGeometry(saveGeometry());
    saveConfig(m_config);
    if (m_tray) {
        m_tray->hide();
    }
    QCoreApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveWindowGeometry(saveGeometry());
    saveConfig(m_config);

    if (m_tray && !m_quitting) {
        // Keep running in the tray, the mapping stays active.
        hide();
        if (m_toggleWindowAction) {
            m_toggleWindowAction->setText(tr("Show Window"));
        }
        if (!m_trayHintShown && m_tray->supportsMessages()) {
            m_trayHintShown = true;
            m_tray->showMessage(tr("QSpaceMIDI"),
                                tr("Still running in the system tray. Use the tray "
                                   "icon to show the window again or to quit."),
                                QSystemTrayIcon::Information, 4000);
        }
        event->ignore();
        return;
    }

    event->accept();
}

void MainWindow::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange && m_toggleWindowAction) {
        m_toggleWindowAction->setText(isMinimized() ? tr("Show Window") : tr("Hide Window"));
    }
}

} // namespace qspacemidi
