#pragma once

#include "hkx/HkxAttribute.h"

#include "hkx/base/HavokDataList.h"

namespace nemesis
{
    struct HkxAttributeList : public nemesis::HavokDataList<nemesis::HkxAttribute>
    {
        const nemesis::HkxAttribute* GetVariable(const std::string& name) const noexcept;
        const nemesis::HkxAttribute* GetVariable(size_t id) const noexcept;
        bool HasVariable(const std::string& name) const noexcept;
        bool HasVariable(size_t id) const noexcept;
    };
}
