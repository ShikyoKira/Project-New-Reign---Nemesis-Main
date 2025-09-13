#pragma once

#include <sstream>

#include "Nemesis_Core_Engine/include/Utilities/Types.h"
#include "Nemesis_Core_Engine/include/Utilities/NonCopyable.h"

#include "Core/AnimationRequest.h"

#include "Utilities/Sha256.h"

namespace nemesis
{
    struct AnimationRequestCollection : public nemesis::NonCopyableStruct
    {
    private:
        Vec<UPtr<nemesis::AnimationRequest>> uRequestList;
        Vec<const nemesis::AnimationRequest*> RequestList;

    public:
        std::string get_hash() const
        {
            std::ostringstream oss;

            for (auto& request : RequestList)
            {
                oss << request->GetHash();
            }

            return nemesis::SHA256::hex(oss.str());
        }

        template <class... _Valty>
        decltype(auto) emplace_back(_Valty&&... _Val)
        {
            return RequestList.emplace_back(uRequestList.emplace_back(std::forward<_Valty>(_Val)...).get());
        }

        const nemesis::AnimationRequest* operator[](size_t index) const noexcept
        {
            return RequestList[index];
        }

        const nemesis::AnimationRequest* at(size_t index) const
        {
            return RequestList.at(index);
        }

        std::vector<const nemesis::AnimationRequest*>::const_iterator begin() const noexcept
        {
            return RequestList.begin();
        }
        
        std::vector<const nemesis::AnimationRequest*>::const_iterator end() const noexcept
        {
            return RequestList.end();
        }

        bool empty() const noexcept
        {
            return RequestList.empty();
        }

        size_t size() const noexcept
        {
            return RequestList.size();
        }
    };
}
