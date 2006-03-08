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
#include "ifaces/mediaobject.h"
#include "factory.h"

#include <kdebug.h>

namespace Phonon
{
PHONON_HEIR_IMPL( MediaObject, AbstractMediaProducer )

KUrl MediaObject::url() const
{
	K_D( const MediaObject );
	return d->iface() ? d->iface()->url() : d->url;
}

long MediaObject::totalTime() const
{
	K_D( const MediaObject );
	return d->iface() ? d->iface()->totalTime() : -1;
}

long MediaObject::remainingTime() const
{
	K_D( const MediaObject );
	return d->iface() ? d->iface()->remainingTime() : -1;
}

long MediaObject::aboutToFinishTime() const
{
	K_D( const MediaObject );
	return d->iface() ? d->iface()->aboutToFinishTime() : d->aboutToFinishTime;
}

void MediaObject::setUrl( const KUrl& url )
{
	K_D( MediaObject );
	if( iface() )
	{
		d->iface()->setUrl( url );
		if( state() == Phonon::ErrorState )
		{
			d->deleteIface();
			//TODO: at this point MediaObject should try to use an Ifaces::ByteStream
			//instead and send the data it receives from a KIO Job via writeBuffer.
			//This essentially makes all media frameworks read data via KIO...
		}
	}
	else
		d->url = url;
}

void MediaObject::setAboutToFinishTime( long newAboutToFinishTime )
{
	K_D( MediaObject );
	//kdDebug( 600 ) << k_funcinfo << endl;
	if( d->iface() )
		d->iface()->setAboutToFinishTime( newAboutToFinishTime );
	else
		d->aboutToFinishTime = newAboutToFinishTime;
}

bool MediaObjectPrivate::aboutToDeleteIface()
{
	//kdDebug( 600 ) << k_funcinfo << endl;
	if( iface() )
		aboutToFinishTime = iface()->aboutToFinishTime();
	return AbstractMediaProducerPrivate::aboutToDeleteIface();
}

void MediaObject::setupIface()
{
	K_D( MediaObject );
	Q_ASSERT( d->iface() );
	//kdDebug( 600 ) << k_funcinfo << endl;
	AbstractMediaProducer::setupIface();

	connect( d->iface()->qobject(), SIGNAL( finished() ), SIGNAL( finished() ) );
	connect( d->iface()->qobject(), SIGNAL( aboutToFinish( long ) ), SIGNAL( aboutToFinish( long ) ) );
	connect( d->iface()->qobject(), SIGNAL( length( long ) ), SIGNAL( length( long ) ) );

	// set up attributes
	if( !d->url.isEmpty() )
		d->iface()->setUrl( d->url );
	if( state() == Phonon::ErrorState )
	{
		d->deleteIface();
		//TODO: at this point MediaObject should try to use an Ifaces::ByteStream
		//instead and send the data it receives from a KIO Job via writeBuffer.
		//This essentially makes all media frameworks read data via KIO...
		return;
	}
	d->iface()->setAboutToFinishTime( d->aboutToFinishTime );
}

} //namespace Phonon

#include "mediaobject.moc"

// vim: sw=4 ts=4 tw=80 noet
