#pragma once

#include "hkx/varid.h"

#include "base/havokdatalist.h"

namespace nemesis
{
    struct VarList : public HavokDataList<VarId>
    {
        const VarId* GetVariable(const std::string& name) const noexcept;
        const VarId* GetVariable(size_t id) const noexcept;
        bool HasVariable(const std::string& name) const noexcept;
        bool HasVariable(size_t id) const noexcept;
    };
} // namespace nemesis
