#include "Global.h"

#include <QDesktopServices>

#include <QtGui/QColor>

#include "connector.h"

#include "ui/BehaviorListModel.h"
#include "ui/BehaviorListView.h"
#include "ui/SettingsSave.h"

int BehaviorListModel::rowCount(const QModelIndex& parent) const
{
    return behaviorList.size();
}

int BehaviorListModel::columnCount(const QModelIndex& parent) const
{
    return COL_LASTCOL + 1;
}

QVariant BehaviorListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= behaviorList.size()) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::CheckStateRole
        || role == Qt::ToolTipRole)
    {
        if (index.column() == 0)
        {
            if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole)
                return behaviorList.at(index.row()).modname;
            else
                return behaviorList.at(index.row()).state;
        }
        else if (role != Qt::CheckStateRole && role != Qt::ToolTipRole)
        {
            if (index.column() == 1)
                return behaviorList.at(index.row()).author;
            else if (index.column() == 2)
                return index.row();
        }
    }
    else if (index.column() == 2 && role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    else if (index.row() % 2 != 0 && role == Qt::BackgroundColorRole)
    {
        return QColor(235, 235, 235);
    }

    return QVariant();
}

QVariant BehaviorListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            switch (section)
            {
                case COL_NAME:
                    return QString::fromStdWString(UIMessage(1005));
                case COL_AUTHOR:
                    return QString::fromStdWString(UIMessage(1006));
                case COL_PRIORITY:
                    return QString::fromStdWString(UIMessage(1007));
                default:
                    return QString::fromStdWString(UIMessage(1008));
            }
        }
    }
    else
    {
        return QString("Row %1").arg(section);
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags BehaviorListModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags defaultflags = QAbstractItemModel::flags(index);

    if (index.internalId() < 0) return Qt::ItemIsEnabled;

    if (index.isValid())
        return defaultflags | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled;
    else
        return defaultflags | Qt::ItemIsDropEnabled;
}

bool BehaviorListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid())
    {
        if (role == Qt::EditRole)
        {
            BehaviorInfo item = value.value<BehaviorInfo>();
            behaviorList.replace(index.row(), item);
            emit dataChanged(index, index);
            return true;
        }
        else if (role == Qt::CheckStateRole)
        {
            tempCheck                       = behaviorList[index.row()].state;
            click_time                      = std::chrono::high_resolution_clock::now();
            behaviorList[index.row()].state = tempCheck == Qt::Unchecked ? Qt::Checked : Qt::Unchecked;
            VecStr chosenBehavior;

            for (auto& behavior : behaviorList)
            {
                if (behavior.state) chosenBehavior.push_back(behavior.modname.toStdString());
            }

            createModCache(chosenBehavior);
            emit dataChanged(index, index);
            return true;
        }
    }

    return false;
}

bool BehaviorListModel::insertRows(int position, int rows, const QModelIndex& parent)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
        BehaviorInfo newinfo;
        newinfo.modname = "new mod";
        behaviorList.insert(behaviorList.begin() + position, newinfo);
    }

    endInsertRows();
    return true;
}

bool BehaviorListModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
        behaviorList.erase(behaviorList.begin() + position);
    }

    endRemoveRows();
    VecStr behaviorOrder;
    behaviorOrder.reserve(behaviorList.size());

    for (auto& behavior : behaviorList)
    {
        behaviorOrder.push_back(modConvert[behavior.modname.toStdString()]);
    }

    createModOrderCache(behaviorOrder);
    return true;
}

void BehaviorListModel::goToUrl(const QModelIndex& index)
{
    if (index.column() != 0) return;

    if (tempCheck != behaviorList[index.row()].state)
    {
        auto diff = (std::chrono::high_resolution_clock::now() - click_time);
        int ms    = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

        if (ms < 400) return;
    }

    std::string link = data(index, Qt::DisplayRole).toString().toStdString();
    size_t pos       = link.rfind("(") + 1;
    link             = link.substr(pos, link.length() - 1 - pos);
    QDesktopServices::openUrl(QUrl(QString::fromStdString(link)));
}

QModelIndex BehaviorListModel::index(int row, int column, const QModelIndex&) const
{
    if ((row < 0) || (row >= rowCount()) || (column < 0) || (column >= columnCount())) QModelIndex();

    QModelIndex res = createIndex(row, column, row);
    return res;
}

QModelIndex BehaviorListModel::parent(const QModelIndex& index) const
{
    return QModelIndex();
}

Qt::DropActions BehaviorListModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList BehaviorListModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData* BehaviorListModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    int column = 0;

    foreach (QModelIndex index, indexes)
    {
        if (index.isValid())
        {
            stream << data(index, Qt::DisplayRole).toString();

            if (index.column() == 0)
            {
                if (data(index, Qt::CheckStateRole) == Qt::Checked)
                {
                    QString text = "1";
                    stream << text;
                }
                else
                {
                    QString text = "0";
                    stream << text;
                }
            }
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

bool BehaviorListModel::dropMimeData(
    const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    if (action == Qt::IgnoreAction) return true;

    if (!data->hasFormat("application/vnd.text.list") || column > 0) return false;

    int beginRow;

    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = parent.row();
    else
        beginRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<BehaviorInfo> newItems;
    int rows    = 0;
    int counter = 0;
    BehaviorInfo* curBehaviorInfo;

    while (!stream.atEnd())
    {
        QString text;
        stream >> text;

        if (counter == 0)
        {
            BehaviorInfo newInfo;
            newItems.push_back(newInfo);
            curBehaviorInfo          = &newItems.back();
            curBehaviorInfo->modname = text;
            ++counter;
        }
        else if (counter == 1)
        {
            curBehaviorInfo->state = text.toInt() != 0 ? Qt::Checked : Qt::Unchecked;
            ++counter;
        }
        else if (counter == 2)
        {
            curBehaviorInfo->author = text;
            ++counter;
        }
        else
        {
            counter = 0;
            ++rows;
        }
    }

    insertRows(beginRow, rows, QModelIndex());

    for (auto item : newItems)
    {
        QModelIndex idx = index(beginRow, 0, QModelIndex());
        QVariant stored;
        stored.setValue(item);
        setData(idx, stored);
        ++beginRow;
    }

    return true;
}

void BehaviorListModel::dropModeUpdate(bool dropOnItems)
{
    if (m_DropOnItems != dropOnItems) m_DropOnItems = dropOnItems;
}
