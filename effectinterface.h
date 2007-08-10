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

#ifndef PHONON_EFFECTINTERFACE_H
#define PHONON_EFFECTINTERFACE_H

#include <QtCore/QVariant>

namespace Phonon
{
    class EffectParameter;
    /**
     * \author Matthias Kretz <kretz@kde.org>
     */
    class EffectInterface
    {
        public:
            virtual ~EffectInterface() {}
            virtual QList<EffectParameter> parameters() const = 0;
            virtual QVariant parameterValue(const EffectParameter &) const = 0;
            virtual void setParameterValue(const EffectParameter &, const QVariant &newValue) = 0;
    };
} //namespace Phonon::Fake

Q_DECLARE_INTERFACE(Phonon::EffectInterface, "EffectInterface0.phonon.kde.org")

#endif // PHONON_EFFECTINTERFACE_H
