#pragma once

#include "hkx/eventid.h"

#include "base/havokdatalist.h"

namespace nemesis
{
    struct EventList : public HavokDataList<EventId>
    {
        const EventId* GetEvent(const std::string& name) const noexcept;
        const EventId* GetEvent(size_t id) const noexcept;
        bool HasEvent(const std::string& name) const noexcept;
        bool HasEvent(size_t id) const noexcept;
    };
}
