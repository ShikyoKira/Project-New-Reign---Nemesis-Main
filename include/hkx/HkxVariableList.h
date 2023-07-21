#pragma once

#include "hkx/HkxVariable.h"

#include "hkx/base/HavokDataList.h"

namespace nemesis
{
    struct HkxVariableList : public nemesis::HavokDataList<nemesis::HkxVariable>
    {
        const nemesis::HkxVariable* GetVariable(const std::string& name) const noexcept;
        const nemesis::HkxVariable* GetVariable(size_t id) const noexcept;
        bool HasVariable(const std::string& name) const noexcept;
        bool HasVariable(size_t id) const noexcept;
    };
}
