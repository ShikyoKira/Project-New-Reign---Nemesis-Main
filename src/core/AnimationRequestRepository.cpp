#include "core/AnimationRequestRepository.h"

nemesis::AnimationRequestCollection nemesis::AnimationRequestRepository::Proxy;

void nemesis::AnimationRequestRepository::AddRequest(UPtr<nemesis::AnimationRequest>&& request)
{
    Requests[std::string(request->GetTemplateName())].emplace_back(std::move(request));
}

const nemesis::AnimationRequestCollection&
nemesis::AnimationRequestRepository::GetRequests(const std::string& template_name) const
{
    auto itr = Requests.find(template_name);
    return itr == Requests.end() ? Proxy : itr->second;
}
