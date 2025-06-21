#include "Core/ParsingForEachScope.h"

nemesis::ParsingForEachScope::ParsingForEachScope(const nemesis::ForEachStatement& statement,
                                                  nemesis::SemanticManager& manager)
{
    Manager        = &manager;
    Type           = statement.GetType();
    Expression_Ptr = &statement.GetExpression();
    std::string key;
    Manager->AddForEachToQueue(*Expression_Ptr);

    switch (Type)
    {
        case nemesis::ForEachStatement::REQUEST:
        {
            Manager->TryAddRequestToQueue(*Expression_Ptr);
            break;
        }
        case nemesis::ForEachStatement::OPTION:
        {
            statement.TryGetOptionName(key);
            Manager->TryAddOptionToQueue(key, *Expression_Ptr);
            break;
        }
        case nemesis::ForEachStatement::MAP:
        {
            statement.TryGetMapKey(key);
            Manager->TryAddMapToQueue(key);
            break;
        }
        case nemesis::ForEachStatement::MOTION_DATA:
        {
            Manager->AddMotionDataToQueue();
            break;
        }
        case nemesis::ForEachStatement::ROTATION_DATA:
        {
            Manager->AddRotationDataToQueue();
            break;
        }
        default:
            break;
    }
}

nemesis::ParsingForEachScope::~ParsingForEachScope() noexcept
{
    Manager->RemoveTopForEachFromQueue(*Expression_Ptr);

    switch (Type)
    {
        case nemesis::ForEachStatement::REQUEST:
        {
            Manager->TryRemoveLastRequest();
            break;
        }
        case nemesis::ForEachStatement::OPTION:
        {
            Manager->TryRemoveLastOption();
            break;
        }
        case nemesis::ForEachStatement::MAP:
        {
            Manager->TryRemoveLastMap();
            break;
        }
        case nemesis::ForEachStatement::MOTION_DATA:
        {
            Manager->TryRemoveLastMotionData();
            break;
        }
        case nemesis::ForEachStatement::ROTATION_DATA:
        {
            Manager->TryRemoveLastRotationData();
            break;
        }
        default:
            break;
    }
}
