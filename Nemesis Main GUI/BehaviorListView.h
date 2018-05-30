#ifndef BEHAVIORLISTVIEW_H_
#define BEHAVIORLISTVIEW_H_

#include <QTreeView>
#include <QDragEnterEvent>
#include "ScrollBar.h"
#include "BehaviorListModel.h"
#include "master.h"

extern std::unordered_map<std::string, std::string> modConvert;

class BehaviorListView : public QTreeView
{
	Q_OBJECT
public:
	explicit BehaviorListView(QWidget *parent = 0);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void setModel(QAbstractItemModel *model);

signals:
	void dropModeUpdate(bool dropOnRows);

public slots:

private:

	ScrollBar *m_Scrollbar;
};

#endif