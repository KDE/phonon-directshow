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

#ifndef PHONON_UI_SEEKSLIDER_H
#define PHONON_UI_SEEKSLIDER_H

#include <QWidget>
#include <kdelibs_export.h>
#include "../state.h"

namespace Phonon
{
class AbstractMediaProducer;

namespace Ui
{
class SeekSliderPrivate;

/**
 * \short Widget providing a slider for seeking in AbstractMediaProducer objects.
 *
 * \author Matthias Kretz <kretz@kde.org>
 */
class PHONON_EXPORT SeekSlider : public QWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( SeekSlider )
	public:
		SeekSlider( QWidget* parent = 0 );
		~SeekSlider();

	public Q_SLOTS:
		void setMediaProducer( AbstractMediaProducer* );

	private Q_SLOTS:
		void stateChanged( Phonon::State );
		void mediaDestroyed();
		void seek( int );
		void tick( long );
		void length( long );

	protected:
		SeekSlider( SeekSliderPrivate& d, QWidget* parent );
		SeekSliderPrivate* d_ptr;
};

}} // namespace Phonon::Ui

// vim: sw=4 ts=4 tw=80
#endif // PHONON_UI_SEEKSLIDER_H
