/*  This file is part of the KDE project
    Copyright (C) 2005-2007 Matthias Kretz <kretz@kde.org>

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
#ifndef Phonon_UI_FAKE_VIDEOWIDGET_H
#define Phonon_UI_FAKE_VIDEOWIDGET_H

#include <QtGui/QWidget>
#include <phonon/experimental/videoframe.h>
#include <phonon/videowidget.h>
#include <phonon/videowidgetinterface.h>
#include "abstractvideooutput.h"
#include <QtGui/QPixmap>

class QString;

namespace Phonon
{
namespace Fake
{
    class VideoWidget : public QWidget, public Phonon::VideoWidgetInterface, public Phonon::Fake::AbstractVideoOutput
    {
        Q_OBJECT
            Q_INTERFACES(Phonon::VideoWidgetInterface Phonon::Fake::AbstractVideoOutput Phonon::Fake::VideoNode)
        public:
            VideoWidget(QWidget *parent = 0);

            Phonon::VideoWidget::AspectRatio aspectRatio() const;
            void setAspectRatio(Phonon::VideoWidget::AspectRatio aspectRatio);
            Phonon::VideoWidget::ScaleMode scaleMode() const;
            void setScaleMode(Phonon::VideoWidget::ScaleMode);
            qreal brightness() const;
            void setBrightness(qreal);
            qreal contrast() const;
            void setContrast(qreal);
            qreal hue() const;
            void setHue(qreal);
            qreal saturation() const;
            void setSaturation(qreal);

            // Fake specific:
            virtual void processFrame(Phonon::Experimental::VideoFrame &frame);

        public slots:
            QWidget *widget() { return this; }
            //int overlayCapabilities() const;
            //bool createOverlay(QWidget *widget, int type);

        protected:
            virtual void childEvent(QChildEvent *event);
            virtual void paintEvent(QPaintEvent *ev);
            virtual void resizeEvent(QResizeEvent *ev);

        private:
            QWidget *overlay;
            bool m_fullscreen;
            QPixmap m_pixmap;
            QSize m_videoSize;
            Phonon::VideoWidget::AspectRatio m_aspectRatio;
            Phonon::VideoWidget::ScaleMode m_scaleMode;
            qreal m_brightness, m_hue, m_contrast, m_saturation;
    };
}} //namespace Phonon::Fake

// vim: sw=4 ts=4 tw=80
#endif // Phonon_UI_FAKE_VIDEOWIDGET_H
