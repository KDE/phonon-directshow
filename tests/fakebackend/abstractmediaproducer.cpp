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

#include "abstractmediaproducer.h"
#include <QTimer>
#include "videopath.h"
#include "audiopath.h"
#include <kdebug.h>
#include <QVector>
#include <cmath>
#include <QFile>
#include <QByteArray>
#include <QStringList>

namespace Phonon
{
namespace Fake
{
static const int SAMPLE_RATE = 44100;
static const float SAMPLE_RATE_FLOAT = 44100.0f;
static const int FRAME_RATE = 25;
static const int SAMPLES_PER_FRAME = SAMPLE_RATE / FRAME_RATE;

AbstractMediaProducer::AbstractMediaProducer( QObject* parent )
	: QObject( parent )
	, m_state( Phonon::LoadingState )
	, m_tickTimer( new QTimer( this ) )
	, m_bufferSize( SAMPLES_PER_FRAME )//512 )
	, m_lastSamplesMissing( 0 )
	, m_position( 0.0f )
	, m_frequency( 440.0f )
{
	//kDebug( 604 ) << k_funcinfo << endl;
	connect( m_tickTimer, SIGNAL( timeout() ), SLOT( emitTick() ) );
}

AbstractMediaProducer::~AbstractMediaProducer()
{
	//kDebug( 604 ) << k_funcinfo << endl;
}

void AbstractMediaProducer::setBufferSize( int size )
{
    Q_UNUSED(size);
	//m_bufferSize = size;
}

bool AbstractMediaProducer::addVideoPath( QObject* videoPath )
{
	//kDebug( 604 ) << k_funcinfo << endl;
	Q_ASSERT( videoPath );
	VideoPath* vp = qobject_cast<VideoPath*>( videoPath );
	Q_ASSERT( vp );
	Q_ASSERT( !m_videoPathList.contains( vp ) );
	m_videoPathList.append( vp );
	return true;
}

bool AbstractMediaProducer::addAudioPath( QObject* audioPath )
{
	//kDebug( 604 ) << k_funcinfo << endl;
	Q_ASSERT( audioPath );
	AudioPath* ap = qobject_cast<AudioPath*>( audioPath );
	Q_ASSERT( ap );
	Q_ASSERT( !m_audioPathList.contains( ap ) );
	m_audioPathList.append( ap );
	return true;
}

void AbstractMediaProducer::removeVideoPath( QObject* videoPath )
{
	Q_ASSERT( videoPath );
	VideoPath* vp = qobject_cast<VideoPath*>( videoPath );
	Q_ASSERT( vp );
	Q_ASSERT( m_videoPathList.contains( vp ) );
	m_videoPathList.removeAll( vp );
}

void AbstractMediaProducer::removeAudioPath( QObject* audioPath )
{
	Q_ASSERT( audioPath );
	AudioPath* ap = qobject_cast<AudioPath*>( audioPath );
	Q_ASSERT( ap );
	Q_ASSERT( m_audioPathList.contains( ap ) );
	m_audioPathList.removeAll( ap );
}

State AbstractMediaProducer::state() const
{
	//kDebug( 604 ) << k_funcinfo << endl;
	return m_state;
}

bool AbstractMediaProducer::hasVideo() const
{
	//kDebug( 604 ) << k_funcinfo << endl;
	return false;
}

bool AbstractMediaProducer::isSeekable() const
{
	//kDebug( 604 ) << k_funcinfo << endl;
	return true;
}

qint64 AbstractMediaProducer::currentTime() const
{
    //kDebug(604) << k_funcinfo << endl;
	switch( state() )
	{
		case Phonon::PausedState:
		case Phonon::BufferingState:
			return m_startTime.msecsTo( m_pauseTime );
		case Phonon::PlayingState:
			return m_startTime.elapsed();
		case Phonon::StoppedState:
		case Phonon::LoadingState:
			return 0;
		case Phonon::ErrorState:
			break;
	}
	return -1;
}

qint32 AbstractMediaProducer::tickInterval() const
{
	//kDebug( 604 ) << k_funcinfo << endl;
	return m_tickInterval;
}

void AbstractMediaProducer::setTickInterval( qint32 newTickInterval )
{
	//kDebug( 604 ) << k_funcinfo << endl;
	m_tickInterval = newTickInterval;
	if( m_tickInterval <= 0 )
		m_tickTimer->setInterval( 50 );
	else
		m_tickTimer->setInterval( newTickInterval );
}

QStringList AbstractMediaProducer::availableAudioStreams() const
{
	QStringList ret;
	ret << QLatin1String( "en" ) << QLatin1String( "de" );
	return ret;
}

QStringList AbstractMediaProducer::availableVideoStreams() const
{
	QStringList ret;
	ret << QLatin1String( "en" ) << QLatin1String( "de" );
	return ret;
}

QStringList AbstractMediaProducer::availableSubtitleStreams() const
{
	QStringList ret;
	ret << QLatin1String( "en" ) << QLatin1String( "de" );
	return ret;
}

QString AbstractMediaProducer::selectedAudioStream( const QObject* audioPath ) const
{
	return m_selectedAudioStream[ audioPath ];
}

QString AbstractMediaProducer::selectedVideoStream( const QObject* videoPath ) const
{
	return m_selectedVideoStream[ videoPath ];
}

QString AbstractMediaProducer::selectedSubtitleStream( const QObject* videoPath ) const
{
	return m_selectedSubtitleStream[ videoPath ];
}

void AbstractMediaProducer::selectAudioStream( const QString& streamName, const QObject* audioPath )
{
	if( availableAudioStreams().contains( streamName ) )
		m_selectedAudioStream[ audioPath ] = streamName;
}

void AbstractMediaProducer::selectVideoStream( const QString& streamName, const QObject* videoPath )
{
	if( availableVideoStreams().contains( streamName ) )
		m_selectedVideoStream[ videoPath ] = streamName;
}

void AbstractMediaProducer::selectSubtitleStream( const QString& streamName, const QObject* videoPath )
{
	if( availableSubtitleStreams().contains( streamName ) )
		m_selectedSubtitleStream[ videoPath ] = streamName;
}

void AbstractMediaProducer::play()
{
	//kDebug( 604 ) << k_funcinfo << endl;
    if (m_state == Phonon::LoadingState) {
        setState(Phonon::BufferingState);
    } else {
        setState(Phonon::PlayingState);
    }
}

void AbstractMediaProducer::pause()
{
    //kDebug(604) << k_funcinfo << endl;
    m_tickTimer->stop();
    setState(Phonon::PausedState);
}

void AbstractMediaProducer::stop()
{
	//kDebug( 604 ) << k_funcinfo << endl;
	m_tickTimer->stop();
	setState( Phonon::StoppedState );
	m_position = 0.0f;
	m_frequency = 440.0f;
}

void AbstractMediaProducer::seek( qint64 time )
{
	//kDebug( 604 ) << k_funcinfo << endl;
	if( isSeekable() )
	{
		switch( state() )
		{
			case Phonon::PausedState:
			case Phonon::BufferingState:
				m_startTime = m_pauseTime;
				break;
			case Phonon::PlayingState:
                m_startTime.start();
				break;
			case Phonon::StoppedState:
			case Phonon::ErrorState:
			case Phonon::LoadingState:
				return; // cannot seek
		}
		m_startTime = m_startTime.addMSecs( -time );
	}
}

QString AbstractMediaProducer::errorString() const
{
    return QString();
}

Phonon::ErrorType AbstractMediaProducer::errorType() const
{
    return Phonon::NoError;
}

void AbstractMediaProducer::setState( State newstate )
{
	if( newstate == m_state )
		return;
	State oldstate = m_state;
	m_state = newstate;
	switch( newstate )
	{
		case Phonon::PausedState:
		case Phonon::BufferingState:
			m_pauseTime.start();
			break;
		case Phonon::PlayingState:
            m_tickTimer->start();
			if( oldstate == Phonon::PausedState || oldstate == Phonon::BufferingState )
				m_startTime = m_startTime.addMSecs( m_pauseTime.elapsed() );
			else
				m_startTime.start();
			break;
		case Phonon::StoppedState:
		case Phonon::ErrorState:
		case Phonon::LoadingState:
            m_startTime = QTime();
			break;
	}
	//kDebug( 604 ) << "emit stateChanged( " << newstate << ", " << oldstate << " )" << endl;
	emit stateChanged( newstate, oldstate );
}

void AbstractMediaProducer::emitTick()
{
	//kDebug( 604 ) << "emit tick( " << currentTime() << " )" << endl;
	int tickInterval = 50;
	if( m_tickInterval > 0 )
	{
		emit tick( currentTime() );
		tickInterval = m_tickInterval;
	}
	QVector<float> buffer( m_bufferSize );
    Experimental::VideoFrame frame;
	frame.fourcc = 0x00000000;
	frame.width = 320;
	frame.height = 240;
	frame.depth = 24;
	frame.bpp = 8;
	frame.data.resize( frame.width * frame.height * 4 );

	const int availableSamples = tickInterval * SAMPLE_RATE / 1000 + m_lastSamplesMissing;
	const int bufferCount = availableSamples / m_bufferSize;
	m_lastSamplesMissing = availableSamples - bufferCount * m_bufferSize;
	for( int i = 0; i < bufferCount; ++i )
	{
		fillBuffer( &buffer );
		foreach( AudioPath* ap, m_audioPathList )
			ap->processBuffer( buffer );
		fillFrameData( &frame );
		foreach( VideoPath* vp, m_videoPathList )
			vp->processFrame( frame );
	}
}

static const float TWOPI = 6.28318530718f;
static const float maxFrequency = 1760.0f;
static const float minFrequency = 440.0f;
static const float frequencyToDelta = TWOPI / SAMPLE_RATE_FLOAT;

void AbstractMediaProducer::fillBuffer( QVector<float>* buffer )
{
	//static QFile createdump( "createdump" );
	//if( !createdump.isOpen() )
		//createdump.open( QIODevice::WriteOnly );

	m_frequency *= 1.059463094359f;
	if( m_frequency > maxFrequency )
		m_frequency = minFrequency;
	float delta = frequencyToDelta * m_frequency;

	float* data = buffer->data();
	const float * const end = data + m_bufferSize;

	while( data != end )
	{
		const float sample = sinf( m_position );
		//createdump.write( QByteArray::number( sample ) + "\n" );
		*( data++ ) = sample;
		m_position += delta;
		if( m_position > TWOPI )
			m_position -= TWOPI;
	}
}

void AbstractMediaProducer::fillFrameData(Phonon::Experimental::VideoFrame *frame)
{
	static quint32 frameCount = 0;
	quint8* dataPtr = reinterpret_cast<quint8*>( frame->data.data() );
	for( int y = 0; y < frame->height; ++y )
		for( int x = 0; x < frame->width; ++x )
		{
			*dataPtr++ = static_cast<quint8>( 0xff );
			*dataPtr++ = static_cast<quint8>( ( x + frameCount ) * 2 / 3 ); //red
			*dataPtr++ = static_cast<quint8>( y + frameCount ); //green
			*dataPtr++ = static_cast<quint8>( frameCount / 2 ); //blue
		}
	++frameCount;
}

}}
#include "abstractmediaproducer.moc"
// vim: sw=4 ts=4
