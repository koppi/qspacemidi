/*
 * knob.cpp - small Aqua style rotary knob
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "widgets/knob.h"

#include <QConicalGradient>
#include <QKeyEvent>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QRadialGradient>
#include <QWheelEvent>

#include <algorithm>
#include <cmath>

namespace qspacemidi {

namespace {

/*! Sweep of the indicator, in degrees, centred on "up". */
constexpr double KnobSweep = 280.0;

/*! Pixels of vertical drag needed to travel the whole range. */
constexpr double KnobDragRange = 140.0;

} // namespace

Knob::Knob(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::SizeVerCursor);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void Knob::setRange(int minimum, int maximum)
{
    m_minimum = std::min(minimum, maximum);
    m_maximum = std::max(minimum, maximum);
    setValue(m_value);
}

void Knob::setDefaultValue(int value)
{
    m_defaultValue = std::clamp(value, m_minimum, m_maximum);
}

void Knob::setValue(int value)
{
    const int clamped = std::clamp(value, m_minimum, m_maximum);
    if (clamped == m_value) {
        return;
    }
    m_value = clamped;
    update();
    emit valueChanged(m_value);
}

QSize Knob::sizeHint() const
{
    return QSize(24, 24);
}

QSize Knob::minimumSizeHint() const
{
    return QSize(20, 20);
}

void Knob::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const double side = std::min(width(), height()) - 2.0;
    const QRectF body((width() - side) / 2.0, (height() - side) / 2.0, side, side);

    // Outer bezel.
    QLinearGradient bezel(body.topLeft(), body.bottomLeft());
    bezel.setColorAt(0.0, QColor(0xfd, 0xfd, 0xfd));
    bezel.setColorAt(1.0, QColor(0xcf, 0xcf, 0xcf));
    painter.setPen(QPen(QColor(0x9a, 0x9a, 0x9a), 1.0));
    painter.setBrush(bezel);
    painter.drawEllipse(body);

    // Inset face.
    const QRectF face = body.adjusted(3.0, 3.0, -3.0, -3.0);
    QRadialGradient glass(face.center() - QPointF(0.0, face.height() * 0.2),
                          face.width());
    glass.setColorAt(0.0, QColor(0xff, 0xff, 0xff));
    glass.setColorAt(1.0, QColor(0xe2, 0xe2, 0xe2));
    painter.setPen(Qt::NoPen);
    painter.setBrush(glass);
    painter.drawEllipse(face);

    // Indicator.
    const double span = std::max(1, m_maximum - m_minimum);
    const double t = (m_value - m_minimum) / span;
    const double angle = (-90.0 - KnobSweep / 2.0 + t * KnobSweep) * M_PI / 180.0;
    const double radius = face.width() / 2.0 - 1.5;
    const QPointF centre = face.center();
    const QPointF tip(centre.x() + std::cos(angle) * radius,
                      centre.y() + std::sin(angle) * radius);

    painter.setPen(QPen(isEnabled() ? QColor(0x44, 0x44, 0x44) : QColor(0xa0, 0xa0, 0xa0),
                        2.0, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(centre.x() + std::cos(angle) * radius * 0.35,
                             centre.y() + std::sin(angle) * radius * 0.35),
                     tip);

    if (hasFocus()) {
        painter.setPen(QPen(QColor(0x4a, 0x90, 0xd9), 1.0));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(body.adjusted(-1.0, -1.0, 1.0, 1.0));
    }
}

void Knob::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }
    m_dragging = true;
    m_dragStartY = static_cast<int>(event->position().y());
    m_dragStartValue = m_value;
    setFocus(Qt::MouseFocusReason);
    event->accept();
}

void Knob::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dragging) {
        QWidget::mouseMoveEvent(event);
        return;
    }
    const double delta = m_dragStartY - event->position().y();
    const double span = m_maximum - m_minimum;
    setValue(m_dragStartValue + static_cast<int>(std::lround(delta / KnobDragRange * span)));
    event->accept();
}

void Knob::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void Knob::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setValue(m_defaultValue);
        event->accept();
        return;
    }
    QWidget::mouseDoubleClickEvent(event);
}

void Knob::wheelEvent(QWheelEvent *event)
{
    const int steps = event->angleDelta().y() / 120;
    if (steps != 0) {
        setValue(m_value + steps);
        event->accept();
        return;
    }
    QWidget::wheelEvent(event);
}

void Knob::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Right:
        setValue(m_value + 1);
        break;
    case Qt::Key_Down:
    case Qt::Key_Left:
        setValue(m_value - 1);
        break;
    case Qt::Key_PageUp:
        setValue(m_value + 10);
        break;
    case Qt::Key_PageDown:
        setValue(m_value - 10);
        break;
    case Qt::Key_Home:
        setValue(m_minimum);
        break;
    case Qt::Key_End:
        setValue(m_maximum);
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    event->accept();
}

} // namespace qspacemidi
