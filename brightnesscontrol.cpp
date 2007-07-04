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

#include "brightnesscontrol.h"
#include "brightnesscontrol_p.h"
#include "factory.h"
#include "effectparameter.h"

#define PHONON_CLASSNAME BrightnessControl

namespace Phonon
{
PHONON_HEIR_IMPL(Effect)

PHONON_GETTER(int, brightness, d->brightness)
PHONON_GETTER(int, lowerBound, -1000)
PHONON_GETTER(int, upperBound,  1000)
PHONON_SETTER(setBrightness, brightness, int)

bool BrightnessControlPrivate::aboutToDeleteBackendObject()
{
    if (m_backendObject)
        pBACKEND_GET(int, brightness, "brightness");
    return true;
}

void BrightnessControlPrivate::setupBackendObject()
{
    Q_ASSERT(m_backendObject);

    // set up attributes
    pBACKEND_CALL1("setBrightness", int, brightness);
}
} // namespace Phonon

#undef PHONON_CLASSNAME

#include "moc_brightnesscontrol.cpp"

// vim: sw=4 ts=4
