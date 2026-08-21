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

#include <QElapsedTimer>
#include <QIcon>

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

    /*! The icon the tray should show, honouring Config::trayIconStyle. */
    QIcon trayIcon() const;
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
    void onDeviceIdle();
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
    QTimer *m_idle = nullptr;

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

    /*!
     * \brief Largest deflection seen since the previous tick.
     *
     * The device reports faster than the CC update rate, so a quick flick
     * can begin and end between two ticks.  Sending the extreme of the
     * window rather than whatever happened to be current when the timer
     * fired keeps those flicks from disappearing.
     */
    std::array<double, NumAxes> m_peak{};

    /*!
     * \brief Whether silence from the device means the cap was released.
     *
     * Only true for devices that report deltas: an absolute device always
     * announces the centre position, and the kernel drops repeated equal
     * values, so a steadily held cap is silent and must not be re-centred.
     */
    bool m_idleWatchdog = false;

    /*! Fractional part carried between ticks in the relative modes. */
    std::array<double, NumAxes> m_accumulator{};

    /*! Last CC value sent per axis in absolute mode, -1 when unknown. */
    std::array<int, NumAxes> m_lastAbsolute{};

    /*! Latch state of the buttons in toggle mode. */
    std::array<bool, NumButtons> m_buttonLatch{};

    /*! State last sent for each button, for the indicator hold below. */
    std::array<bool, NumButtons> m_buttonState{};

    /*! When each button's indicator lit up, so a tap stays visible. */
    std::array<QElapsedTimer, NumButtons> m_buttonSince{};

    /*! Axis currently selected by the axis lock, -1 when none. */
    int m_lockedAxis = -1;

    bool m_quitting = false;
    bool m_trayHintShown = false;
};

} // namespace qspacemidi

#endif // QSPACEMIDI_MAINWINDOW_H
