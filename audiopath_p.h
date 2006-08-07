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

#ifndef AUDIOPATH_P_H
#define AUDIOPATH_P_H

#include "audiopath.h"
#include "base_p.h"
#include <QList>

namespace Phonon
{
class AudioPathPrivate : public BasePrivate, private BaseDestructionHandler
{
	K_DECLARE_PUBLIC( AudioPath )
	PHONON_PRIVATECLASS( Base )
	protected:
		AudioPathPrivate()
		{
		}

		QList<AbstractAudioOutput*> outputs;
		QList<AudioEffect*> effects;

	private:
		virtual void phononObjectDestroyed( Base* );
};
}

#endif // AUDIOPATH_P_H
// vim: sw=4 ts=4 tw=80
