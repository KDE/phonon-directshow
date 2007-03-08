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

#include <klocale.h>
#include <kiconloader.h>
#include <QAction>
#include <kdebug.h>

namespace Phonon
{

VideoWidget::VideoWidget( QWidget* parent )
	: QWidget( parent )
	, Phonon::AbstractVideoOutput( *new VideoWidgetPrivate( this ) )
{
	K_D( VideoWidget );
    d->init();
	d->createIface();
}

VideoWidget::VideoWidget(VideoWidgetPrivate &dd, QWidget *parent)
    : QWidget(parent),
    Phonon::AbstractVideoOutput(dd)
{
    K_D(VideoWidget);
    d->init();
}

void VideoWidgetPrivate::init()
{
    Q_Q(VideoWidget);
    fullScreenAction = new QAction(SmallIcon("view-fullscreen"), i18n("F&ull Screen Mode"), q);
    fullScreenAction->setShortcut(Qt::Key_F);
    fullScreenAction->setCheckable(true);
    fullScreenAction->setChecked(false);
    QObject::connect(fullScreenAction, SIGNAL(triggered(bool)), q, SLOT(setFullScreen(bool)));

    QObject::connect(&cursorTimer, SIGNAL(timeout()), q, SLOT(_k_cursorTimeout()));
    cursorTimer.start();
    q->setMouseTracking(true);
}

void VideoWidget::mouseMoveEvent(QMouseEvent *)
{
    K_D(VideoWidget);
    if (iface()) {
        QWidget *w = 0;
        BACKEND_GET(QWidget *, w, "widget");
        if (w && Qt::BlankCursor == w->cursor().shape()) {
            w->unsetCursor();
            d->cursorTimer.start();
        }
    }
}

void VideoWidgetPrivate::_k_cursorTimeout()
{
    if (backendObject) {
        QWidget *w = 0;
        pBACKEND_GET(QWidget *, w, "widget");
        if (w && Qt::ArrowCursor == w->cursor().shape()) {
            w->setCursor(Qt::BlankCursor);
        }
    }
}
void VideoWidgetPrivate::createIface()
{
	if( backendObject )
		return;
	K_Q( VideoWidget );
	backendObject = UiFactory::self()->createVideoWidget( q );
	if( backendObject )
		q->setupIface();
}

#define PHONON_CLASSNAME VideoWidget

PHONON_GETTER( Phonon::VideoWidget::AspectRatio, aspectRatio, d->aspectRatio )
PHONON_SETTER( setAspectRatio, aspectRatio, Phonon::VideoWidget::AspectRatio )

PHONON_GETTER(Phonon::VideoWidget::ScaleMode, scaleMode, d->scaleMode)
PHONON_SETTER(setScaleMode, scaleMode, Phonon::VideoWidget::ScaleMode)

PHONON_GETTER(Phonon::VideoWidget::OverlayTypes, overlayCapabilities, OverlayNone)

bool VideoWidget::createOverlay(QWidget *widget, VideoWidget::OverlayType type)
{
	K_D( VideoWidget );

	if( !d->backendObject )
		return false;

	bool result;

	if (BACKEND_GET2(bool, result, "createOverlay", QWidget *, widget, Phonon::VideoWidget::OverlayType, type))
		return result;

	return false;
}

void VideoWidget::setFullScreen( bool newFullScreen )
{
	kDebug( 602 ) << k_funcinfo << newFullScreen << endl;
	K_D( VideoWidget );
	// TODO: disable screensaver? or should we leave that responsibility to the
	// application?
	if( ! d->fullScreenWidget )
		d->fullScreenWidget = new FullScreenVideoWidget( this );
	QWidget *w = 0;
	if( iface() )
		BACKEND_GET( QWidget *, w, "widget" );
	if( newFullScreen )
	{
        d->fullScreenWidget->show();
		if( w )
			w->setParent( d->fullScreenWidget );
	}
	else
	{
		if( w )
		{
			w->setParent( this );
			layout()->addWidget( w );
		}
        d->fullScreenWidget->hide();
	}
	// make sure the action is in the right state
	d->fullScreenAction->setChecked( newFullScreen );
}

void VideoWidget::exitFullScreen()
{
	setFullScreen( false );
}

void VideoWidget::enterFullScreen()
{
	setFullScreen( true );
}

bool VideoWidgetPrivate::aboutToDeleteIface()
{
	return AbstractVideoOutputPrivate::aboutToDeleteIface();
}

void VideoWidget::setupIface()
{
	K_D( VideoWidget );
	Q_ASSERT( d->backendObject );
	AbstractVideoOutput::setupIface();
	kDebug( 602 ) << "calling setAspectRatio on the backend " << d->aspectRatio << endl;
	BACKEND_CALL1( "setAspectRatio", Phonon::VideoWidget::AspectRatio, d->aspectRatio );

	QWidget *w = 0;
	BACKEND_GET( QWidget *, w, "widget" );
	if( w )
	{
		w->addAction( d->fullScreenAction );
		d->layout.addWidget( w );
		setSizePolicy( w->sizePolicy() );
	}
}

/*
QSize VideoWidget::sizeHint()
{
	if( iface() )
	{
		QWidget *w = 0;
		BACKEND_GET( QWidget *, w, "widget" );
		if( w )
			return w->sizeHint();
	}
	return QSize( 0, 0 );
}

QSize VideoWidget::minimumSizeHint()
{
	if( iface() )
	{
		QWidget *w = 0;
		BACKEND_GET( QWidget *, w, "widget" );
		if( w )
			return w->minimumSizeHint();
	}
	return QSize( 0, 0 );
}*/

} //namespace Phonon

#include "videowidget.moc"
#include "videowidget_p.moc"

// vim: sw=4 ts=4 tw=80
