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

#include "audioeffectdescription.h"
#include "audioeffectdescription_p.h"
#include "factory.h"
#include "ifaces/backend.h"

#include <QString>
#include <QSet>

namespace Phonon
{

AudioEffectDescription::AudioEffectDescription()
	: NameDescriptionTuple( *new AudioEffectDescriptionPrivate, -1, QString(), QString() )
{
}

AudioEffectDescription::AudioEffectDescription( const AudioEffectDescription& rhs )
	: NameDescriptionTuple( *new AudioEffectDescriptionPrivate, rhs.index(), rhs.name(), rhs.description() )
{
}

AudioEffectDescription::AudioEffectDescription( int index, const QString& name, const QString& description )
	: NameDescriptionTuple( *new AudioEffectDescriptionPrivate, index, name, description )
{
}

AudioEffectDescription& AudioEffectDescription::operator=( const AudioEffectDescription& rhs )
{
	*d_func() = *rhs.d_func();
	return *this;
}

bool AudioEffectDescription::operator==( const AudioEffectDescription& rhs ) const
{
	return *d_func() == *rhs.d_func();
}

AudioEffectDescription AudioEffectDescription::fromIndex( int index )
{
	const Ifaces::Backend* b = Factory::self()->backend();
	if( b->audioEffectIndexes().contains( index ) )
		return AudioEffectDescription( index,
				b->audioEffectName( index ),
				b->audioEffectDescription( index ) );
	else
		return AudioEffectDescription(); //isValid() == false
}

} // namespace Phonon

// vim: sw=4 ts=4 noet
