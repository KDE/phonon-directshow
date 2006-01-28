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

#ifndef STATETEST_H
#define STATETEST_H

#include <QObject>

#include <phonon/state.h>

class KUrl;

namespace Phonon
{
	class MediaObject;
}

class StateTester : public QObject
{
	Q_OBJECT

	public Q_SLOTS:
		void run();

	private Q_SLOTS:
		void initTestCase();
		void stateChanged( Phonon::State, Phonon::State );
		void cleanupTestCase();

	private:
		void testplaying();
		void wrongStateChange();

		Phonon::MediaObject* player;
		KUrl* m_url;
};

#endif // STATETEST_H
// vim: sw=4 ts=4 noet
