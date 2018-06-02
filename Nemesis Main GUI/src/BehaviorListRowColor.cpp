#include "BehaviorListRowColor.h"

BehaviorListRowDelegate::BehaviorListRowDelegate(QAbstractItemModel* newModel, QModelIndexList* newIndexList)
{
	model = newModel;
	indexList = newIndexList;
}

void BehaviorListRowDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	int row = index.row();

	if (indexList->contains(index))
	{
		if (row % 2 != 0)
		{
			painter->fillRect(option.rect, QColor(240, 240, 240));
		}
	}

	QStyledItemDelegate::paint(painter, option, index);
}
