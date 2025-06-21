#pragma once

#include "Core/NObject.h"

namespace nemesis
{
    struct ModClass;

    template <typename NObjectType>
	struct ModPatch
    {
    private:
        const nemesis::ModClass& Class;
        std::filesystem::path FilePath;
        UPtr<NObjectType> Content;

    public:
        ModPatch(const nemesis::ModClass& mod_class, UPtr<NObjectType>&& content)
            : Class(mod_class)
        {
            Content  = std::move(content);
            FilePath = Content->GetFilePath();
        }

        const nemesis::ModClass& GetModClass() const
        {
            return Class;
        }
        
        const std::filesystem::path& GetFilePath() const
        {
            return FilePath;
        }

        const NObjectType& GetContent() const
        {
            return *Content;
        }

        static_assert(std::is_base_of_v<nemesis::NObject, NObjectType>,
                      "Non-NObject type is passed to NObjectType");
    };
}
