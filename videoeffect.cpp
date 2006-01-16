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
#include "videoeffect.h"
#include "ifaces/videoeffect.h"
#include "factory.h"

namespace Kdem2m
{
class VideoEffect::Private
{
	public:
		Private()
		{ }

		QString type;
};

KDEM2M_OBJECT_IMPL( VideoEffect )

QString VideoEffect::type() const
{
	return m_iface ? m_iface->type() : d->type;
}

void VideoEffect::setType( const QString& type )
{
	if( m_iface )
		m_iface->setType( type );
	else
		d->type = type;
}

bool VideoEffect::aboutToDeleteIface()
{
	if( m_iface )
	{
		d->type = m_iface->type();
	}
	return true;
}

void VideoEffect::setupIface()
{
	if( !m_iface )
		return;

	// set up attributes
	m_iface->setType( d->type );
}

} //namespace Kdem2m

#include "videoeffect.moc"

// vim: sw=4 ts=4 tw=80 noet
