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
#ifndef PHONON_STATE_H
#define PHONON_STATE_H

#include <kdelibs_export.h>

namespace Phonon
{
	/**
	 * The state the media producing object is in at the moment.
	 *
	 * @since 4.0
	 * @see AbstractMediaProducer
	 */
	enum State
	{
		/**
		 * After construction it might take a while before the Player is
		 * ready to play(). Normally this doesn't happen for local
		 * files, but can happen for remote files where the asynchronous
		 * mimetype detection and prebuffering can take a while.
		 */
		LoadingState,
		/**
		 * The Player has a valid media file loaded and is ready for
		 * playing.
		 */
		StoppedState,
		/**
		 * The Player is playing a media file.
		 */
		PlayingState,
		/**
		 * The Player is waiting for data to be able to continue
		 * playing.
		 */
		BufferingState,
		/**
		 * The Player is currently paused.
		 */
		PausedState,
		/**
		 * An unrecoverable error occured. The Object is unusable in this state.
		 */
		ErrorState
	};

	/**
	 * Set's the category your program should be listed in in the mixer.
	 *
	 * A Jukebox will set this to Music, a VoIP program to Communication, a
	 * DVD player to video, and so on.
	 *
	 * \see AudioOutput::setCategory
	 */
	enum Category
	{
		Unspecified,
		Notification,
		Music,
		Video,
		Communication
	};

	QString categoryToString( Category c );
} //namespace Phonon

class kdbgstream;
#include <kdebug.h>
/**
 * Implements needed operator to use Phonon::State with kdDebug
 */
inline PHONON_EXPORT kdbgstream& operator<<( kdbgstream & stream, const Phonon::State state )
{
	switch( state )
	{
		case Phonon::ErrorState:
			stream << "Error";
			break;
		case Phonon::LoadingState:
			stream << "LoadingState";
			break;
		case Phonon::StoppedState:
			stream << "StoppedState";
			break;
		case Phonon::PlayingState:
			stream << "PlayingState";
			break;
		case Phonon::BufferingState:
			stream << "BufferingState";
			break;
		case Phonon::PausedState:
			stream << "PausedState";
			break;
	}
	return stream;
}

// vim: sw=4 ts=4 tw=80 noet
#endif // PHONON_STATE_H
