/*  This file is part of the KDE project
    Copyright (C) 2006 Matthias Kretz <kretz@kde.org>

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

#ifndef VOLUMESLIDER_P_H
#define VOLUMESLIDER_P_H

#include "volumeslider.h"
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QPixmap>
#include <QToolButton>
#include <kicon.h>

namespace Phonon
{
class AudioOutput;
class VolumeSliderPrivate
{
    Q_DECLARE_PUBLIC( VolumeSlider )
    protected:
        VolumeSliderPrivate( VolumeSlider* parent )
            : q_ptr(parent),
            outerlayout(QBoxLayout::TopToBottom, parent),
            layout(QBoxLayout::LeftToRight),
            slider(Qt::Horizontal, parent),
            icon(parent),
            muteButton(parent),
            volumeIcon("player_volume"),
            muteIcon("player_mute"),
            unmuteIcon("player_unmute"),
            output(0),
            ignoreVolumeChange(false)
        {
            slider.setRange(0, 100);
            slider.setPageStep(5);
            slider.setSingleStep(1);

            muteButton.setCheckable(true);

            icon.setPixmap(volumeIcon.pixmap(16));
            icon.setAlignment(Qt::AlignCenter);
            muteButton.setIcon(muteIcon);
            muteButton.setAutoRaise(true);
            outerlayout.addStretch();
            outerlayout.addLayout(&layout);
            outerlayout.addStretch();
            layout.setMargin(0);
            layout.setSpacing(2);
            layout.addWidget(&icon);
            layout.addWidget(&muteButton);
            layout.addWidget(&slider);
        }

        VolumeSlider *q_ptr;

        void _k_outputDestroyed();
        void _k_sliderChanged(int);
        void _k_volumeChanged(float);
        void _k_mutedChanged(bool);
        void _k_buttonToggled(bool);

    private:
        QBoxLayout outerlayout;
        QBoxLayout layout;
        QSlider slider;
        QLabel icon;
        QToolButton muteButton;
        KIcon volumeIcon;
        KIcon muteIcon;
        KIcon unmuteIcon;

        AudioOutput *output;
        bool ignoreVolumeChange;
};
} // namespace Phonon

#endif // VOLUMESLIDER_P_H
// vim: sw=4 sts=4 et tw=100
