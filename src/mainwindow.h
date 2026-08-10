/*
 * mainwindow.h - main window of qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_MAINWINDOW_H
#define QSPACEMIDI_MAINWINDOW_H

#include "mapping.h"
#include "spacenavigator.h"

#include <QWidget>

#include <array>

class QAction;
class QCheckBox;
class QComboBox;
class QLabel;
class QMenu;
class QSpinBox;
class QSystemTrayIcon;
class QTimer;

namespace qspacemidi {

class AxisRow;
class MidiEngine;

/*!
 * \brief The MIDI Navigator style main window.
 *
 * Holds the device connection, the MIDI output and the mapping state.  A
 * timer converts the current axis deflections into CC messages at the
 * configured update rate, so that a deflected cap keeps sending in the
 * relative modes and the absolute modes do not flood the host.
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    /*! Brings the window to the front, restoring it when minimised. */
    void showAndRaise();

    /*! Fills the meters with a demo pose; used by --screenshot. */
    void applyDemoState();

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    void buildUi();
    void buildTrayIcon();
    void connectDevice();
    void applyConfigToUi();
    void refreshPortList();
    void openConfiguredPort();
    void updateStatus();
    void setStatusMessage(const QString &message, bool isError = false);

    void onAxesNormChanged(const SpaceNavigator::AxesNorm &axes);
    void onButtonChanged(int button, bool pressed);
    void onDeviceOpened();
    void onDeviceClosed();
    void onTick();
    void onTrayActivated(int reason);
    void setAutostartEnabled(bool enabled);
    void showAbout();
    void toggleWindow();
    void quitApplication();

    Config m_config;

    SpaceNavigator *m_device = nullptr;
    MidiEngine *m_midi = nullptr;
    QTimer *m_tick = nullptr;
    QTimer *m_deviceRetry = nullptr;

    std::array<AxisRow *, NumAxes> m_axisRows{};
    std::array<AxisRow *, NumButtons> m_buttonRows{};

    QComboBox *m_portCombo = nullptr;
    QSpinBox *m_channelSpin = nullptr;
    QSpinBox *m_deadZoneSpin = nullptr;
    QCheckBox *m_axisLockCheck = nullptr;
    QCheckBox *m_autostartCheck = nullptr;
    QLabel *m_statusLabel = nullptr;

    QSystemTrayIcon *m_tray = nullptr;
    QMenu *m_trayMenu = nullptr;
    QAction *m_toggleWindowAction = nullptr;

    /*! Latest normalised deflections, updated from the device signal. */
    std::array<double, NumAxes> m_deflection{};

    /*! Fractional part carried between ticks in the relative modes. */
    std::array<double, NumAxes> m_accumulator{};

    /*! Last CC value sent per axis in absolute mode, -1 when unknown. */
    std::array<int, NumAxes> m_lastAbsolute{};

    /*! Latch state of the buttons in toggle mode. */
    std::array<bool, NumButtons> m_buttonLatch{};

    /*! Axis currently selected by the axis lock, -1 when none. */
    int m_lockedAxis = -1;

    bool m_quitting = false;
    bool m_trayHintShown = false;
};

} // namespace qspacemidi

#endif // QSPACEMIDI_MAINWINDOW_H
