/*  This file is part of the KDE project
    Copyright (C) 2006-2008 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) version 3.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#include "devicepreference.h"

#include <QtCore/QList>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QItemDelegate>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHeaderView>

#include <phonon/backendcapabilities.h>
#include <phonon/objectdescription.h>
#include <phonon/phononnamespace.h>
#include "../libkaudiodevicelist/audiodeviceenumerator.h"
#include "../libkaudiodevicelist/audiodevice.h"
#include "../qsettingsgroup_p.h"
#include "../globalconfig_p.h"
#include <kfadewidgeteffect.h>

#include <kdialog.h>
#include <klistwidget.h>
#include <klocale.h>
#include <kmenu.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kdebug.h>

#ifndef METATYPE_QLIST_INT_DEFINED
#define METATYPE_QLIST_INT_DEFINED
// Want this exactly once, see phonondefs_p.h kcm/devicepreference.cpp
Q_DECLARE_METATYPE(QList<int>)
#endif

using Phonon::QSettingsGroup;

static const Phonon::Category captureCategories[] = {
    Phonon::NoCategory,
    Phonon::CommunicationCategory,
    Phonon::AccessibilityCategory
};
static const int captureCategoriesCount = sizeof(captureCategories)/sizeof(Phonon::Category);

void operator++(Phonon::Category &c)
{
    c = static_cast<Phonon::Category>(1 + static_cast<int>(c));
    //Q_ASSERT(c <= Phonon::LastCategory);
}

class CategoryItem : public QStandardItem {
    public:
        CategoryItem(Phonon::Category cat, bool output = true)
            : QStandardItem(cat == Phonon::NoCategory ? (output ? i18n("Audio Output") : i18n("Audio Capture")) : Phonon::categoryToString(cat)),
            isOutputItem(output),
            m_cat(cat)
        {
        }

        int type() const { return 1001; }
        Phonon::Category category() const { return m_cat; }
        const bool isOutputItem;

    private:
        Phonon::Category m_cat;
};

class DeviceTreeDelegate : public QItemDelegate
{
    public:
        DeviceTreeDelegate(QObject *parent = 0);
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

DeviceTreeDelegate::DeviceTreeDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QSize DeviceTreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant value = index.data(Qt::SizeHintRole);
    if (value.isValid()) {
        return qvariant_cast<QSize>(value);
    }

    QRect iconRect;
    value = index.data(Qt::DecorationRole);
    if (value.type() == QVariant::Icon) {
        QIcon icon = qvariant_cast<QIcon>(value);
        iconRect = QRect(QPoint(0, 0), icon.actualSize(option.decorationSize));
    }

    QRect textRect;
    value = index.data(Qt::DisplayRole);
    if (value.isValid()) {
        QString text = value.toString();
        const QSize size = option.fontMetrics.size(Qt::TextSingleLine, text);
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        textRect = QRect(0, 0, size.width() + 2 * textMargin, size.height());
    }
    QRect checkRect = rect(option, index, Qt::CheckStateRole);

    doLayout(option, &checkRect, &iconRect, &textRect, true);

    return (iconRect | textRect | checkRect).size();
}

void DeviceTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV3 opt = setOptions(index, option);

    const QStyleOptionViewItemV2 *v2 = qstyleoption_cast<const QStyleOptionViewItemV2 *>(&option);
    const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option);
    opt.features = v2 ? v2->features : QStyleOptionViewItemV2::ViewItemFeatures(QStyleOptionViewItemV2::None);
    opt.locale   = v3 ? v3->locale   : QLocale();
    opt.widget   = v3 ? v3->widget   : 0;

    painter->save();
    // clip painter to the actual item - i.e. don't paint outside
    painter->setClipRect(opt.rect);

    QIcon icon;
    QIcon::Mode mode = QIcon::Normal;
    QIcon::State state = QIcon::Off;
    QRect iconRect;
    QVariant value = index.data(Qt::DecorationRole);
    if (value.type() == QVariant::Icon) {
        icon = qvariant_cast<QIcon>(value);
        if (!(option.state & QStyle::State_Enabled)) {
            mode = QIcon::Disabled;
        } else if (option.state & QStyle::State_Selected) {
            mode = QIcon::Selected;
        }
        if (option.state & QStyle::State_Open) {
            state = QIcon::On;
        }
        iconRect = QRect(QPoint(0, 0), icon.actualSize(option.decorationSize, mode, state));
    }

    QString text;
    QRect textRect;
    value = index.data(Qt::DisplayRole);
    if (value.isValid()) {
        text = value.toString();
        const QSize size = opt.fontMetrics.size(Qt::TextSingleLine, text);
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        textRect = QRect(0, 0, size.width() + 2 * textMargin, size.height());
    }

    QRect checkRect;
    Qt::CheckState checkState = Qt::Unchecked;
    value = index.data(Qt::CheckStateRole);
    if (value.isValid()) {
        checkState = static_cast<Qt::CheckState>(value.toInt());
        checkRect = check(opt, opt.rect, value);
    }

    doLayout(opt, &checkRect, &iconRect, &textRect, false);

    drawBackground(painter, opt, index);
    drawCheck(painter, opt, checkRect, checkState);
    icon.paint(painter, iconRect, option.decorationAlignment, mode, state);
    drawDisplay(painter, opt, textRect, text);
    drawFocus(painter, opt, textRect);

    painter->restore();
}

/**
 * Need this to change the colors of the ListView if the Palette changed. With CSS set this won't
 * change automatically
 */
