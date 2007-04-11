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

#ifndef PHONON_OBJECTDESCRIPTIONMODEL_H
#define PHONON_OBJECTDESCRIPTIONMODEL_H

#include "phonon_export.h"
#include "objectdescription.h"
#include <QAbstractListModel>

template<class t> class QList;

namespace Phonon
{
    template<ObjectDescriptionType type>
    class ObjectDescriptionModelPrivate;

    /**
     * \short The ObjectDescriptionModel class provides a model from
     * a list of ObjectDescription objects.
     *
     * ObjectDescriptionModel is a readonly model that supplies a list
     * using ObjectDescription::name() for the text and
     * ObjectDescription::description() for the tooltip. If set the properties
     * "icon" and "available" are used to set the decoration and disable the
     * item (disabled only visually, you can still select and drag it).
     *
     * It also provides the methods moveUp() and moveDown() to order the list.
     * Additionally drag and drop is possible so that
     * QAbstractItemView::InternalMove can be used.
     * The resulting order of the ObjectDescription::index() values can then be
     * retrieved using tupleIndexOrder().
     *
     * An example use case would be to give the user a QComboBox to select
     * the output device:
     * \code
     * QComboBox *cb = new QComboBox(parentWidget);
     * ObjectDescriptionModel *model = new ObjectDescriptionModel(cb);
     * model->setModelData(BackendCapabilities::availableAudioOutputDevices());
     * cb->setModel(model);
     * cb->setCurrentIndex(0); // select first entry
     * \endcode
     *
     * And to retrieve the selected AudioOutputDevice:
     * \code
     * int cbIndex = cb->currentIndex();
     * AudioOutputDevice selectedDevice = model->modelData(cbIndex);
     * \endcode
     *
     * \author Matthias Kretz <kretz@kde.org>
     */
    template<ObjectDescriptionType type>
    class PHONONCORE_EXPORT ObjectDescriptionModel : public QAbstractListModel
    {
        inline ObjectDescriptionModelPrivate<type> *d_func() { return reinterpret_cast<ObjectDescriptionModelPrivate<type> *>(d_ptr); } \
        inline const ObjectDescriptionModelPrivate<type> *d_func() const { return reinterpret_cast<const ObjectDescriptionModelPrivate<type> *>(d_ptr); } \
        friend class ObjectDescriptionModelPrivate<type>;
        public:
#if QT_VERSION >= 0x040300
            Q_OBJECT_CHECK
#endif
            /** \internal */
            static const QMetaObject staticMetaObject;
            /** \internal */
            const QMetaObject *metaObject() const;
            /** \internal */
            void *qt_metacast(const char *_clname);
            //int qt_metacall(QMetaObject::Call _c, int _id, void **_a);

            /**
             * Constructs a ObjectDescription model with the
             * given \p parent.
             */
            explicit ObjectDescriptionModel(QObject *parent = 0);

            /**
             * Constructs a ObjectDescription model with the
             * given \p parent and the given \p data.
             */
            explicit ObjectDescriptionModel(const QList<ObjectDescription<type> > &data, QObject *parent = 0);

            virtual ~ObjectDescriptionModel();

            /**
             * Sets the model data using the list provided by \p data.
             *
             * All previous model data is cleared.
             */
            void setModelData(const QList<ObjectDescription<type> > &data);

            /**
             * Returns the model data.
             *
             * As the order of the list might have changed this can be different
             * to what was set using setModelData().
             */
            QList<ObjectDescription<type> > modelData() const;

            /**
             * Returns one ObjectDescription of the model data for the given \p index.
             */
            ObjectDescription<type> modelData(const QModelIndex &index) const;

            /**
             * Returns the number of rows in the model. This value corresponds
             * to the size of the list passed through setModelData.
             *
             * \param parent The optional \p parent argument is used in most models to specify
             * the parent of the rows to be counted. Because this is a list if a
             * valid parent is specified the result will always be 0.
             *
             * Reimplemented from QAbstractItemModel.
             *
             * \see QAbstractItemModel::rowCount
             */
            int rowCount(const QModelIndex &parent = QModelIndex()) const;

            /**
             * Returns data from the item with the given \p index for the specified
             * \p role.
             * If the view requests an invalid index, an invalid variant is
             * returned.
             *
             * Reimplemented from QAbstractItemModel.
             *
             * \see QAbstractItemModel::data
             * \see Qt::ItemDataRole
             */
            QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

            /**
             * Reimplemented to show unavailable devices as disabled (but still
             * selectable).
             */
            Qt::ItemFlags flags(const QModelIndex &index) const;

            /**
             * Moves the item at the given \p index up. In the resulting list
             * the items at index.row() and index.row() - 1 are swapped.
             *
             * Connected views are updated automatically.
             */
            void moveUp(const QModelIndex &index);

            /**
             * Moves the item at the given \p index down. In the resulting list
             * the items at index.row() and index.row() + 1 are swapped.
             *
             * Connected views are updated automatically.
             */
            void moveDown(const QModelIndex &index);

            /**
             * Returns a list of indexes in the same order as they are in the
             * model. The indexes come from the ObjectDescription::index
             * method.
             *
             * This is useful to let the user define a list of preference.
             */
            QList<int> tupleIndexOrder() const;

            /**
             * Returns the ObjectDescription::index for the tuple
             * at the given position \p positionIndex. For example a
             * QComboBox will give you the currentIndex as the
             * position in the list. But to select the according
             * AudioOutputDevice using AudioOutputDevice::fromIndex
             * you can use this method.
             *
             * \param positionIndex The position in the list.
             */
            int tupleIndexAtPositionIndex(int positionIndex) const;

            /**
             * This model supports drag and drop to copy or move
             * items.
             */
            Qt::DropActions supportedDropActions() const;

            /**
             * Accept drops from other models of the same ObjectDescriptionType.
             *
             * If a valid \p parent is given the dropped items will be inserted
             * above that item.
             */
            bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

            /**
             * Removes count rows starting with the given row.
             *
             * If a valid \p parent is given no rows are removed since this is a
             * list model.
             *
             * Returns true if the rows were successfully removed; otherwise returns false.
             */
            bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

            /**
             * Returns a list of supported drag and drop MIME types. Currently
             * it only supports one type used internally.
             */
            QStringList mimeTypes() const;

            /**
             * Returns the MIME data that dropMimeData() can use to create new
             * items.
             */
            QMimeData *mimeData(const QModelIndexList &indexes) const;

        protected:
            ObjectDescriptionModelPrivate<type> *d_ptr;
    };

    typedef ObjectDescriptionModel<AudioOutputDeviceType> AudioOutputDeviceModel;
    typedef ObjectDescriptionModel<AudioCaptureDeviceType> AudioCaptureDeviceModel;
    typedef ObjectDescriptionModel<VideoOutputDeviceType> VideoOutputDeviceModel;
    typedef ObjectDescriptionModel<VideoCaptureDeviceType> VideoCaptureDeviceModel;
    typedef ObjectDescriptionModel<AudioEffectType> AudioEffectDescriptionModel;
    typedef ObjectDescriptionModel<VideoEffectType> VideoEffectDescriptionModel;
    typedef ObjectDescriptionModel<AudioCodecType> AudioCodecDescriptionModel;
    typedef ObjectDescriptionModel<VideoCodecType> VideoCodecDescriptionModel;
    typedef ObjectDescriptionModel<ContainerFormatType> ContainerFormatDescriptionModel;
    typedef ObjectDescriptionModel<VisualizationType> VisualizationDescriptionModel;

}

#endif // PHONON_OBJECTDESCRIPTIONMODEL_H
// vim: sw=4 ts=4 tw=80
