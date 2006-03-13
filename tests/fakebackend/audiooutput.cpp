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

#include "audiooutput.h"
#include <QVector>
#include <kdebug.h>

#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <iostream>

namespace Phonon
{
namespace Fake
{
AudioOutput::AudioOutput( QObject* parent )
	: AbstractAudioOutput( parent )
	, m_dsp( "/dev/dsp" )
{
	if( !m_dsp.open( QIODevice::WriteOnly ) )
		kdWarning() << "couldn't open /dev/dsp for writing" << endl;
	else
	{
		int fd = m_dsp.handle();
		int format = AFMT_S16_LE;
		int stereo = 1;
		int samplingRate = 44100;
		ioctl( fd, SNDCTL_DSP_SETFMT, &format );
		ioctl( fd, SNDCTL_DSP_STEREO, &stereo );
		ioctl( fd, SNDCTL_DSP_SPEED, &samplingRate );
	}
}

AudioOutput::~AudioOutput()
{
}

QString AudioOutput::name() const
{
	return m_name;
}

float AudioOutput::volume() const
{
	return m_volume;
}

QString AudioOutput::setName( const QString& newName )
{
	m_name = newName;
	return m_name;
}

float AudioOutput::setVolume( float newVolume )
{
	m_volume = newVolume;
	emit volumeChanged( m_volume );
	return m_volume;
}

void AudioOutput::processBuffer( const QVector<float>& buffer )
{
	if( !m_dsp.isOpen() )
		return;

	// we dump the data in /dev/dsp
	qint16* pcm = new qint16[ 2*buffer.size() ]; // 2* for stereo
	char* towrite = reinterpret_cast<char*>( pcm );
	int converted;
	for( int i = 0; i < buffer.size(); ++i )
	{
		converted = static_cast<qint16>( m_volume * buffer[ i ] * static_cast<float>( 0x7FFF ) );
		*pcm++ = converted;
		*pcm++ = converted;
	}
	int size = sizeof( qint16 ) * 2 * buffer.size();
	int written;
	while( size > 0 )
	{
		written = m_dsp.write( towrite, size );
		size = size - written;
		if( size > 0 )
		{
			towrite += written;
			kdWarning() << "only " << written << " bytes written to /dev/dsp" << endl;
		}
	}

	pcm -= 2*buffer.size();
	delete[] pcm;
}

}} //namespace Phonon::Fake

#include "audiooutput.moc"
// vim: sw=4 ts=4 noet