void DevicePreference::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e->type() == QEvent::PaletteChange) {
        //deviceList->viewport()->setStyleSheet(deviceList->viewport()->styleSheet());
        deviceList->setStyleSheet(deviceList->styleSheet());
    }
}

DevicePreference::DevicePreference(QWidget *parent)
    : QWidget(parent),
    m_headerModel(0, 1, 0)
{
    setupUi(this);
    deviceList->setItemDelegate(new DeviceTreeDelegate(deviceList));
    removeButton->setIcon(KIcon("list-remove"));
    deferButton->setIcon(KIcon("go-down"));
    preferButton->setIcon(KIcon("go-up"));
    deviceList->setDragDropMode(QAbstractItemView::InternalMove);
    //deviceList->viewport()->setStyleSheet(QString("QWidget#qt_scrollarea_viewport {"
    deviceList->setStyleSheet(QString("QTreeView {"
                "background-color: palette(base);"
                "background-image: url(%1);"
                "background-position: bottom left;"
                "background-attachment: fixed;"
                "background-repeat: no-repeat;"
                "}")
            .arg(KStandardDirs::locate("data", "kcm_phonon/listview-background.png")));
    deviceList->setAlternatingRowColors(false);
    QStandardItem *parentItem = m_categoryModel.invisibleRootItem();

    // Audio Output Parent
    QStandardItem *outputItem = new CategoryItem(Phonon::NoCategory);
    m_outputModel[Phonon::NoCategory] = new Phonon::AudioOutputDeviceModel;
    outputItem->setEditable(false);
    outputItem->setToolTip(i18n("Defines the default ordering of devices which can be overridden by individual categories."));
    parentItem->appendRow(outputItem);

    // Audio Capture Parent
    QStandardItem *captureItem = new CategoryItem(Phonon::NoCategory, false);
    m_captureModel[Phonon::NoCategory] = new Phonon::AudioCaptureDeviceModel;
    captureItem->setEditable(false);
    captureItem->setToolTip(i18n("Defines the default ordering of devices which can be overridden by individual categories."));
    parentItem->appendRow(captureItem);

    // Audio Output Children
    parentItem = outputItem;
    for (int i = 0; i <= Phonon::LastCategory; ++i) {
        m_outputModel[i] = new Phonon::AudioOutputDeviceModel;
        QStandardItem *item = new CategoryItem(static_cast<Phonon::Category>(i));
        item->setEditable(false);
        parentItem->appendRow(item);
    }

    // Audio Capture Children
    parentItem = captureItem;
    for (int i = 1; i < captureCategoriesCount; ++i) { // i == 1 to skip NoCategory
        m_captureModel[captureCategories[i]] = new Phonon::AudioCaptureDeviceModel;
        QStandardItem *item = new CategoryItem(captureCategories[i], false);
        item->setEditable(false);
        parentItem->appendRow(item);
    }

    categoryTree->setModel(&m_categoryModel);
    if (categoryTree->header()) {
        categoryTree->header()->hide();
    }
    categoryTree->expandAll();

    connect(categoryTree->selectionModel(),
            SIGNAL(currentChanged(const QModelIndex &,const QModelIndex &)),
            SLOT(updateDeviceList()));

    for (int i = -1; i <= Phonon::LastCategory; ++i) {
        connect(m_outputModel[i], SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SIGNAL(changed()));
        connect(m_outputModel[i], SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SIGNAL(changed()));
        connect(m_outputModel[i], SIGNAL(layoutChanged()), this, SIGNAL(changed()));
        connect(m_outputModel[i], SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this, SIGNAL(changed()));
        if (m_captureModel.contains(i)) {
            connect(m_captureModel[i], SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SIGNAL(changed()));
            connect(m_captureModel[i], SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SIGNAL(changed()));
            connect(m_captureModel[i], SIGNAL(layoutChanged()), this, SIGNAL(changed()));
            connect(m_captureModel[i], SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this, SIGNAL(changed()));
        }
    }
    connect(showCheckBox, SIGNAL(stateChanged (int)), this, SIGNAL(changed()));
    connect(Phonon::BackendCapabilities::notifier(), SIGNAL(availableAudioOutputDevicesChanged()), SLOT(updateAudioOutputDevices()));
    connect(Phonon::BackendCapabilities::notifier(), SIGNAL(availableAudioCaptureDevicesChanged()), SLOT(updateAudioCaptureDevices()));

    if (!categoryTree->currentIndex().isValid()) {
        categoryTree->setCurrentIndex(m_categoryModel.index(0, 0).child(1, 0));
    }
}

