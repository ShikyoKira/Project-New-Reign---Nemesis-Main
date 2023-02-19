#ifndef BEHAVIORLISTVIEW_H_
#define BEHAVIORLISTVIEW_H_

#include <QDragEnterEvent>
#include <QTreeView>

#include "connector.h"

#include "core/modinfomanager.h"

#include "ui/ScrollBar.h"
#include "ui/BehaviorListModel.h"

class NemesisInfo;
class NemesisEngine;

class BehaviorListView : public QTreeView
{
    Q_OBJECT
public:
    explicit BehaviorListView(QWidget* parent = 0);
    explicit BehaviorListView(NemesisEngine* parent = 0);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void setModel(QAbstractItemModel* model);

    void setIni(NemesisInfo* _ini);

signals:
    void dropModeUpdate(bool dropOnRows);

public slots:
    void resizeHeader(int logicalIndex, int oldSize, int newSize);

private:
    ScrollBar* m_Scrollbar;
    size_t modNameWidth;
    size_t authorWidth;
    size_t priorityWidth;
    NemesisInfo* nemesisInfo;
};

#endif