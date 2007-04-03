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

#ifndef PHONON_FRONTENDINTERFACEPRIVATE_H
#define PHONON_FRONTENDINTERFACEPRIVATE_H

#include "addoninterface.h"
#include "abstractmediaproducer_p.h"

namespace Phonon
{
class FrontendInterfacePrivate
{
    public:
        FrontendInterfacePrivate(AbstractMediaProducer *mp) : media(mp) {
            Q_ASSERT(media);
            AbstractMediaProducerPrivate *d = media->k_func();
            d->interfaceList << this;
        }
        virtual ~FrontendInterfacePrivate() {
            AbstractMediaProducerPrivate *d = media->k_func();
            d->interfaceList << this;
        }
        virtual void backendObjectChanged(QObject *iface) = 0;
        void _backendObjectChanged() {
            kDebug(600) << k_funcinfo << endl;
            QObject *x = media->iface();
            if (x) {
                backendObjectChanged(x);
            }
        }
        AddonInterface *iface() { return qobject_cast<AddonInterface *>(media->iface()); }

        AbstractMediaProducer *media;
};
} // namespace Phonon

#endif // PHONON_FRONTENDINTERFACEPRIVATE_H
