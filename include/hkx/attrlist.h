#pragma once

#include "hkx/attrid.h"

#include "base/havokdatalist.h"

namespace nemesis
{
    struct AttrList : public HavokDataList<AttrId>
    {
        const AttrId* GetVariable(const std::string& name) const noexcept;
        const AttrId* GetVariable(size_t id) const noexcept;
        bool HasVariable(const std::string& name) const noexcept;
        bool HasVariable(size_t id) const noexcept;
    };
} // namespace nemesis