void DevicePreference::updateDeviceList()
{
    QStandardItem *currentItem = m_categoryModel.itemFromIndex(categoryTree->currentIndex());
    KFadeWidgetEffect *animation = new KFadeWidgetEffect(deviceList);
    if (deviceList->selectionModel()) {
        disconnect(deviceList->selectionModel(),
                SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)),
                this, SLOT(updateButtonsEnabled()));
    }
    if (currentItem && currentItem->type() == 1001) {
        CategoryItem *catItem = static_cast<CategoryItem *>(currentItem);
        const Phonon::Category cat = catItem->category();
        QString deviceString;
        if (catItem->isOutputItem) {
            deviceList->setModel(m_outputModel[cat]);
            deviceString = i18n("Output Device");
        } else {
            deviceList->setModel(m_captureModel[cat]);
            deviceString = i18n("Capture Device");
        }
        if (cat == Phonon::NoCategory) {
            m_headerModel.setHeaderData(0, Qt::Horizontal, i18n("Default %1 Preference:", deviceString), Qt::DisplayRole);
        } else {
            m_headerModel.setHeaderData(0, Qt::Horizontal, i18n("%1 Preference for the '%2' Category:", deviceString, Phonon::categoryToString(cat)), Qt::DisplayRole);
        }
    } else {
        m_headerModel.setHeaderData(0, Qt::Horizontal, QString(), Qt::DisplayRole);
        deviceList->setModel(0);
    }
    deviceList->header()->setModel(&m_headerModel);
    updateButtonsEnabled();
    if (deviceList->selectionModel()) {
        connect(deviceList->selectionModel(),
                SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)),
                this, SLOT(updateButtonsEnabled()));
    }
    deviceList->resizeColumnToContents(0);
    animation->start();
}

void DevicePreference::updateAudioCaptureDevices()
{
    //TODO
    kFatal() << "not implemented";
}

void DevicePreference::updateAudioOutputDevices()
{
    const QList<Phonon::AudioOutputDevice> list = availableAudioOutputDevices();
    QHash<int, Phonon::AudioOutputDevice> hash;
    foreach (const Phonon::AudioOutputDevice &dev, list) {
        hash.insert(dev.index(), dev);
    }
    for (int i = -1; i <= Phonon::LastCategory; ++i) {
        Phonon::AudioOutputDeviceModel *model = m_outputModel.value(i);
        Q_ASSERT(model);

        QHash<int, Phonon::AudioOutputDevice> hashCopy(hash);
        QList<Phonon::AudioOutputDevice> orderedList;
        if (model->rowCount() > 0) {
            QList<int> order = model->tupleIndexOrder();
            foreach (int idx, order) {
                if (hashCopy.contains(idx)) {
                    orderedList << hashCopy.take(idx);
                }
            }
            if (hashCopy.size() > 1) {
                // keep the order of the original list
                foreach (const Phonon::AudioOutputDevice &dev, list) {
                    if (hashCopy.contains(dev.index())) {
                        orderedList << hashCopy.take(dev.index());
                    }
                }
            } else if (hashCopy.size() == 1) {
                orderedList += hashCopy.values();
            }
            model->setModelData(orderedList);
        } else {
            model->setModelData(list);
        }
    }
    deviceList->resizeColumnToContents(0);
}

