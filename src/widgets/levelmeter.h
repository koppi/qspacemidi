/*
 * levelmeter.h - Aqua style horizontal level meter
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_LEVELMETER_H
#define QSPACEMIDI_LEVELMETER_H

#include <QWidget>

namespace qspacemidi {

/*!
 * \brief Green horizontal level meter in the style of the Mac OS X
 *        NSLevelIndicator used by MIDI Navigator.
 *
 * The meter shows a bipolar value: -1 fills nothing, 0 fills half the
 * track and +1 fills it completely, so a 3D mouse at rest sits in the
 * middle.
 */
class LevelMeter : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double value READ value WRITE setValue)

public:
    explicit LevelMeter(QWidget *parent = nullptr);

    /*! Current value in [-1, 1]. */
    double value() const { return m_value; }

    /*! Sets the value; out of range values are clamped. */
    void setValue(double value);

    /*! Draws the fill from the centre outwards instead of from the left. */
    void setBipolarFill(bool bipolar);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_value = 0.0;
    bool m_bipolarFill = false;
};

} // namespace qspacemidi

#endif // QSPACEMIDI_LEVELMETER_H
