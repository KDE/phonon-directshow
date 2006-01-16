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

#include "audiopath.h"
#include "audioeffect.h"
#include "abstractaudiooutput.h"

namespace Phonon
{
namespace Fake
{

AudioPath::AudioPath( QObject* parent )
	: QObject( parent )
{
}

AudioPath::~AudioPath()
{
}

int AudioPath::availableChannels() const
{
	return 1;
}

QString AudioPath::channelName( int idx ) const
{
	if( idx == 1 )
		return "default";
	return QString();
}

bool AudioPath::selectChannel( int idx )
{
	if( idx == 1 )
		return true;
	return false;
}

int AudioPath::selectedChannel() const
{
	return 1;
}

bool AudioPath::addOutput( Ifaces::AbstractAudioOutput* audioOutput )
{
	Q_ASSERT( audioOutput );
	AbstractAudioOutput* ao = qobject_cast<AbstractAudioOutput*>( audioOutput->qobject() );
	Q_ASSERT( ao );
	if( m_outputs.contains( ao ) )
		return false;
	m_outputs.append( ao );
	return true;
}

bool AudioPath::removeOutput( Ifaces::AbstractAudioOutput* audioOutput )
{
	Q_ASSERT( audioOutput );
	AbstractAudioOutput* ao = qobject_cast<AbstractAudioOutput*>( audioOutput->qobject() );
	Q_ASSERT( ao );
	if( m_outputs.removeAll( ao ) > 0 )
		return true;
	return false;
}

bool AudioPath::insertEffect( Ifaces::AudioEffect* newEffect, Ifaces::AudioEffect* insertBefore )
{
	Q_ASSERT( newEffect );
	AudioEffect* ae = qobject_cast<AudioEffect*>( newEffect->qobject() );
	Q_ASSERT( ae );
	AudioEffect* before = 0;
	if( insertBefore )
	{
		before = qobject_cast<AudioEffect*>( insertBefore->qobject() );
		Q_ASSERT( before );
		if( !m_effects.contains( before ) )
			return false;
		m_effects.insert( m_effects.indexOf( before ), ae );
	}
	else
		m_effects.append( ae );

	return true;
}

bool AudioPath::removeEffect( Ifaces::AudioEffect* effect )
{
	Q_ASSERT( effect );
	AudioEffect* ae = qobject_cast<AudioEffect*>( effect->qobject() );
	Q_ASSERT( ae );
	if( m_effects.removeAll( ae ) > 0 )
		return true;
	return false;
}
}}

#include "audiopath.moc"
// vim: sw=4 ts=4 noet