QList<Phonon::AudioOutputDevice> DevicePreference::availableAudioOutputDevices() const
{
    QList<Phonon::AudioOutputDevice> ret;
    const QList<int> deviceIndexes = Phonon::GlobalConfig().audioOutputDeviceListFor(Phonon::NoCategory,
            showCheckBox->isChecked()
            ? Phonon::GlobalConfig::ShowAdvancedDevices
            : Phonon::GlobalConfig::HideAdvancedDevices);
    foreach (int i, deviceIndexes) {
        ret.append(Phonon::AudioOutputDevice::fromIndex(i));
    }
    return ret;
}

QList<Phonon::AudioCaptureDevice> DevicePreference::availableAudioCaptureDevices() const
{
    QList<Phonon::AudioCaptureDevice> ret;
    const QList<int> deviceIndexes = Phonon::GlobalConfig().audioCaptureDeviceListFor(Phonon::NoCategory,
            showCheckBox->isChecked()
            ? Phonon::GlobalConfig::ShowAdvancedDevices
            : Phonon::GlobalConfig::HideAdvancedDevices);
    kDebug() << deviceIndexes;
    foreach (int i, deviceIndexes) {
        ret.append(Phonon::AudioCaptureDevice::fromIndex(i));
    }
    return ret;
}

void DevicePreference::load()
{
    QSettings phononConfig(QLatin1String("kde.org"), QLatin1String("libphonon"));
    QSettingsGroup outputDeviceGroup(&phononConfig, QLatin1String("AudioOutputDevice"));
    QSettingsGroup captureDeviceGroup(&phononConfig, QLatin1String("AudioCaptureDevice"));
    QSettingsGroup generalGroup(&phononConfig, QLatin1String("General"));
    showCheckBox->setChecked(!generalGroup.value(QLatin1String("HideAdvancedDevices"), true));

    {
        // the following call returns ordered according to NoCategory
        const QList<Phonon::AudioOutputDevice> list = availableAudioOutputDevices();
        m_outputModel[Phonon::NoCategory]->setModelData(list);

        QHash<int, Phonon::AudioOutputDevice> hash;
        foreach (const Phonon::AudioOutputDevice &dev, list) {
            hash.insert(dev.index(), dev);
        }
        for (int i = 0; i <= Phonon::LastCategory; ++i) {
            const QString configKey(QLatin1String("Category") + QString::number(i));
            if (!outputDeviceGroup.hasKey(configKey)) {
                m_outputModel[i]->setModelData(list); // use the NoCategory order
                continue;
            }
            QHash<int, Phonon::AudioOutputDevice> hashCopy(hash);
            const QList<int> order = outputDeviceGroup.value(configKey, QList<int>());
            QList<Phonon::AudioOutputDevice> orderedList;
            foreach (int idx, order) {
                if (hashCopy.contains(idx)) {
                    orderedList << hashCopy.take(idx);
                }
            }
            if (hashCopy.size() > 1) {
                // keep the order of the original list
                foreach (const Phonon::AudioOutputDevice &dev, list) {
                    if (hashCopy.contains(dev.index())) {
                        orderedList << hashCopy.take(dev.index());
                    }
                }
            } else if (hashCopy.size() == 1) {
                orderedList += hashCopy.values();
            }
            m_outputModel[i]->setModelData(orderedList);
        }
    }
    {
        // the following call returns ordered according to NoCategory
        QList<Phonon::AudioCaptureDevice> list = availableAudioCaptureDevices();
        m_captureModel[Phonon::NoCategory]->setModelData(list);

        QHash<int, Phonon::AudioCaptureDevice> hash;
        foreach (const Phonon::AudioCaptureDevice &dev, list) {
            hash.insert(dev.index(), dev);
        }
        for (int i = 1; i < captureCategoriesCount; ++i) { // i == 1 to skip NoCategory
            const Phonon::Category cat = captureCategories[i];
            const QString configKey(QLatin1String("Category") + QString::number(cat));
            if (!captureDeviceGroup.hasKey(configKey)) {
                m_captureModel[cat]->setModelData(list); // use the NoCategory order
                continue;
            }
            QHash<int, Phonon::AudioCaptureDevice> hashCopy(hash);
            const QList<int> order = captureDeviceGroup.value(configKey, QList<int>());
            QList<Phonon::AudioCaptureDevice> orderedList;
            foreach (int idx, order) {
                if (hashCopy.contains(idx)) {
                    orderedList << hashCopy.take(idx);
                }
            }
            if (hashCopy.size() > 1) {
                // keep the order of the original list
                foreach (const Phonon::AudioCaptureDevice &dev, list) {
                    if (hashCopy.contains(dev.index())) {
                        orderedList << hashCopy.take(dev.index());
                    }
                }
            } else if (hashCopy.size() == 1) {
                orderedList += hashCopy.values();
            }
            m_captureModel[cat]->setModelData(orderedList);
        }
    }

    deviceList->resizeColumnToContents(0);
}

