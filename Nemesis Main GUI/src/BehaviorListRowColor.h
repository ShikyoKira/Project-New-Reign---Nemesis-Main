#ifndef BEHAVIORLISTROWCOLOR_H_
#define BEHAVIORLISTROWCOLOR_H_

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QPainter>

class BehaviorListRowDelegate : public QStyledItemDelegate
{
public:
	BehaviorListRowDelegate(QAbstractItemModel* newModel, QModelIndexList* newIndexList);
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QList<QModelIndex>* indexList;
	QAbstractItemModel* model;
};

#endif