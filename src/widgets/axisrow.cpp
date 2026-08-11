/*
 * axisrow.cpp - one "label / meter / mode / CC / knob" row
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "widgets/axisrow.h"

#include "widgets/knob.h"
#include "widgets/levelmeter.h"

#include <QAction>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QSpinBox>
#include <QToolButton>

namespace qspacemidi {

namespace {

constexpr int LabelWidth = 30;
constexpr int MeterWidth = 128;
constexpr int ModeWidth = 168;
constexpr int CcWidth = 62;
constexpr int KnobWidth = 24;
constexpr int InvertWidth = 24;

} // namespace

AxisRow::AxisRow(const QString &label, QWidget *parent)
    : QWidget(parent)
{
    m_label = new QLabel(label, this);
    m_label->setObjectName(QStringLiteral("axisLabel"));
    m_label->setFixedWidth(LabelWidth);

    m_meter = new LevelMeter(this);
    m_meter->setFixedWidth(MeterWidth);

    m_mode = new QComboBox(this);
    m_mode->setFixedWidth(ModeWidth);
    m_mode->setFocusPolicy(Qt::StrongFocus);

    m_cc = new QSpinBox(this);
    m_cc->setRange(0, 127);
    m_cc->setFixedWidth(CcWidth);
    m_cc->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_cc->setToolTip(tr("MIDI control change number"));

    m_knob = new Knob(this);
    m_knob->setRange(0, 100);
    m_knob->setValue(50);
    m_knob->setDefaultValue(50);
    m_knob->setFixedWidth(KnobWidth);
    m_knob->setToolTip(tr("Sensitivity - drag up or down, double click to reset"));

    // Keeps the knob column occupied on rows that have no knob, so that all
    // rows keep the same column positions.
    m_knobPlaceholder = new QWidget(this);
    m_knobPlaceholder->setFixedWidth(KnobWidth);
    m_knobPlaceholder->hide();

    m_invert = new QToolButton(this);
    m_invert->setObjectName(QStringLiteral("invertButton"));
    m_invert->setText(QStringLiteral("±"));
    m_invert->setCheckable(true);
    m_invert->setFixedWidth(InvertWidth);
    m_invert->setFocusPolicy(Qt::TabFocus);
    m_invert->setToolTip(tr("Invert the direction of this axis"));

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(6, 2, 6, 2);
    layout->setSpacing(8);
    layout->addWidget(m_label);
    layout->addWidget(m_meter);
    layout->addWidget(m_mode);
    layout->addWidget(m_cc);
    layout->addWidget(m_knob);
    layout->addWidget(m_knobPlaceholder);
    layout->addWidget(m_invert);
    layout->addStretch(1);

    connect(m_mode, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (index >= 0) {
            emit modeChanged(m_mode->itemData(index).toInt());
        }
    });
    connect(m_cc, &QSpinBox::valueChanged, this, &AxisRow::ccChanged);
    connect(m_knob, &Knob::valueChanged, this, &AxisRow::sensitivityChanged);
    connect(m_invert, &QToolButton::toggled, this, &AxisRow::setInverted);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &AxisRow::showContextMenu);
}

void AxisRow::setModes(const ModeList &modes)
{
    QSignalBlocker blocker(m_mode);
    m_mode->clear();
    for (const auto &[label, value] : modes) {
        m_mode->addItem(label, value);
    }
}

int AxisRow::mode() const
{
    const int index = m_mode->currentIndex();
    return index >= 0 ? m_mode->itemData(index).toInt() : -1;
}

void AxisRow::setMode(int mode)
{
    const int index = m_mode->findData(mode);
    if (index >= 0 && index != m_mode->currentIndex()) {
        QSignalBlocker blocker(m_mode);
        m_mode->setCurrentIndex(index);
    }
}

int AxisRow::cc() const
{
    return m_cc->value();
}

void AxisRow::setCc(int cc)
{
    QSignalBlocker blocker(m_cc);
    m_cc->setValue(cc);
}

int AxisRow::sensitivity() const
{
    return m_knob->value();
}

void AxisRow::setSensitivity(int sensitivity)
{
    QSignalBlocker blocker(m_knob);
    m_knob->setValue(sensitivity);
}

void AxisRow::setSensitivityEnabled(bool enabled)
{
    // Keep the space so that all rows line up, only hide the control.
    m_knob->setVisible(enabled);
    m_knobPlaceholder->setVisible(!enabled);
}

void AxisRow::setInvertEnabled(bool enabled)
{
    m_invert->setVisible(enabled);
}

void AxisRow::setMuted(bool muted)
{
    if (m_muted == muted) {
        return;
    }
    m_muted = muted;
    updateMutedLook();
    emit mutedChanged(m_muted);
}

void AxisRow::setInverted(bool inverted)
{
    if (m_inverted == inverted) {
        return;
    }
    m_inverted = inverted;
    if (m_invert->isChecked() != inverted) {
        QSignalBlocker blocker(m_invert);
        m_invert->setChecked(inverted);
    }
    emit invertedChanged(m_inverted);
}

void AxisRow::setLevel(double level)
{
    m_meter->setValue(m_muted ? m_restLevel : level);
}

void AxisRow::setRestLevel(double level)
{
    m_restLevel = level;
}

void AxisRow::setBipolarMeter(bool bipolar)
{
    m_meter->setBipolarFill(bipolar);
}

void AxisRow::updateMutedLook()
{
    m_label->setEnabled(!m_muted);
    m_meter->setEnabled(!m_muted);
    m_mode->setEnabled(!m_muted);
    m_cc->setEnabled(!m_muted);
    m_knob->setEnabled(!m_muted);
    m_invert->setEnabled(!m_muted);
    m_meter->setValue(m_restLevel);
}

void AxisRow::showContextMenu(const QPoint &pos)
{
    QMenu menu(this);

    QAction *mute = menu.addAction(tr("Mute"));
    mute->setCheckable(true);
    mute->setChecked(m_muted);

    QAction *invert = menu.addAction(tr("Invert direction"));
    invert->setCheckable(true);
    invert->setChecked(m_inverted);
    invert->setEnabled(!m_muted);

    menu.addSeparator();
    QAction *reset = menu.addAction(tr("Reset sensitivity"));
    reset->setEnabled(!m_muted && m_knob->isVisible());

    QAction *chosen = menu.exec(mapToGlobal(pos));
    if (chosen == mute) {
        setMuted(mute->isChecked());
    } else if (chosen == invert) {
        setInverted(invert->isChecked());
    } else if (chosen == reset) {
        m_knob->setValue(50);
    }
}

} // namespace qspacemidi