void DevicePreference::save()
{
    QSettings config(QLatin1String("kde.org"), QLatin1String("libphonon"));
    {
        QSettingsGroup generalGroup(&config, QLatin1String("General"));
        generalGroup.setValue(QLatin1String("HideAdvancedDevices"), !showCheckBox->isChecked());
    }
    {
        QSettingsGroup globalGroup(&config, QLatin1String("AudioOutputDevice"));
        const QList<int> noCategoryOrder = m_outputModel.value(Phonon::NoCategory)->tupleIndexOrder();
        globalGroup.setValue(QLatin1String("Category") + QString::number(Phonon::NoCategory), noCategoryOrder);
        for (int i = 0; i <= Phonon::LastCategory; ++i) {
            Q_ASSERT(m_outputModel.value(i));
            const QList<int> order = m_outputModel.value(i)->tupleIndexOrder();
            if (order == noCategoryOrder) {
                globalGroup.removeEntry(QLatin1String("Category") + QString::number(i));
            } else {
                globalGroup.setValue(QLatin1String("Category") + QString::number(i), order);
            }
        }
    }
    {
        QSettingsGroup globalGroup(&config, QLatin1String("AudioCaptureDevice"));
        const QList<int> noCategoryOrder = m_captureModel.value(Phonon::NoCategory)->tupleIndexOrder();
        globalGroup.setValue(QLatin1String("Category") + QString::number(Phonon::NoCategory), noCategoryOrder);
        for (int i = 1; i < captureCategoriesCount; ++i) {
            const Phonon::Category cat = captureCategories[i];
            Q_ASSERT(m_captureModel.value(cat));
            const QList<int> order = m_captureModel.value(cat)->tupleIndexOrder();
            if (order == noCategoryOrder) {
                globalGroup.removeEntry(QLatin1String("Category") + QString::number(cat));
            } else {
                globalGroup.setValue(QLatin1String("Category") + QString::number(cat), order);
            }
        }
    }
}

void DevicePreference::defaults()
{
    {
        QList<Phonon::AudioOutputDevice> list = availableAudioOutputDevices();
        for (int i = -1; i <= Phonon::LastCategory; ++i) {
            m_outputModel[i]->setModelData(list);
        }
    }
    {
        QList<Phonon::AudioCaptureDevice> list = availableAudioCaptureDevices();
        for (int i = 0; i < captureCategoriesCount; ++i) {
            m_captureModel[captureCategories[i]]->setModelData(list);
        }
    }

    deviceList->resizeColumnToContents(0);
}

void DevicePreference::on_preferButton_clicked()
{
    QAbstractItemModel *model = deviceList->model();
    {
        Phonon::AudioOutputDeviceModel *deviceModel = qobject_cast<Phonon::AudioOutputDeviceModel *>(model);
        if (deviceModel) {
            deviceModel->moveUp(deviceList->currentIndex());
            updateButtonsEnabled();
            emit changed();
        }
    }
    {
        Phonon::AudioCaptureDeviceModel *deviceModel = qobject_cast<Phonon::AudioCaptureDeviceModel *>(model);
        if (deviceModel) {
            deviceModel->moveUp(deviceList->currentIndex());
            updateButtonsEnabled();
            emit changed();
        }
    }
}

