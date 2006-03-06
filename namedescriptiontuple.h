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

#ifndef PHONON_NAMEDESCRIPTIONTUPLE_H
#define PHONON_NAMEDESCRIPTIONTUPLE_H

#include <QtGlobal>
class QString;

namespace Phonon
{
	class NameDescriptionTuplePrivate;

/**
 * \short Provides a tuple of enduser visible name and description.
 *
 * Some parts give the enduser choices, e.g. what source to capture audio from.
 * These choices are described by the name and description methods of this class
 * and identified with the id method. Subclasses then define additional
 * information like which audio and video choices belong together.
 *
 * \author Matthias Kretz <kretz@kde.org>
 * \see AudioCaptureSource
 * \see VideoCaptureSource
 */
class NameDescriptionTuple
{
	Q_DECLARE_PRIVATE( NameDescriptionTuple )
	public:
		/** \internal
		 * Frees the memory used
		 */
		~NameDescriptionTuple();

		/**
		 * Returns the name of the capture source.
		 *
		 * \return A string that should be presented to the user to
		 * choose the capture source.
		 */
		const QString& name() const;

		/**
		 * Returns a description of the capture source. This text should
		 * make clear what sound source this is, which is sometimes hard
		 * to describe or understand from just the name.
		 *
		 * \return A string describing the capture source.
		 */
		const QString& description() const;

		/**
		 * A unique identifier for this capture source. Used internally
		 * to distinguish between the capture sources.
		 *
		 * \return An integer that uniquely identifies every capture
		 * source.
		 */
		int index() const;

	protected:
		/**
		 * \internal
		 * Sets the data.
		 */
		NameDescriptionTuple( NameDescriptionTuplePrivate &dd, int index,
				const QString& name, const QString& description );
		/**
		 * \internal
		 * for copy ctor
		 */
		NameDescriptionTuple( NameDescriptionTuplePrivate &dd );

		NameDescriptionTuplePrivate* d_ptr;

	private:
		Q_DISABLE_COPY( NameDescriptionTuple )
};
} //namespace Phonon

#endif // PHONON_NAMEDESCRIPTIONTUPLE_H
// vim: sw=4 ts=4 noet tw=80
