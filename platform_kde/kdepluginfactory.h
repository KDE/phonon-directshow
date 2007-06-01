/*  This file is part of the KDE project
    Copyright (C) 2007 Matthias Kretz <kretz@kde.org>

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

#ifndef PHONON_KDEPLUGINFACTORY_H
#define PHONON_KDEPLUGINFACTORY_H

#include "../pluginfactory.h"
#include <QtCore/QObject>
#include <kservice.h>

namespace Phonon
{

class KdePluginFactory : public QObject, public PluginFactory
{
    Q_INTERFACES(Phonon::PluginFactory)
    Q_OBJECT
    public:
        KdePluginFactory();

        AbstractMediaStream *createKioMediaStream(const QUrl &url, QObject *parent);

        QIcon icon(const QString &name);
        void notification(const char *notificationName, const QString &text,
                const QStringList &actions, QObject *receiver,
                const char *actionSlot);
        QString applicationName() const;
        QObject *createBackend();
        QObject *createBackend(const QString &library, const QString &version);
        bool isMimeTypeAvailable(const QString &mimeType);

    private:
        QObject *createBackend(KService::Ptr newService);
};

} // namespace Phonon

#endif // PHONON_KDEPLUGINFACTORY_H
