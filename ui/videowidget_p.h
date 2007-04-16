/*  This file is part of the KDE project
    Copyright (C) 2006-2007 Matthias Kretz <kretz@kde.org>

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

#ifndef VIDEOWIDGET_P_H
#define VIDEOWIDGET_P_H

#include "videowidget.h"
#include "../abstractvideooutput_p.h"
#include <QHBoxLayout>
#include <QAction>
#include <QChildEvent>
#include <QCoreApplication>
#include <QPalette>
#include <QCloseEvent>
#include <QTimer>

namespace Phonon
{
class FullScreenVideoWidget : public QWidget
{
    Q_OBJECT
    public:
        FullScreenVideoWidget(QWidget *parent);
    protected:
        void childEvent(QChildEvent *e);
        void closeEvent(QCloseEvent *e);
        void mouseMoveEvent(QMouseEvent *);
        bool event(QEvent *e);

    private Q_SLOTS:
        void cursorTimeout();
    private:
        QWidget *child;
        QTimer cursorTimer;
};

void FullScreenVideoWidget::mouseMoveEvent(QMouseEvent *)
{
    if (child && Qt::BlankCursor == child->cursor().shape()) {
        child->unsetCursor();
        cursorTimer.start();
    }
}

void FullScreenVideoWidget::cursorTimeout()
{
    if (child && Qt::ArrowCursor == child->cursor().shape()) {
        child->setCursor(Qt::BlankCursor);
    }
}

FullScreenVideoWidget::FullScreenVideoWidget(QWidget *parent)
    : QWidget(parent, Qt::Window),
    child(0)
{
    setAttribute(Qt::WA_QuitOnClose, false); // if the other windows are closed but the fullscreen
    // video widget is still open quit the application

    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
    setCursor(QCursor(Qt::BlankCursor));
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setLayout(new QHBoxLayout);
    layout()->setMargin(0);
    hide();

    cursorTimer.setInterval(1000); // 1s timeout until the cursor disappears
    cursorTimer.setSingleShot(true);
    connect(&cursorTimer, SIGNAL(timeout()), SLOT(cursorTimeout()));
    setMouseTracking(true);
}

void FullScreenVideoWidget::childEvent(QChildEvent *e)
{
    if (!(e->child() && e->child()->isWidgetType()))
        return;

    child = qobject_cast<QWidget *>(e->child());
    Q_ASSERT(child);
    if (e->added())
    {
        layout()->addWidget(child);
        showFullScreen();
    }
    else if (e->removed())
    {
        layout()->removeWidget(child);
        hide();
    }
}

void FullScreenVideoWidget::closeEvent(QCloseEvent *e)
{
    e->ignore();
    VideoWidget *vw = qobject_cast<VideoWidget *>(parent());
    vw->exitFullScreen();
}

bool FullScreenVideoWidget::event(QEvent *e)
{
    bool ret = QWidget::event(e);
    if (e->isAccepted())
        return ret;
    else 
    {
        QCoreApplication::sendEvent(parent(), e);
        return true;
    }
}

class VideoWidgetPrivate : public Phonon::AbstractVideoOutputPrivate
{
    K_DECLARE_PUBLIC(VideoWidget)
    protected:
        virtual bool aboutToDeleteBackendObject();
        virtual void createBackendObject();
        void setupBackendObject();

        void _k_cursorTimeout();

        VideoWidgetPrivate(VideoWidget *parent)
            : layout(parent),
            fullScreenWidget(0),
            aspectRatio(VideoWidget::AspectRatioAuto),
            scaleMode(VideoWidget::AddBarsScaleMode)
        {
            layout.setMargin(0);
            cursorTimer.setInterval(1000); // 1s timeout until the cursor disappears
            cursorTimer.setSingleShot(true);
        }

        QHBoxLayout layout;
        QAction *fullScreenAction;
        FullScreenVideoWidget *fullScreenWidget;
        VideoWidget::AspectRatio aspectRatio;
        VideoWidget::ScaleMode scaleMode;
        QTimer cursorTimer;

    private:
        void init();
};
} // namespace Phonon

#endif // VIDEOWIDGET_P_H
// vim: sw=4 ts=4 tw=80
