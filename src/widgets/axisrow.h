/*
 * axisrow.h - one "label / meter / mode / CC / knob" row
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_AXISROW_H
#define QSPACEMIDI_AXISROW_H

#include <QList>
#include <QPair>
#include <QString>
#include <QWidget>

class QComboBox;
class QLabel;
class QSpinBox;
class QToolButton;

namespace qspacemidi {

class Knob;
class LevelMeter;

/*!
 * \brief A single configuration row, as used by MIDI Navigator.
 *
 * From left to right: the axis letter, a green level meter showing the
 * current deflection, the CC mode, the CC number and a sensitivity knob.
 * Muting and direction inversion live in the row's context menu so that
 * the layout stays as compact as the original.
 */
class AxisRow : public QWidget
{
    Q_OBJECT

public:
    /*! Mode entries are shown as (label, value) pairs in the combo box. */
    using ModeList = QList<QPair<QString, int>>;

    explicit AxisRow(const QString &label, QWidget *parent = nullptr);

    /*! Fills the mode combo box. */
    void setModes(const ModeList &modes);

    /*! Currently selected mode value, or -1 when the list is empty. */
    int mode() const;
    void setMode(int mode);

    int cc() const;
    void setCc(int cc);

    int sensitivity() const;
    void setSensitivity(int sensitivity);

    /*! Hides the sensitivity knob but keeps the column width. */
    void setSensitivityEnabled(bool enabled);

    /*! Shows or hides the invert toggle at the end of the row. */
    void setInvertEnabled(bool enabled);

    /*! Muted rows send nothing and are drawn greyed out. */
    bool isMuted() const { return m_muted; }
    void setMuted(bool muted);

    bool isInverted() const { return m_inverted; }
    void setInverted(bool inverted);

    /*! Deflection shown by the level meter, in [-1, 1]. */
    void setLevel(double level);

    /*! Draws the meter filled from the centre (used for the buttons). */
    void setBipolarMeter(bool bipolar);

signals:
    void modeChanged(int mode);
    void ccChanged(int cc);
    void sensitivityChanged(int sensitivity);
    void mutedChanged(bool muted);
    void invertedChanged(bool inverted);

private:
    void showContextMenu(const QPoint &pos);
    void updateMutedLook();

    QLabel *m_label = nullptr;
    LevelMeter *m_meter = nullptr;
    QComboBox *m_mode = nullptr;
    QSpinBox *m_cc = nullptr;
    Knob *m_knob = nullptr;
    QWidget *m_knobPlaceholder = nullptr;
    QToolButton *m_invert = nullptr;
    bool m_muted = false;
    bool m_inverted = false;
};

} // namespace qspacemidi

#endif // QSPACEMIDI_AXISROW_H
