#pragma once

#include "utilities/types.h"

namespace nemesis
{
    struct OptionModel;
    struct AnimQuery;
    struct AnimationRequest;

    struct Option
    {
        struct Model
        {

        };

    private:
        std::string name;
        std::string query;
        UMap<std::string, std::string> variables;

        bool success = false;

        const nemesis::AnimationRequest* Request;
        const nemesis::AnimQuery* animquery;
        const nemesis::OptionModel& model;

    public:
        Option(const std::string& query,
               const nemesis::OptionModel& model,
               const nemesis::AnimationRequest& request) noexcept;
        
        Option(const std::string& query,
               const nemesis::OptionModel& model,
               const nemesis::AnimQuery& animquery) noexcept;

        const nemesis::AnimationRequest& GetAnimationRequest() const;
        const nemesis::AnimQuery& GetAnimQuery() const;
        std::string GetName() const noexcept;
        const nemesis::OptionModel& GetModel() const noexcept;
        std::string GetVariable(const std::string& key) const;

        bool IsSuccess() const noexcept;
    };
}
