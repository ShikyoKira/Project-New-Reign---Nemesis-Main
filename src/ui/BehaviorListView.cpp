#include <QUrl>
#include <QObject>
#include <QMimeData>
#include <QProxyStyle>
#include <QMessageBox>
#include <QHeaderView>

#include "header/ui/ErrorMsgBox.h"
#include "header/ui/SettingsSave.h"
#include "header/ui/BehaviorListView.h"

std::unordered_map<std::string, std::string> modConvert;
extern std::atomic<unsigned int> resizeCount;

BehaviorListView::BehaviorListView(QWidget *parent) : QTreeView(parent), m_Scrollbar(new ScrollBar(this->model(), this))
{
	setVerticalScrollBar(m_Scrollbar);
	QList<BehaviorInfo> mods;
	std::vector<std::string> orderList;
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

	if (getOrderCache(orderList))
	{
		std::set<std::string> tempset;

		for (auto& each : orderList)
		{
			if (modinfo.find(each) != modinfo.end())
			{
				mods.push_back(*new BehaviorInfo(QString::fromStdString(modinfo[each][0]), QString::fromStdString(modinfo[each][1])));
				modConvert[modinfo[each][0]] = each;
				tempset.insert(each);
			}
		}

		for (auto& info : modinfo)
		{
			if (tempset.find(info.first) == tempset.end())
			{
				mods.push_back(*new BehaviorInfo(QString::fromStdString(info.second[0]), QString::fromStdString(info.second[1])));
			}
		}
	}
	else
	{
		for (auto& info : modinfo)
		{
			modConvert[info.second[0]] = info.first;
			mods.push_back(*new BehaviorInfo(QString::fromStdString(info.second[0]), QString::fromStdString(info.second[1])));
		}
	}

	setRootIsDecorated(false);
	setModel(new BehaviorListModel(mods));

	connect(header(), &QHeaderView::sectionResized, this, &BehaviorListView::resizeHeader);
	connect(this, &QAbstractItemView::doubleClicked, static_cast<BehaviorListModel*>(model()), &BehaviorListModel::goToUrl);
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
