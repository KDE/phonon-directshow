/*  This file is part of the KDE project
    Copyright (C) 2005-2006 Matthias Kretz <kretz@kde.org>

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

#include "videowidget.h"
#include "videowidget_p.h"
#include "factory.h"

#include <phonon/ifaces/ui/videowidget.h>

namespace Phonon
{
namespace Ui
{

VideoWidget::VideoWidget( QWidget* parent )
	: QWidget( parent )
	, Phonon::AbstractVideoOutput( *new VideoWidgetPrivate )
{
	K_D( VideoWidget );
	d->createIface();
}

VideoWidget::VideoWidget( VideoWidgetPrivate& d, QWidget* parent )
	: QWidget( parent )
	, Phonon::AbstractVideoOutput( d )
{
}

Ifaces::VideoWidget* VideoWidget::iface()
{
	K_D( VideoWidget );
	if( !d->iface() )
		d->createIface();
	return d->iface();
}

void VideoWidgetPrivate::createIface()
{
	K_Q( VideoWidget );
	Q_ASSERT( iface_ptr == 0 );
	setIface( Factory::self()->createVideoWidget( q ) );
	q->setupIface();
}

bool VideoWidget::isFullscreen() const
{
	K_D( const VideoWidget );
	return d->iface() ? d->iface()->isFullscreen() : d->fullscreen;
}

void VideoWidget::setFullscreen( bool newFullscreen )
{
	K_D( VideoWidget );
	if( iface() )
		d->iface()->setFullscreen( newFullscreen );
	else
		d->fullscreen = newFullscreen;
}

bool VideoWidgetPrivate::aboutToDeleteIface()
{
	if( iface() )
		fullscreen = iface()->isFullscreen();
	return true;
}

void VideoWidget::setupIface()
{
	K_D( VideoWidget );
	Q_ASSERT( d->iface() );

	d->iface()->setFullscreen( d->fullscreen );
}

}} //namespace Phonon::Ui

#include "videowidget.moc"

// vim: sw=4 ts=4 tw=80 noet
