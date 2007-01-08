/*  This file is part of the KDE project
    Copyright (C) 2006 Tim Beaulen <tbscope@gmail.com>

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

#ifndef Phonon_XINE_BACKEND_H
#define Phonon_XINE_BACKEND_H

#include <QList>
#include <QPointer>
#include <QStringList>

#include <xine.h>
#include <xine/xineutils.h>

#include "xineengine.h"
#include <QObject>
#include <phonon/objectdescription.h>
#include <phonon/backendinterface.h>

class KUrl;

namespace Phonon
{
namespace Xine
{
	class AudioOutput;

    class Backend : public QObject, public BackendInterface
	{
		Q_OBJECT
        Q_INTERFACES(Phonon::BackendInterface)
		public:
			Backend( QObject* parent, const QStringList& args );
			~Backend();

            QObject* createObject0(BackendInterface::Class0, QObject *parent);
            QObject* createObject1(BackendInterface::Class1, QObject *parent, QVariant arg1);

			Q_INVOKABLE bool supportsVideo() const;
			Q_INVOKABLE bool supportsOSD() const;
			Q_INVOKABLE bool supportsFourcc( quint32 fourcc ) const;
			Q_INVOKABLE bool supportsSubtitles() const;

			Q_INVOKABLE void freeSoundcardDevices();

            QSet<int> objectDescriptionIndexes(ObjectDescriptionType) const;
            QHash<QByteArray, QVariant> objectDescriptionProperties(ObjectDescriptionType, int) const;

		public slots:
            QStringList knownMimeTypes() const;
			const char* uiLibrary() const;
			//const char* uiSymbol() const;

		private:
            mutable QStringList m_supportedMimeTypes;
	};
}} // namespace Phonon::Xine

// vim: sw=4 ts=4 tw=80
#endif // Phonon_XINE_BACKEND_H
