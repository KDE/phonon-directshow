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

#include "xinethread.h"
#include <QtCore/QTimer>
#include <QtCore/QCoreApplication>
#include "xineengine.h"
#include "xinestream.h"
#include "audiopostlist.h"
#include "events.h"

namespace Phonon
{
namespace Xine
{

XineThread::XineThread()
    : m_newStream(0),
    m_eventLoopReady(false)
{
}

XineThread::~XineThread()
{
}

// called from main thread
// should never be called from ByteStream
void XineThread::waitForEventLoop()
{
    m_mutex.lock();
    if (!m_eventLoopReady) {
        m_waitingForEventLoop.wait(&m_mutex);
    }
    m_mutex.unlock();
}

XineStream *XineThread::newStream()
{
    XineThread *that = XineEngine::thread();
    Q_ASSERT(that->m_newStream == 0);
    QCoreApplication::postEvent(that, new QEVENT(NewStream));
    if (!that->m_newStream) {
        that->m_mutex.lock();
        if (!that->m_newStream) {
            that->m_waitingForNewStream.wait(&that->m_mutex);
        }
        that->m_mutex.unlock();
    }
    Q_ASSERT(that->m_newStream);
    XineStream *ret = that->m_newStream;
    that->m_newStream = 0;
    return ret;
}

void XineThread::quit()
{
    foreach (QObject *child, children()) {
        kDebug(610) << child;
    }
    QThread::quit();
}

bool XineThread::event(QEvent *e)
{
    switch (e->type()) {
    case Event::NewStream:
        e->accept();
        m_mutex.lock();
        Q_ASSERT(m_newStream == 0);
        m_newStream = new XineStream;
        m_newStream->moveToThread(this);
        m_mutex.unlock();
        m_waitingForNewStream.wakeAll();
        return true;
    case Event::Rewire:
        e->accept();
        kDebug(610) << "XineThread Rewire event:";
        {
            RewireEvent *ev = static_cast<RewireEvent *>(e);
            foreach (WireCall wire, ev->wireCalls) {
                kDebug(610) << "     " << wire.source << " -> " << wire.sink;
                wire.sink->assert();
                wire.source->assert();
                wire.sink->rewireTo(wire.source);
            }
        }
        return true;
    default:
        return QThread::event(e);
    }
}

void XineThread::run()
{
    Q_ASSERT(QThread::currentThread() == this);
    QTimer::singleShot(0, this, SLOT(eventLoopReady()));
    exec();
    m_eventLoopReady = false;
    // there should be no remaining XineStreams
    const QList<QObject *> c = children();
    foreach (QObject *obj, c) {
        XineStream *xs = qobject_cast<XineStream *>(obj);
        if (xs) {
            delete xs;
        }
    }
}

// xine thread
void XineThread::eventLoopReady()
{
    m_mutex.lock();
    m_eventLoopReady = true;
    m_mutex.unlock();
    m_waitingForEventLoop.wakeAll();
}

} // namespace Xine
} // namespace Phonon

#include "xinethread.moc"
// vim: sw=4 sts=4 et tw=100
