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

#ifndef AUDIOOUTPUT_P_H
#define AUDIOOUTPUT_P_H

#include "audiooutput.h"
#include "abstractaudiooutput_p.h"
#include <kaboutdata.h>
#include <kglobal.h>
#include <kcomponentdata.h>

namespace Phonon
{
class AudioOutputPrivate : public AbstractAudioOutputPrivate
{
    K_DECLARE_PUBLIC(AudioOutput)
    PHONON_PRIVATECLASS
    public:
        void deviceListChanged();
        inline static AudioOutputPrivate *cast(BasePrivate *x)
        {
            if (x && x->castId == BasePrivate::AudioOutputType) {
                return static_cast<AudioOutputPrivate *>(x);
            }
            return 0;
        }

    protected:
        AudioOutputPrivate(CastId castId = BasePrivate::AudioOutputType)
            : AbstractAudioOutputPrivate(castId),
            volume(1.0),
            outputDeviceIndex(-1),
            deviceBeforeFallback(-1),
            outputDeviceOverridden(false),
            muted(false)
        {
            const KAboutData *ad = KGlobal::mainComponent().aboutData();
            if (ad)
                name = ad->programName();
            else
                name = KGlobal::mainComponent().componentName();
        }

        enum DeviceChangeType {
            FallbackChange,
            HigherPreferenceChange
        };
        void handleAutomaticDeviceChange(int newIndex, DeviceChangeType type);

        void _k_volumeChanged(float);
        void _k_revertFallback();
        void _k_audioDeviceFailed();

    private:
        float volume;
        Category category;
        QString name;
        int outputDeviceIndex;
        int deviceBeforeFallback;
        bool outputDeviceOverridden;
        bool muted;
};
} //namespace Phonon

#endif // AUDIOOUTPUT_P_H
// vim: sw=4 ts=4 tw=80
