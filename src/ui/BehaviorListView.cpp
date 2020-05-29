#include <QHeaderView>
#include <QMessageBox>
#include <QMimeData>
#include <QObject>
#include <QProxyStyle>
#include <QUrl>

#include "Global.h"
#include "nemesisinfo.h"

#include "ui/UiModInfo.h"
#include "ui/ErrorMsgBox.h"
#include "ui/SettingsSave.h"
#include "ui/BehaviorListView.h"

extern std::atomic<uint> resizeCount;

std::unordered_map<std::string, std::string> modConvert;

BehaviorListView::BehaviorListView(QWidget* parent)
    : QTreeView(parent)
    , m_Scrollbar(new ScrollBar(this->model(), this))
{
    setVerticalScrollBar(m_Scrollbar);
    QList<BehaviorInfo> mods;
    VecStr orderList;
    std::wstring errmsg;

    if (!readMod(errmsg))
    {
        CEMsgBox* msgbox = new CEMsgBox;
        QString msg      = QString::fromStdWString(errmsg);
        msgbox->setText(msg);
        msgbox->setWindowTitle("CRITITAL ERROR");
        msgbox->show();
        error = true;
        return;
    }

    if (getOrderCache(orderList))
    {
        std::set<std::wstring> tempset;

        for (auto& each : orderList)
        {
            std::wstring mod = nemesis::transform_to<std::wstring>(each);

            if (modinfo.find(mod) != modinfo.end())
            {
                mods.push_back(*new BehaviorInfo(QString::fromStdWString(modinfo[mod]->getName()),
                                                 QString::fromStdWString(modinfo[mod]->getAuthor())));
                modConvert[modinfo[mod]->getNameA()] = each;
                tempset.insert(mod);
            }
        }

        for (auto& info : modinfo)
        {
            if (tempset.find(info.first) == tempset.end())
            {
                mods.push_back(*new BehaviorInfo(QString::fromStdWString(info.second->getName()),
                                                 QString::fromStdWString(info.second->getAuthor())));
            }
        }
    }
    else
    {
        for (auto& info : modinfo)
        {
            modConvert[info.second->getNameA()] = nemesis::transform_to<std::string>(info.first);
            mods.push_back(*new BehaviorInfo(QString::fromStdWString(info.second->getName()),
                                             QString::fromStdWString(info.second->getAuthor())));
        }
    }

    setRootIsDecorated(false);
    setModel(new BehaviorListModel(mods));

    connect(header(), &QHeaderView::sectionResized, this, &BehaviorListView::resizeHeader);
    connect(this,
            &QAbstractItemView::doubleClicked,
            static_cast<BehaviorListModel*>(model()),
            &BehaviorListModel::goToUrl);
}

void BehaviorListView::dragEnterEvent(QDragEnterEvent* event)
{
    emit dropModeUpdate(event->mimeData()->hasUrls());
    QTreeView::dragEnterEvent(event);
}

void BehaviorListView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
    setVerticalScrollBar(new ScrollBar(model, this));
}

void BehaviorListView::setIni(NemesisInfo* _ini)
{
    nemesisInfo = _ini;
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
