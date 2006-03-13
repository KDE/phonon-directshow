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

#ifndef SEEKSLIDER_H
#define SEEKSLIDER_H

#include <QObject>

namespace Phonon {
	class MediaObject;
	namespace Ui {
		class SeekSlider;
	}
}
class QSlider;
class QLabel;

class SeekSliderTest : public QObject
{
	Q_OBJECT
	private Q_SLOTS:
		void initTestCase();
		void testEnabled();
		void setMedia();
		void playMedia();
		void seekWithSlider();
		void cleanupTestCase();
	private:
		Phonon::Ui::SeekSlider* ss;
		QSlider* qslider;
		QLabel* qlabel;
		Phonon::MediaObject* media;
};

#endif // SEEKSLIDER_H
