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
#ifndef Phonon_MEDIAOBJECT_H
#define Phonon_MEDIAOBJECT_H

#include "abstractmediaproducer.h"
#include <kmimetype.h>
#include <kdelibs_export.h>
#include "phonondefs.h"

class KUrl;

namespace Phonon
{
	class MediaObjectPrivate;
	namespace Ifaces
	{
		class MediaObject;
	}

	/**
	 * \short Interface for media playback of a given URL.
	 *
	 * This class is the most important class for most mediaplayback tasks. With
	 * this class you open a URL, play, pause, stop, seek and get a lot of
	 * information about the media data.
	 *
	 * A common usage example is the following:
	 * \code
	 * media = new MediaObject( this );
	 * connect( media, SIGNAL( finished() ), SLOT( slotFinished() );
	 * media->setUrl( "/home/username/music/filename.ogg" );
	 * media->play();
	 * \endcode
	 *
	 * \author Matthias Kretz <kretz@kde.org>
	 */
	class PHONON_EXPORT MediaObject : public AbstractMediaProducer
	{
		Q_OBJECT
		K_DECLARE_PRIVATE( MediaObject )
		PHONON_HEIR( MediaObject )
		public:
			/**
			 * Get the URL the MediaObject should use as media data source.
			 *
			 * \return The currently used URL to the media data
			 */
			KUrl url() const;
			/**
			 * Get the total time (in milliseconds) of the file currently being played.
			 *
			 * \see length
			 */
			long totalTime() const;
			/**
			 * Get the remaining time (in milliseconds) of the file currently being played.
			 */
			long remainingTime() const;

			/**
			 * Returns the time in milliseconds the aboutToFinish signal is
			 * emitted before the playback finishes. Defaults to 0.
			 *
			 * \return The aboutToFinish time in milliseconds.
			 *
			 * \see setAboutToFinishTime
			 * \see aboutToFinish
			 */
			long aboutToFinishTime() const;

		public Q_SLOTS:
			// Attributes Setters:
			/**
			 * Set the URL the MediaObject should use as media data source.
			 *
			 * \param url The URL to the media data.
			 */
			void setUrl( const KUrl& url );

			/**
			 * Sets the time in milliseconds the aboutToFinish signal is emitted
			 * before the playback finishes. Defaults to 0.
			 *
			 * \param newAboutToFinishTime The new aboutToFinishTime in
			 * milliseconds
			 *
			 * \see aboutToFinishTime
			 * \see aboutToFinish
			 */
			void setAboutToFinishTime( long newAboutToFinishTime );

		Q_SIGNALS:
			/**
			 * Emitted when the file has finished playing on its own.
			 * I.e. it is not emitted if you call stop(), pause() or
			 * load(), but only on end-of-file or a critical error.
			 *
			 * \see aboutToFinish
			 */
			void finished();

			/**
			 * Emitted when there are only aboutToFinishTime milliseconds left
			 * for playback.
			 *
			 * \param msec The remaining time until the playback finishes.
			 *
			 * \see setAboutToFinishTime
			 * \see aboutToFinishTime
			 * \see finished
			 */
			void aboutToFinish( long msec );

			/**
			 * This signal is emitted as soon as the length of the media file is
			 * known or has changed. For most non-local media data the length of
			 * the media can only be known after some time. At that time the
			 * totalTime function can not return usefull information. You have
			 * to wait for this signal to know the real length.
			 *
			 * \param length The length of the media file in milliseconds.
			 *
			 * \see totalTime
			 */
			void length( long length );
	};
} //namespace Phonon

// vim: sw=4 ts=4 tw=80 noet
#endif // Phonon_MEDIAOBJECT_H
