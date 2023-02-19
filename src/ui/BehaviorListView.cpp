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
#include "ui/NemesisEngine.h"
#include "ui/BehaviorListView.h"

extern std::atomic<size_t> resizeCount;

BehaviorListView::BehaviorListView(QWidget* parent)
    : QTreeView(parent)
    , m_Scrollbar(new ScrollBar(this->model(), this))
{
}

BehaviorListView::BehaviorListView(NemesisEngine* parent)
    : QTreeView(parent)
    , m_Scrollbar(new ScrollBar(this->model(), this))
{
    setVerticalScrollBar(m_Scrollbar);
    QList<BehaviorInfo> mods;
    UMap<std::string, bool> chosenBehavior;
    VecWstr order_list;

    if (GetOrderCache(order_list) && GetModSelectionCache(chosenBehavior))
    {
        std::set<std::wstring> tempset;

        for (auto& modcode : order_list)
        {
            auto* modinfo_ptr = parent->modinfo_manager.GetModInfo(modcode);

            if (!modinfo_ptr) continue;

            mods.push_back(*new BehaviorInfo(QString::fromStdWString(modinfo_ptr->GetDisplayName()),
                                             QString::fromStdWString(modinfo_ptr->GetAuthor().data())));
            tempset.insert(modcode);

            if (chosenBehavior.find(modinfo_ptr->GetDisplayNameA()) == chosenBehavior.end()) continue;

            mods.back().state = Qt::Checked;
        }

        for (auto& modinfo : parent->modinfo_manager.GetModInfoList())
        {
            if (tempset.find(std::wstring(modinfo.GetModCode())) != tempset.end()) continue;

            mods.push_back(*new BehaviorInfo(QString::fromStdWString(modinfo.GetDisplayName()),
                                             QString::fromStdWString(modinfo.GetAuthor().data())));
        }
    }
    else
    {
        for (auto& modinfo : parent->modinfo_manager.GetModInfoList())
        {
            mods.push_back(*new BehaviorInfo(QString::fromStdWString(modinfo.GetDisplayName()),
                                             QString::fromStdWString(modinfo.GetAuthor().data())));
        }
    }

    setRootIsDecorated(false);
    auto bhvmodel = new BehaviorListModel(mods);
    bhvmodel->setModInfoManager(&parent->modinfo_manager);
    setModel(bhvmodel);

    connect(header(), &QHeaderView::sectionResized, this, &BehaviorListView::resizeHeader);
    connect(this, &QAbstractItemView::doubleClicked, bhvmodel, &BehaviorListModel::goToUrl);
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
        priorityWidth = newSize;
        nemesisInfo->SetPriorityWidth(newSize);
    }
    else if (logicalIndex == 1)
    {
        nemesisInfo->SetAuthorWidth(newSize);
    }
    else
    {
        nemesisInfo->SetModNameWidth(newSize);
    }
}
