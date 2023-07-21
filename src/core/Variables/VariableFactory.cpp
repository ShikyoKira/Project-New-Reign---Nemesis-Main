#include "core/Variables/Variable.h"
#include "core/Variables/VariableFactory.h"

nemesis::VariableFactory::VariableFactory(const std::string& expression)
{
}

UPtr<nemesis::Variable> nemesis::VariableFactory::Create()
{
    return nullptr;
}
