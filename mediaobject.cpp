/*  This file is part of the KDE project
    Copyright (C) 2005 Matthias Kretz <kretz@kde.org>

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
#include "mediaobject.h"
#include "mediaobject_p.h"
#include "factory.h"
#include "mediaobjectinterface.h"
#include "kiofallback.h"

#include <kdebug.h>
#include <QCoreApplication>

#define PHONON_CLASSNAME MediaObject
#define PHONON_INTERFACENAME MediaObjectInterface

namespace Phonon
{
PHONON_HEIR_IMPL( AbstractMediaProducer )

MediaObject::MediaObject( Phonon::MediaObjectPrivate& dd, QObject* parent )
	: AbstractMediaProducer( dd, parent )
{
}

KUrl MediaObject::url() const
{
    K_D(const MediaObject);
    MediaObjectInterface *iface = qobject_cast<MediaObjectInterface *>(d->backendObject);
    if (iface) {
        return iface->url();
    }
    return d->url;
}

PHONON_GETTER( qint32, aboutToFinishTime, d->aboutToFinishTime )

qint64 MediaObject::totalTime() const
{
	K_D( const MediaObject );
	if( !d->backendObject )
		return -1;
    if (d->kiofallback) {
        return d->kiofallback->totalTime();
    }
    return INTERFACE_CALL(totalTime());
}

qint64 MediaObject::remainingTime() const
{
	K_D( const MediaObject );
	if( !d->backendObject )
		return -1;
	qint64 ret;
	if( !BACKEND_GET( qint64, ret, "remainingTime" ) )
		ret = totalTime() - currentTime();
	if( ret < 0 )
		return -1;
	return ret;
}

void MediaObject::setUrl( const KUrl& url )
{
	K_D( MediaObject );
	d->url = url;
	if( iface() )
	{
		stop(); // first call stop as that often is the expected state
		                    // for setting a new URL
		MediaObjectInterface *iface = qobject_cast<MediaObjectInterface*>( d->backendObject );
		if( iface )
		{
			iface->setUrl( url );
            // if the state changes to ErrorState it will be handled in
            // _k_stateChanged and a ByteStream will be used.
			return;
		}

		// we're using a ByteStream
		// first try to do with a proper MediaObject
		d->deleteIface();
		d->createIface();
		// createIface will set up a ByteStream (in setupIface) if needed
	}
}

void MediaObject::stop()
{
	Phonon::State prevState = state();
	if( prevState == Phonon::StoppedState || prevState == Phonon::LoadingState )
		return;
	AbstractMediaProducer::stop();

    K_D(MediaObject);
    if (d->kiofallback) {
        d->kiofallback->stopped();
    }
}

void MediaObject::play()
{
	AbstractMediaProducer::play();
}

PHONON_SETTER( setAboutToFinishTime, aboutToFinishTime, qint32 )

void MediaObjectPrivate::setupKioStreaming()
{
    if (!kiofallback) {
        Q_Q(MediaObject);
        kiofallback = new KioFallback(q); // TODO get from Factory
        if (!kiofallback) {
            return;
        }
    }
    kiofallback->setupKioStreaming();
}

bool MediaObjectPrivate::aboutToDeleteIface()
{
	//kDebug( 600 ) << k_funcinfo << endl;
	pBACKEND_GET( qint32, aboutToFinishTime, "aboutToFinishTime" );
    if (AbstractMediaProducerPrivate::aboutToDeleteIface()) {
        // the next backend object we'll handle is a MediaObject, so delete the
        // KioFallback object
        delete kiofallback;
        kiofallback = 0;
        return true;
    }
    return false;
}

void MediaObjectPrivate::_k_stateChanged( Phonon::State newstate, Phonon::State oldstate )
{
    K_Q(MediaObject);

    // backend MediaObject reached ErrorState, try a ByteStream
    if (newstate == Phonon::ErrorState && !kiofallback) {
        kDebug(600) << "backend MediaObject reached ErrorState, trying ByteStream now" << endl;
        deleteIface();
        if (oldstate != Phonon::LoadingState) {
            emit q->stateChanged(Phonon::LoadingState, oldstate);
        }
        setupKioStreaming();
		return;
	}
	emit q->stateChanged( newstate, oldstate );
}

// setupIface is not called for ByteStream
void MediaObject::setupIface()
{
	K_D( MediaObject );
	Q_ASSERT( d->backendObject );
	//kDebug( 600 ) << k_funcinfo << endl;
	AbstractMediaProducer::setupIface();

	// disconnect what AbstractMediaProducer::setupIface connected to filter out
	// the LoadingState -> ErrorState change on setUrl
	disconnect( d->backendObject, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ), this, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ) );
	connect( d->backendObject, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ), SLOT( _k_stateChanged( Phonon::State, Phonon::State ) ) );

	connect( d->backendObject, SIGNAL( finished() ), SIGNAL( finished() ) );
	connect( d->backendObject, SIGNAL( aboutToFinish( qint32 ) ), SIGNAL( aboutToFinish( qint32 ) ) );
	connect( d->backendObject, SIGNAL( length( qint64 ) ), SIGNAL( length( qint64 ) ) );

	// set up attributes
    BACKEND_CALL1("setAboutToFinishTime", qint32, d->aboutToFinishTime);
    if(!d->url.isEmpty() && !d->kiofallback) {
        INTERFACE_CALL(setUrl(d->url));
        // if the state changes to ErrorState it will be handled in
        // _k_stateChanged and a ByteStream will be used.
    }
}

} //namespace Phonon

#include "mediaobject.moc"

// vim: sw=4 ts=4 tw=80
