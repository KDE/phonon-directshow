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

#include <qtest_kde.h>

#include "seekslider.h"
#include "../seekslider.h"
#include "../../mediaobject.h"
#include <QSlider>

using namespace Phonon;
using namespace Phonon::Ui;

void SeekSliderTest::initTestCase()
{
	ss = new SeekSlider;
	QVERIFY( ss != 0 );
	qslider = ss->findChild<QSlider*>();
	QVERIFY( qslider != 0 );
}

void SeekSliderTest::testEnabled()
{
	QVERIFY( !qslider->isEnabled() );
	ss->setMediaProducer( 0 );
	QVERIFY( !qslider->isEnabled() );
}

void SeekSliderTest::testErrorMedia()
{
	MediaObject media( "/dev/null", this );
	QVERIFY( media.state() == Phonon::ErrorState );
	ss->setMediaProducer( &media );
	QVERIFY( !qslider->isEnabled() );
}

void SeekSliderTest::cleanupTestCase()
{
	qslider = 0;
	delete ss;
}

QTEST_KDEMAIN( SeekSliderTest, GUI )
#include "seekslider.moc"
