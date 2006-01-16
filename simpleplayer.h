/*  This file is part of the KDE project
    Copyright (C) 2004-2005 Matthias Kretz <kretz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/

#ifndef Kdem2m_SIMPLEPLAYER_H
#define Kdem2m_SIMPLEPLAYER_H

#include <QObject>
#include "state.h"
#include <kdelibs_export.h>

class KURL;

namespace Kdem2m
{

class KDEM2M_EXPORT SimplePlayer : public QObject
{
	Q_OBJECT
	public:
		SimplePlayer( QObject * parent = 0 );
		~SimplePlayer();

		void play( const KURL & url );
		void pause();
		void stop();

		long totalTime() const;
		long currentTime() const;
		void seek( long ms );

		float volume() const;
		void setVolume( float volume );

		bool isPlaying() const;
		bool isPaused() const;

	signals:
		void finished();

	private slots:
		void stateChanged( Kdem2m::State, Kdem2m::State );

	private:
		class Private;
		Private * d;
};

} //namespace Kdem2m

#endif // Kdem2m_SIMPLEPLAYER_H
// vim: sw=4 ts=4 noet tw=80
