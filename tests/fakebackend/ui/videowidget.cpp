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

#include "videowidget.h"
#include <QPalette>

namespace Phonon
{
namespace Ui
{
namespace Fake
{

VideoWidget::VideoWidget( QWidget* parent )
	: QWidget( parent )
{
	QPalette p = palette();
	p.setColor( QPalette::Window, Qt::blue );
	setPalette( p );
	setBackgroundRole( QPalette::Window );
	setMinimumSize( 100, 100 );
}

bool VideoWidget::isFullscreen() const
{
	return QWidget::isFullScreen();
}

void VideoWidget::setFullscreen( bool fullscreen )
{
	if( fullscreen )
		setWindowState( windowState() | Qt::WindowFullScreen );
	else
		setWindowState( windowState() & ~Qt::WindowFullScreen );
}

}}} //namespace Phonon::Ui::Fake

#include "videowidget.moc"
// vim: sw=4 ts=4 noet
