/*  This file is part of the KDE project
    Copyright (C) 2006-2007 Matthias Kretz <kretz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#include "volumeslider.h"
#include "volumeslider_p.h"
#include "audiooutput.h"
#include "phonondefs_p.h"
#include "phononnamespace_p.h"
#include "factory.h"

namespace Phonon
{


VolumeSlider::VolumeSlider(QWidget *parent)
    : QWidget(parent),
    k_ptr(new VolumeSliderPrivate(this))
{
    K_D(VolumeSlider);
    setToolTip(i18n("Volume: %1%", QString::number(100)));
    setWhatsThis(i18n("Use this slider to adjust the volume. The leftmost position is 0%, the rightmost is %1%", QString::number(100)));

    connect(&d->slider, SIGNAL(valueChanged(int)), SLOT(_k_sliderChanged(int)));
    connect(&d->muteButton, SIGNAL(clicked()), SLOT(_k_buttonClicked()));
}

VolumeSlider::VolumeSlider(AudioOutput *output, QWidget *parent)
    : QWidget(parent),
    k_ptr(new VolumeSliderPrivate(this))
{
    K_D(VolumeSlider);
    setToolTip(i18n("Volume: %1%", QString::number(100)));
    setWhatsThis(i18n("Use this slider to adjust the volume. The leftmost position is 0%, the rightmost is %1%", QString::number(100)));

    connect(&d->slider, SIGNAL(valueChanged(int)), SLOT(_k_sliderChanged(int)));
    connect(&d->muteButton, SIGNAL(clicked()), SLOT(_k_buttonClicked()));

    if (output) {
        d->output = output;
        d->slider.setValue(qRound(100 * output->volume()));
        d->slider.setEnabled(true);
        d->muteButton.setEnabled(true);
        connect(output, SIGNAL(volumeChanged(qreal)), SLOT(_k_volumeChanged(qreal)));
        connect(output, SIGNAL(mutedChanged(bool)), SLOT(_k_mutedChanged(bool)));
    }
}

VolumeSlider::~VolumeSlider()
{
    delete k_ptr;
}

bool VolumeSlider::isMuteVisible() const
{
    return k_ptr->muteButton.isVisible();
}

void VolumeSlider::setMuteVisible(bool visible)
{
    k_ptr->muteButton.setVisible(visible);
}

QSize VolumeSlider::iconSize() const
{
    return k_ptr->muteButton.iconSize();
}

void VolumeSlider::setIconSize(const QSize &iconSize)
{
    pDebug() << Q_FUNC_INFO << iconSize;
    k_ptr->muteButton.setIconSize(iconSize);
}

qreal VolumeSlider::maximumVolume() const
{
    return k_ptr->slider.maximum() * 0.01;
}

void VolumeSlider::setMaximumVolume(qreal volume)
{
    int max = static_cast<int>(volume * 100);
    k_ptr->slider.setMaximum(max);
    setWhatsThis(i18n("Use this slider to adjust the volume. The leftmost position is 0%, the rightmost is %1%",
                QString::number(max)));
}

Qt::Orientation VolumeSlider::orientation() const
{
    return k_ptr->slider.orientation();
}

void VolumeSlider::setOrientation(Qt::Orientation o)
{
    K_D(VolumeSlider);
    Qt::Alignment align = (o == Qt::Horizontal ? Qt::AlignVCenter : Qt::AlignHCenter);
    d->layout.setAlignment(&d->muteButton, align);
    d->layout.setAlignment(&d->slider, align);
    d->layout.setDirection(o == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
    d->slider.setOrientation(o);
}

AudioOutput *VolumeSlider::audioOutput() const
{
    K_D(const VolumeSlider);
    return d->output;
}

void VolumeSlider::setAudioOutput(AudioOutput *output)
{
    K_D(VolumeSlider);
    if (d->output) {
        disconnect(d->output, 0, this, 0);
    }
    d->output = output;
    if (output) {
        d->slider.setValue(qRound(100 * output->volume()));
        d->slider.setEnabled(true);
        d->muteButton.setEnabled(true);
        connect(output, SIGNAL(volumeChanged(qreal)), SLOT(_k_volumeChanged(qreal)));
        connect(output, SIGNAL(mutedChanged(bool)), SLOT(_k_mutedChanged(bool)));
    } else {
        d->slider.setValue(100);
        d->slider.setEnabled(false);
        d->muteButton.setEnabled(false);
    }
}

void VolumeSliderPrivate::_k_buttonClicked()
{
    if (output) {
        output->setMuted(!output->isMuted());
    } else {
        slider.setEnabled(false);
        muteButton.setEnabled(false);
    }
}

void VolumeSliderPrivate::_k_mutedChanged(bool muted)
{
    Q_Q(VolumeSlider);
    if (muted) {
        q->setToolTip(i18n("Muted"));
        muteButton.setIcon(mutedIcon);
    } else {
        q->setToolTip(i18n("Volume: %1%", QString::number(static_cast<int>(output->volume() * 100.0))));
        muteButton.setIcon(volumeIcon);
    }
}

void VolumeSliderPrivate::_k_sliderChanged(int value)
{
    Q_Q(VolumeSlider);

    if (output) {
        if (!output->isMuted()) {
           q->setToolTip(i18n("Volume: %1%", QString::number(value)));
        }

        ignoreVolumeChange = true;
        output->setVolume((static_cast<qreal>(value)) * 0.01);
        ignoreVolumeChange = false;
    } else {
        slider.setEnabled(false);
        muteButton.setEnabled(false);
    }
}

void VolumeSliderPrivate::_k_volumeChanged(qreal value)
{
    if (!ignoreVolumeChange) {
        slider.setValue(qRound(100 * value));
    }
}

bool VolumeSlider::hasTracking() const
{
    return k_ptr->slider.hasTracking();
}

void VolumeSlider::setTracking(bool tracking)
{
    k_ptr->slider.setTracking(tracking);
}

int VolumeSlider::pageStep() const
{
    return k_ptr->slider.pageStep();
}

void VolumeSlider::setPageStep(int milliseconds)
{
    k_ptr->slider.setPageStep(milliseconds);
}

int VolumeSlider::singleStep() const
{
    return k_ptr->slider.singleStep();
}

void VolumeSlider::setSingleStep(int milliseconds)
{
    k_ptr->slider.setSingleStep(milliseconds);
}

} // namespace Phonon

#include "volumeslider.moc"
// vim: sw=4 et
