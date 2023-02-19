#pragma once

#include <atomic>

namespace nemesis
{
	struct StateId
    {
    private:
        std::atomic_flag lock{};
        size_t startid;

    public:
        StateId(size_t startid);

        size_t GenerateNewId();
    };
}
