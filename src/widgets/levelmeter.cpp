/*
 * levelmeter.cpp - Aqua style horizontal level meter
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "widgets/levelmeter.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>

#include <algorithm>
#include <cmath>

namespace qspacemidi {

LevelMeter::LevelMeter(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void LevelMeter::setValue(double value)
{
    const double clamped = std::clamp(value, -1.0, 1.0);
    if (std::abs(clamped - m_value) < 0.0005) {
        return;
    }
    m_value = clamped;
    update();
}

void LevelMeter::setBipolarFill(bool bipolar)
{
    if (m_bipolarFill == bipolar) {
        return;
    }
    m_bipolarFill = bipolar;
    update();
}

QSize LevelMeter::sizeHint() const
{
    return QSize(120, 12);
}

QSize LevelMeter::minimumSizeHint() const
{
    return QSize(48, 12);
}

void LevelMeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF track(0.5, (height() - 11) / 2.0 + 0.5, width() - 1.0, 10.0);
    const double radius = track.height() / 2.0;

    // Recessed track.
    QLinearGradient trackGradient(track.topLeft(), track.bottomLeft());
    trackGradient.setColorAt(0.0, QColor(0xd6, 0xd6, 0xd6));
    trackGradient.setColorAt(0.35, QColor(0xe9, 0xe9, 0xe9));
    trackGradient.setColorAt(1.0, QColor(0xf6, 0xf6, 0xf6));

    painter.setPen(QPen(QColor(0xa8, 0xa8, 0xa8), 1.0));
    painter.setBrush(trackGradient);
    painter.drawRoundedRect(track, radius, radius);

    // Fill.
    const double normalized = (m_value + 1.0) / 2.0;
    QRectF fill = track.adjusted(1.0, 1.0, -1.0, -1.0);
    if (m_bipolarFill) {
        const double centre = fill.center().x();
        const double half = fill.width() / 2.0;
        if (m_value >= 0.0) {
            fill.setLeft(centre);
            fill.setWidth(half * m_value);
        } else {
            fill.setLeft(centre + half * m_value);
            fill.setRight(centre);
        }
    } else {
        fill.setWidth(fill.width() * normalized);
    }

    if (fill.width() > 0.5) {
        QLinearGradient fillGradient(fill.topLeft(), fill.bottomLeft());
        fillGradient.setColorAt(0.0, QColor(0xa9, 0xdd, 0x62));
        fillGradient.setColorAt(0.45, QColor(0x86, 0xcc, 0x2f));
        fillGradient.setColorAt(0.55, QColor(0x71, 0xbe, 0x1e));
        fillGradient.setColorAt(1.0, QColor(0x8f, 0xd0, 0x41));

        const double fillRadius = std::min(radius - 1.0, fill.width() / 2.0);
        painter.setPen(Qt::NoPen);
        painter.setBrush(fillGradient);
        painter.drawRoundedRect(fill, fillRadius, fillRadius);
    }

    // Centre tick, so the resting position is easy to spot.
    const double centreX = std::round(track.center().x()) + 0.5;
    painter.setPen(QPen(QColor(0x00, 0x00, 0x00, 0x3c), 1.0));
    painter.drawLine(QPointF(centreX, track.top() + 2.0),
                     QPointF(centreX, track.bottom() - 2.0));
}

} // namespace qspacemidi
