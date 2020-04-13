#ifndef SCROLLBAR_H_
#define SCROLLBAR_H_

#include <QAbstractItemModel>
#include <QScrollBar>

class ScrollBar : public QScrollBar
{
public:
    static const int DEFAULT_ROLE = Qt::UserRole + 42;

public:
    ScrollBar(QAbstractItemModel* model, QWidget* parent = 0, int role = DEFAULT_ROLE);

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    QAbstractItemModel* m_Model;
    int m_Role;
};

#endif