#include "BehaviorListView.h"
#include <QUrl>
#include <QMimeData>
#include <QProxyStyle>
#include <QMessageBox>
#include <QObject>
#include "ErrorMsgBox.h"

std::unordered_map<std::string, std::string> modConvert;

class BehaviorListViewStyle : public QProxyStyle {
public:
	BehaviorListViewStyle(QStyle *style, int indentation);

	void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
		QPainter *painter, const QWidget *widget = 0) const;
private:
	int m_Indentation;
};

BehaviorListViewStyle::BehaviorListViewStyle(QStyle *style, int indentation)
	: QProxyStyle(style), m_Indentation(indentation)
{
}

void BehaviorListViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
	QPainter *painter, const QWidget *widget) const
{
	if (element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()) {
		QStyleOption opt(*option);
		opt.rect.setLeft(m_Indentation);
		if (widget) {
			opt.rect.setRight(widget->width() - 5); // 5 is an arbitrary value that seems to work ok
		}
		QProxyStyle::drawPrimitive(element, &opt, painter, widget);
	}
	else {
		QProxyStyle::drawPrimitive(element, option, painter, widget);
	}
}

BehaviorListView::BehaviorListView(QWidget *parent)
	: QTreeView(parent)
	, m_Scrollbar(new ScrollBar(this->model(), this))
{
	setVerticalScrollBar(m_Scrollbar);
	QList<BehaviorInfo> mods;

	std::string modcode;

	if (!readMod(modcode))
	{
		CEMsgBox* msgbox = new CEMsgBox;
		QString msg = "Missing info in \"info.ini\" file. Please contact the mod author\nMod: " + QString::fromStdString(modcode);
		msgbox->setText(msg);
		msgbox->setWindowTitle("CRITITAL ERROR");
		msgbox->show();
		return;
	}

	for (auto& info : modinfo)
	{
		modConvert[info.second[0]] = info.first;
		mods.push_back(*new BehaviorInfo(QString::fromStdString(info.second[0]), QString::fromStdString(info.second[1])));
	}

	setRootIsDecorated(false);
	setModel(new BehaviorListModel(mods));
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
