#include "BehaviorListView.h"
#include <QUrl>
#include <QMimeData>
#include <QProxyStyle>
#include <QMessageBox>
#include <QObject>
#include <QHeaderView>
#include "ErrorMsgBox.h"

std::unordered_map<std::string, std::string> modConvert;
extern std::atomic<unsigned int> resizeCount;

class BehaviorListViewStyle : public QProxyStyle
{
public:
	BehaviorListViewStyle(QStyle *style, int indentation);

	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;

private:
	int m_Indentation;
};

BehaviorListViewStyle::BehaviorListViewStyle(QStyle *style, int indentation) : QProxyStyle(style), m_Indentation(indentation)
{
}

void BehaviorListViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull())
	{
		QStyleOption opt(*option);
		opt.rect.setLeft(m_Indentation);

		if (widget) opt.rect.setRight(widget->width() - 5); // 5 is an arbitrary value that seems to work ok

		QProxyStyle::drawPrimitive(element, &opt, painter, widget);
	}
	else
	{
		QProxyStyle::drawPrimitive(element, option, painter, widget);
	}
}

BehaviorListView::BehaviorListView(QWidget *parent) : QTreeView(parent), m_Scrollbar(new ScrollBar(this->model(), this))
{
	setVerticalScrollBar(m_Scrollbar);
	QList<BehaviorInfo> mods;

	std::string errmsg;

	if (!readMod(errmsg))
	{
		CEMsgBox* msgbox = new CEMsgBox;
		QString msg = QString::fromStdString(errmsg);
		msgbox->setText(msg);
		msgbox->setWindowTitle("CRITITAL ERROR");
		msgbox->show();
		error = true;
		return;
	}

	for (auto& info : modinfo)
	{
		modConvert[info.second[0]] = info.first;
		mods.push_back(*new BehaviorInfo(QString::fromStdString(info.second[0]), QString::fromStdString(info.second[1])));
	}

	setRootIsDecorated(false);
	setModel(new BehaviorListModel(mods));

	QObject::connect(header(), &QHeaderView::sectionResized, this, &BehaviorListView::resizeHeader);
}

void BehaviorListView::dragEnterEvent(QDragEnterEvent *event)
{
	emit dropModeUpdate(event->mimeData()->hasUrls());
	QTreeView::dragEnterEvent(event);
}

void BehaviorListView::setModel(QAbstractItemModel *model)
{
	QTreeView::setModel(model);
	setVerticalScrollBar(new ScrollBar(model, this));
}

bool BehaviorListView::eventFilter(QObject* object, QEvent* event)
{
	QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);

	if (mouse_event)
	{
		if (mouse_event->type() == QEvent::MouseButtonPress && mouse_event->button() == Qt::LeftButton && !mouse_dragging)
		{
			mouse_dragging = true;
		}
	}

	int a = 5;
	int b = 10;
	int c = 15;
	return false;
}

void BehaviorListView::resizeHeader(int logicalIndex, int oldSize, int newSize)
{
	if (resizeCount != 0) return;

	if (logicalIndex == 2)
	{
		if (oldSize < newSize)
		{
			// DRAF LEFT, EXPAND 
			if (columnWidth(1) < authorWidth)
			{

			}
		}

		priorityWidth = newSize;
		nemesisInfo->setPriorityWidth(newSize);
	}
	else if (logicalIndex == 1)
	{
		nemesisInfo->setAuthorWidth(newSize);
	}
	else
	{
		nemesisInfo->setModNameWidth(newSize);
	}
}
