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

#ifndef PHONON_DEINTERLACEFILTER_H
#define PHONON_DEINTERLACEFILTER_H

#include "phonon_export.h"
#include "effect.h"

namespace Phonon
{
class DeinterlaceFilterPrivate;

/** \class DeinterlaceFilter deinterlacefilter.h Phonon/DeinterlaceFilter
 * Video effect to deinterlace the image.
 *
 * \ingroup PhononEffects
 * \ingroup PhononVideo
 * \author Matthias Kretz <kretz@kde.org>
 */
class PHONON_EXPORT DeinterlaceFilter : public Effect
{
    Q_OBJECT
    K_DECLARE_PRIVATE(DeinterlaceFilter)
    PHONON_HEIR(DeinterlaceFilter)
};
} //namespace Phonon

#endif // PHONON_DEINTERLACEFILTER_H
