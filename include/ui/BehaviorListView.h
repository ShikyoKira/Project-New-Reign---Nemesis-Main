#ifndef BEHAVIORLISTVIEW_H_
#define BEHAVIORLISTVIEW_H_

#include <QTreeView>
#include <QDragEnterEvent>

#include "connector.h"

#include "ui/ScrollBar.h"
#include "ui/BehaviorListModel.h"

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
	void resizeHeader(int logicalIndex, int oldSize, int newSize);

private:
	ScrollBar *m_Scrollbar;
	unsigned int modNameWidth;
	unsigned int authorWidth;
	unsigned int priorityWidth;
};

#endif