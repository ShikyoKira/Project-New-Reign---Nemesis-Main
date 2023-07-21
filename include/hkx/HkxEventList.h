#pragma once

#include "hkx/HkxEvent.h"

#include "hkx/base/HavokDataList.h"

namespace nemesis
{
    struct HkxEventList : public nemesis::HavokDataList<nemesis::HkxEvent>
    {
        const nemesis::HkxEvent* GetEvent(const std::string& name) const noexcept;
        const nemesis::HkxEvent* GetEvent(size_t id) const noexcept;
        bool HasEvent(const std::string& name) const noexcept;
        bool HasEvent(size_t id) const noexcept;
    };
}