void DevicePreference::on_deferButton_clicked()
{
    QAbstractItemModel *model = deviceList->model();
    {
        Phonon::AudioOutputDeviceModel *deviceModel = qobject_cast<Phonon::AudioOutputDeviceModel *>(model);
        if (deviceModel) {
            deviceModel->moveDown(deviceList->currentIndex());
            updateButtonsEnabled();
            emit changed();
        }
    }
    {
        Phonon::AudioCaptureDeviceModel *deviceModel = qobject_cast<Phonon::AudioCaptureDeviceModel *>(model);
        if (deviceModel) {
            deviceModel->moveDown(deviceList->currentIndex());
            updateButtonsEnabled();
            emit changed();
        }
    }
}

void DevicePreference::on_removeButton_clicked()
{
    const QModelIndex idx = deviceList->currentIndex();

    QAbstractItemModel *model = deviceList->model();
    Phonon::AudioOutputDeviceModel *playbackModel = qobject_cast<Phonon::AudioOutputDeviceModel *>(model);
    if (playbackModel && idx.isValid()) {
        const Phonon::AudioOutputDevice deviceToRemove = playbackModel->modelData(idx);
        const QList<Phonon::AudioDevice> deviceList = Phonon::AudioDeviceEnumerator::availablePlaybackDevices();
        foreach (Phonon::AudioDevice dev, deviceList) {
            if (dev.index() == deviceToRemove.index()) {
                // remove from persistent store
                if (dev.ceaseToExist()) {
                    // remove from all models, idx.row() is only correct for the current model
                    foreach (Phonon::AudioOutputDeviceModel *model, m_outputModel) {
                        QList<Phonon::AudioOutputDevice> data = model->modelData();
                        for (int row = 0; row < data.size(); ++row) {
                            if (data[row] == deviceToRemove) {
                                model->removeRows(row, 1);
                                break;
                            }
                        }
                    }
                    updateButtonsEnabled();
                    emit changed();
                }
            }
        }
        /*
    } else {
        Phonon::AudioCaptureDeviceModel *captureModel = qobject_cast<Phonon::AudioCaptureDeviceModel *>(model);
        if (captureModel && idx.isValid()) {
            Phonon::AudioCaptureDevice deviceToRemove = captureModel->modelData(idx);
            QList<Phonon::AudioDevice> deviceList = Phonon::AudioDeviceEnumerator::availableCaptureDevices();
            foreach (Phonon::AudioDevice dev, deviceList) {
                if (dev.index() == deviceToRemove.index()) {
                    // remove from persistent store
                    if (dev.ceaseToExist()) {
                        m_captureModel.removeRows(idx.row(), 1);
                        updateButtonsEnabled();
                        emit changed();
                    }
                }
            }
        }
        */
    }

    deviceList->resizeColumnToContents(0);
}

