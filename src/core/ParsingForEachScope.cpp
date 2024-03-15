#include "core/ParsingForEachScope.h"

nemesis::ParsingForEachScope::ParsingForEachScope(const nemesis::ForEachStatement& statement,
                                                  nemesis::SemanticManager& manager)
{
    Manager = &manager;

    std::string option_name;
    
    if (statement.TryGetOptionName(option_name))
    {
        IsOption = true;
        Manager->TryAddOptionToQueue(option_name, statement.GetExpression());
        return;
    }

    Manager->TryAddRequestToQueue(statement.GetExpression());
    IsOption = false;
}

nemesis::ParsingForEachScope::~ParsingForEachScope()
{
    if (IsOption)
    {
        Manager->TryRemoveLastOption();
        return;
    }

    Manager->TryRemoveLastRequest();
}
