/*
 * knob.h - small Aqua style rotary knob
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_KNOB_H
#define QSPACEMIDI_KNOB_H

#include <QWidget>

namespace qspacemidi {

/*!
 * \brief The little sensitivity dial at the right of every axis row.
 *
 * Drag up or down (or use the wheel, the arrow keys or Page Up/Down) to
 * change the value; double click resets it to the default.
 */
class Knob : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit Knob(QWidget *parent = nullptr);

    /*! Current value, within [minimum(), maximum()]. */
    int value() const { return m_value; }

    int minimum() const { return m_minimum; }
    int maximum() const { return m_maximum; }

    /*! Sets the inclusive value range; the value is clamped into it. */
    void setRange(int minimum, int maximum);

    /*! Value a double click returns to. */
    void setDefaultValue(int value);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    /*! Sets the value, clamped to the range; emits valueChanged(). */
    void setValue(int value);

signals:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    int m_minimum = 0;
    int m_maximum = 100;
    int m_value = 50;
    int m_defaultValue = 50;
    bool m_dragging = false;
    int m_dragStartY = 0;
    int m_dragStartValue = 50;
};

} // namespace qspacemidi

#endif // QSPACEMIDI_KNOB_H
