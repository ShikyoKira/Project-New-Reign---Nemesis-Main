#ifndef BEHAVIORLISTMODEL_H_
#define BEHAVIORLISTMODEL_H_

#include <QAbstractItemModel>
#include <QDataStream>
#include <QIODevice>
#include <QMimeData>
#include <QObject>

#include <chrono>

#include "ui/BehaviorInfo.h"

namespace nemesis
{
    struct ModInfoManager;
}

class BehaviorListModel : public QAbstractItemModel
{
    Q_OBJECT
private:
    QList<BehaviorInfo> behaviorList;
    Qt::CheckState tempCheck                                  = Qt::CheckState::Unchecked;
    std::chrono::high_resolution_clock::time_point click_time = std::chrono::high_resolution_clock::now();
    bool m_DropOnItems                                        = true;
    int draggedIndex                                          = 0;

    nemesis::ModInfoManager* modinfo_manager = nullptr;

public:
    enum EColumn
    {
        COL_NAME,
        COL_AUTHOR,
        COL_PRIORITY,

        COL_LASTCOL = COL_PRIORITY
    };

    BehaviorListModel(const QList<BehaviorInfo>& list, QObject* parent = nullptr)
        : QAbstractItemModel(parent)
        , behaviorList(list)
    {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList& indexes) const;
    bool dropMimeData(
        const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);

    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool insertRows(int position, int rows, const QModelIndex& index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex());

    void setModInfoManager(nemesis::ModInfoManager* modinfo_manager);

    void createModSelectionCache();
    void createModOrderCache();

public slots:
    void dropModeUpdate(bool dropOnItems);
    void goToUrl(const QModelIndex& index);
};

#endif