void DevicePreference::on_applyPreferencesButton_clicked()
{
    const QModelIndex idx = categoryTree->currentIndex();
    const QStandardItem *item = m_categoryModel.itemFromIndex(idx);
    Q_ASSERT(item->type() == 1001);
    const CategoryItem *catItem = static_cast<const CategoryItem *>(item);
    const QList<Phonon::AudioOutputDevice> modelData = m_outputModel.value(catItem->category())->modelData();

    KDialog dialog(this);
    dialog.setButtons(KDialog::Ok | KDialog::Cancel);
    dialog.setDefaultButton(KDialog::Ok);

    QWidget mainWidget(&dialog);
    dialog.setMainWidget(&mainWidget);

    QLabel label(&mainWidget);
    label.setText(i18n("Apply the currently shown device preference list to the following other "
                "audio output categories:"));
    label.setWordWrap(true);

    KListWidget list(&mainWidget);
    for (Phonon::Category cat = Phonon::NoCategory; cat <= Phonon::LastCategory; ++cat) {
        QListWidgetItem *item = new QListWidgetItem(cat == Phonon::NoCategory
                ? i18n("Default/Unspecified Category") : Phonon::categoryToString(cat), &list, cat);
        item->setCheckState(Qt::Checked);
        if (cat == catItem->category()) {
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
    }

    QVBoxLayout layout(&mainWidget);
    layout.setMargin(0);
    layout.addWidget(&label);
    layout.addWidget(&list);

    switch (dialog.exec()) {
    case QDialog::Accepted:
        for (Phonon::Category cat = Phonon::NoCategory; cat <= Phonon::LastCategory; ++cat) {
            if (cat != catItem->category()) {
                QListWidgetItem *item = list.item(static_cast<int>(cat) + 1);
                Q_ASSERT(item->type() == cat);
                if (item->checkState() == Qt::Checked) {
                    m_outputModel.value(cat)->setModelData(modelData);
                }
            }
        }
        break;
    case QDialog::Rejected:
        // nothing to do
        break;
    }
}

void DevicePreference::on_showCheckBox_toggled()
{
    {
        // the following call returns ordered according to NoCategory
        const QList<Phonon::AudioOutputDevice> list = availableAudioOutputDevices();
        m_outputModel[Phonon::NoCategory]->setModelData(list);

        QHash<int, Phonon::AudioOutputDevice> hash;
        foreach (const Phonon::AudioOutputDevice &dev, list) {
            hash.insert(dev.index(), dev);
        }
        for (int i = 0; i <= Phonon::LastCategory; ++i) {
            QHash<int, Phonon::AudioOutputDevice> hashCopy(hash);
            const QList<int> order = m_outputModel[i]->tupleIndexOrder();
            QList<Phonon::AudioOutputDevice> orderedList;
            foreach (int idx, order) {
                if (hashCopy.contains(idx)) {
                    orderedList << hashCopy.take(idx);
                }
            }
            if (hashCopy.size() > 1) {
                // keep the order of the original list
                foreach (const Phonon::AudioOutputDevice &dev, list) {
                    if (hashCopy.contains(dev.index())) {
                        orderedList << hashCopy.take(dev.index());
                    }
                }
            } else if (hashCopy.size() == 1) {
                orderedList += hashCopy.values();
            }
            m_outputModel[i]->setModelData(orderedList);
        }
    }
    {
        // the following call returns ordered according to NoCategory
        const QList<Phonon::AudioCaptureDevice> list = availableAudioCaptureDevices();
        m_captureModel[Phonon::NoCategory]->setModelData(list);

        QHash<int, Phonon::AudioCaptureDevice> hash;
        foreach (const Phonon::AudioCaptureDevice &dev, list) {
            hash.insert(dev.index(), dev);
        }
        for (int i = 1; i < captureCategoriesCount; ++i) {
            const Phonon::Category cat = captureCategories[i];
            QHash<int, Phonon::AudioCaptureDevice> hashCopy(hash);
            const QList<int> order = m_captureModel[cat]->tupleIndexOrder();
            QList<Phonon::AudioCaptureDevice> orderedList;
            foreach (int idx, order) {
                if (hashCopy.contains(idx)) {
                    orderedList << hashCopy.take(idx);
                }
            }
            if (hashCopy.size() > 1) {
                // keep the order of the original list
                foreach (const Phonon::AudioCaptureDevice &dev, list) {
                    if (hashCopy.contains(dev.index())) {
                        orderedList << hashCopy.take(dev.index());
                    }
                }
            } else if (hashCopy.size() == 1) {
                orderedList += hashCopy.values();
            }
            m_captureModel[cat]->setModelData(orderedList);
        }
    }
    deviceList->resizeColumnToContents(0);
}

void DevicePreference::updateButtonsEnabled()
{
    //kDebug() ;
    if (deviceList->model()) {
        //kDebug() << "model available";
        QModelIndex idx = deviceList->currentIndex();
        preferButton->setEnabled(idx.isValid() && idx.row() > 0);
        deferButton->setEnabled(idx.isValid() && idx.row() < deviceList->model()->rowCount() - 1);
        removeButton->setEnabled(idx.isValid() && !(idx.flags()  & Qt::ItemIsEnabled));
    } else {
        preferButton->setEnabled(false);
        deferButton->setEnabled(false);
        removeButton->setEnabled(false);
    }
}

#include "moc_devicepreference.cpp"
// vim: sw=4 ts=4