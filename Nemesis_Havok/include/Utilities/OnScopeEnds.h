#pragma once

#include <functional>

namespace nemesis
{
	struct OnScopeEnds
    {
    private:
        std::function<void()> OnDeconstruct;

    public:
        OnScopeEnds(const std::function<void()>& on_end)
            : OnDeconstruct(on_end)
        {
        }

        ~OnScopeEnds()
        {
            OnDeconstruct();
        }
    };
}
