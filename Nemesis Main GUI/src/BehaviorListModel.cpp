#include "BehaviorListModel.h"
#include "master.h"

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
	if (!index.isValid())
	{
		return QVariant();
	}

	if (index.row() >= behaviorList.size())
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::CheckStateRole || role == Qt::ToolTipRole)
	{
		if (index.column() == 0)
		{
			if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole)
			{
				return behaviorList.at(index.row()).modname;
			}
			else
			{
				return behaviorList.at(index.row()).state;
			}
		}
		else if (index.column() == 1 && role != Qt::CheckStateRole && role != Qt::ToolTipRole)
		{
			return behaviorList.at(index.row()).author;
		}
		else if (index.column() == 2 && role != Qt::CheckStateRole && role != Qt::ToolTipRole)
		{
			return index.row();
		}
		else
		{
			return QVariant();
		}
	}
	else if (index.column() == 2 && role == Qt::TextAlignmentRole)
	{
		return Qt::AlignCenter;
	}
	else
	{
		return QVariant();
	}
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
				return tr(UIMessage(1005).c_str());
			case COL_AUTHOR:
				return tr(UIMessage(1006).c_str());
			case COL_PRIORITY:
				return tr(UIMessage(1007).c_str());
			default:
				return tr(UIMessage(1008).c_str());
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

	if (index.internalId() < 0)
	{
		return Qt::ItemIsEnabled;
	}

	if (index.isValid())
	{
		defaultflags |= Qt::ItemIsDragEnabled;
		defaultflags |= Qt::ItemIsUserCheckable;
		defaultflags |= Qt::ItemIsDropEnabled;
	}
	else
	{
		if (!m_DropOnItems)
		{
			defaultflags |= Qt::ItemIsDropEnabled;
		}
	}

	return defaultflags;
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
			if (behaviorList[index.row()].state == Qt::Unchecked)
			{
				behaviorList[index.row()].state = Qt::Checked;
			}
			else
			{
				behaviorList[index.row()].state = Qt::Unchecked;
			}

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
	return true;
}

QModelIndex BehaviorListModel::index(int row, int column, const QModelIndex&) const
{
	if ((row < 0) || (row >= rowCount()) || (column < 0) || (column >= columnCount())) {
		return QModelIndex();
	}

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

QMimeData* BehaviorListModel::mimeData(const QModelIndexList & indexes) const
{
	QMimeData* mimeData = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	int column = 0;

	foreach(QModelIndex index, indexes)
	{
		if (index.isValid())
		{
			if (index.column() == 2)
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

			if (index.column() != 2)
			{
				stream << data(index, Qt::DisplayRole).toString();
			}
			else
			{
				column = 0;
			}
		}
	}

	mimeData->setData("application/vnd.text.list", encodedData);
	return mimeData;
}

bool BehaviorListModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	if (action == Qt::IgnoreAction)
	{
		return true;
	}

	if (!data->hasFormat("application/vnd.text.list"))
	{
		return false;
	}

	if (column > 0)
	{
		return false;
	}

	int beginRow;

	if (row != -1)
	{
		beginRow = row;
	}
	else if (parent.isValid())
	{
		beginRow = parent.row();
	}
	else
	{
		beginRow = rowCount(QModelIndex());
	}

	QByteArray encodedData = data->data("application/vnd.text.list");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);
	QList<BehaviorInfo> newItems;
	int rows = 0;
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
			curBehaviorInfo = &newItems.back();
			curBehaviorInfo->modname = text;
			++counter;
		}
		else if (counter == 1)
		{
			curBehaviorInfo->author = text;
			++counter;
		}
		else
		{
			if (text.toInt() != 0)
			{
				curBehaviorInfo->state = Qt::Checked;
			}
			else
			{
				curBehaviorInfo->state = Qt::Unchecked;
			}

			counter = 0;
			++rows;
		}
	}

	insertRows(beginRow, rows, QModelIndex());

	for(auto& item : newItems)
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
	if (m_DropOnItems != dropOnItems)
	{
		m_DropOnItems = dropOnItems;
	}
}
