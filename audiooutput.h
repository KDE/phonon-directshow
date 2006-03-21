/*  This file is part of the KDE project
    Copyright (C) 2005-2006 Matthias Kretz <kretz@kde.org>

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
#ifndef Phonon_AUDIOOUTPUT_H
#define Phonon_AUDIOOUTPUT_H

#include "abstractaudiooutput.h"
#include "mixeriface.h"
#include <kdelibs_export.h>
#include "phonondefs.h"
#include "phononnamespace.h"

class QString;

namespace Phonon
{
	class AudioOutputPrivate;
	namespace Ifaces
	{
		class AudioOutput;
	}

	/**
	 * \short Class for audio output to the soundcard.
	 *
	 * Use this class to define the audio output. Example:
	 * \code
	 * m_audioOutput = new AudioOutput( this );
	 * m_audioOutput->setCategory( Phonon::MusicCategory );
	 * m_audioPath->addOutput( m_audioOutput );
	 * \endcode
	 *
	 * \author Matthias Kretz <kretz@kde.org>
	 * \see Phonon::Ui::VolumeSlider
	 */
	class PHONON_EXPORT AudioOutput : public AbstractAudioOutput, public MixerIface
	{
		Q_OBJECT
		K_DECLARE_PRIVATE( AudioOutput )
		/**
		 * This is the name that appears in Mixer applications that control
		 * the volume of this output.
		 *
		 * \see category
		 */
		Q_PROPERTY( QString name READ name WRITE setName )
		/**
		 * This is the current volume of the output. 1.0 means 100%, 0.0
		 * means 0%
		 */
		Q_PROPERTY( float volume READ volume WRITE setVolume )
		/**
		 * The category can be used by mixer applications to control the
		 * volume of a whole category instead of the user having to identify
		 * all programs by name.
		 *
		 * The category is also used for the default output device that is
		 * configured centrally. As an example: often users want to have the
		 * audio signal of a VoIP application go to their USB headset while
		 * all other sounds should go to the internal soundcard. The
		 * category defaults to Phonon::UnspecifiedCategory.
		 *
		 * \see Phonon::categoryToString
		 */
		Q_PROPERTY( Category category READ category WRITE setCategory )
		PHONON_HEIR( AudioOutput )
		public:
			QString name() const;
			float volume() const;
			Phonon::Category category() const;

		private:
			QString categoryName() const;

		public Q_SLOTS:
			void setName( const QString& newName );
			void setVolume( float newVolume );
			void setCategory( Phonon::Category category );

		Q_SIGNALS:
			/**
			 * This signal is emitted whenever the volume has changed. As the
			 * volume can change without a call to setVolume (calls over dbus)
			 * this is important
			 * to keep a widget showing the current volume up to date.
			 */
			void volumeChanged( float newVolume );
	};
} //namespace Phonon

// vim: sw=4 ts=4 tw=80 noet
#endif // Phonon_AUDIOOUTPUT_H
