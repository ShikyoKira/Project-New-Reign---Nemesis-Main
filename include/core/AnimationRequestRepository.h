#pragma once

#include "core/AnimationRequestCollection.h"

namespace nemesis
{
	struct AnimationRequestRepository
    {
    private:
        static nemesis::AnimationRequestCollection Proxy;
        Map<std::string, nemesis::AnimationRequestCollection> Requests;

    public:
        void AddRequest(UPtr<nemesis::AnimationRequest>&& request);

        const nemesis::AnimationRequestCollection& GetRequests(const std::string& template_name) const;
    };
}
