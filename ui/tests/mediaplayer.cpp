/*  This file is part of the KDE project
    Copyright (C) 2006 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.

*/

#include "mediaplayer.h"
#include <QVBoxLayout>

using namespace Phonon;
using namespace Phonon::Ui;

MediaPlayer::MediaPlayer( QWidget* parent )
	: QWidget( parent )
{
	QVBoxLayout* layout = new QVBoxLayout( this );

	m_vwidget = new VideoWidget( this );
	layout->addWidget( m_vwidget );
	m_vwidget->hide();

	m_vpath = new VideoPath( this );

	m_aoutput = new AudioOutput( this );
	m_apath = new AudioPath( this );

	m_media = new MediaObject( this );
	m_media->addVideoPath( m_vpath );
	m_vpath->addOutput( m_vwidget );
	m_media->addAudioPath( m_apath );
	m_apath->addOutput( m_aoutput );

	m_controls = new MediaControls( this );
	layout->addWidget( m_controls );
	m_controls->setMediaProducer( m_media );
	m_controls->setAudioOutput( m_aoutput );
}

void MediaPlayer::setUrl( const KUrl& url )
{
	m_media->setUrl( url );
	m_vwidget->setVisible( m_media->hasVideo() );
}

#include <kcmdlineargs.h>
#include <kapplication.h>
#include <kaboutdata.h>
#include <cstdlib>
#include <QVBoxLayout>

int main( int argc, char ** argv )
{
	KAboutData about( "phononmediaplayer", "Phonon Media Player",
			"0.1", "Media Player",
			KAboutData::License_GPL, 0 );
	about.addAuthor( "Matthias Kretz", 0, "kretz@kde.org" );
	KCmdLineArgs::init( argc, argv, &about );
	KApplication app;
	MediaPlayer foo;
	foo.show();
	KUrl url = getenv( "PHONON_TESTURL" );
	if( url.isValid() )
	{
		foo.setUrl( url );
		return app.exec();
	}
	return 1;
}

// vim: sw=4 ts=4 noet
#include "mediaplayer.moc"
