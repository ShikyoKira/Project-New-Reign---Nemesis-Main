#pragma once

#include <functional>

#include "ExAnimation/ExAnimationRequest.h"

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
    struct CompileState;

    struct LinkedExAnimList
    {
    private:
        struct ExAnimConfig
        {
            std::string Variable;
            short Order;
            std::filesystem::path AnimPath;
        };

        UPtr<nemesis::LinkedExAnimList::ExAnimConfig> Config;
        UPtr<nemesis::LinkedExAnimList> Next;

        void
        CompileAsXmlTo(VecStr& lines,
                       std::string& node_id,
                       size_t count,
                       nemesis::CompileState& state,
                       const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)>&
                           add_clip_func) const;

        void SetupConfig(const nemesis::ExAnimationRequest& request);

        LinkedExAnimList(nemesis::LinkedExAnimList& linked_ex_anim);

    public:
        LinkedExAnimList(const nemesis::ExAnimationRequest& request);

        void Add(const nemesis::ExAnimationRequest& request);
        VecStr
        CompileAsXml(std::string& node_id,
                     nemesis::CompileState& state,
                     const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)>&
                         add_clip_func) const;
        void
        CompileAsXmlTo(VecStr& lines,
                       std::string& node_id,
                       nemesis::CompileState& state,
                       const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)>&
                           add_clip_func) const;
    };
}