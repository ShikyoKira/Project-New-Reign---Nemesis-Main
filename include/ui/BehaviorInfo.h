#ifndef BEHAVIORINFO_H_
#define BEHAVIORINFO_H_

#include <vector>
#include <QObject>

struct BehaviorInfo
{
    QString modname;
    QString author;
    Qt::CheckState state = Qt::Unchecked;

    BehaviorInfo()
    {}
    BehaviorInfo(const BehaviorInfo& other);
    BehaviorInfo(const QString name, const QString modauthor);
};

Q_DECLARE_METATYPE(BehaviorInfo);

#endif