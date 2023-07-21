#pragma once

#include <functional>

#include "utilities/types.h"
#include "utilities/StringMatchTrie.h"

namespace nemesis
{
    struct LineModifier;
    struct SemanticManager;

	struct LineModifierFactory
    {
    private:
        struct ModifierBuilderBase
        {
            virtual UPtr<nemesis::LineModifier> Build(size_t begin,
                                                      size_t end,
                                                      const std::string& component,
                                                      size_t linenum,
                                                      const std::filesystem::path& filepath,
                                                      const nemesis::SemanticManager& manager)
                = 0;
        };

        template<typename LineModifierType>
        struct ModifierBuilder : ModifierBuilderBase
        {
            UPtr<nemesis::LineModifier> Build(size_t begin,
                                              size_t end,
                                              const std::string& component,
                                              size_t linenum,
                                              const std::filesystem::path& filepath,
                                              const nemesis::SemanticManager& manager) override
            {
                return std::make_unique<LineModifierType>(
                    begin, end, component, linenum, filepath, manager);
            }

            static_assert(std::is_base_of_v<nemesis::LineModifier, LineModifierType>,
                          "Non-LineModifier type is passed to LineModifierType");
        };

        struct ModifierBuilderCollection
        {
            UMap<std::string, UPtr<ModifierBuilderBase>> FirstBuilders;
            UMap<std::string, UPtr<ModifierBuilderBase>> LastBuilders;
            
            ModifierBuilderCollection();
        };

        LineModifierFactory() = default;

    public:
        static Map<size_t, Vec<UPtr<nemesis::LineModifier>>>
        BuildModifiers(const std::string& line,
                       size_t linenum,
                       const std::filesystem::path& filepath,
                       const nemesis::SemanticManager& manager);
        static UPtr<nemesis::LineModifier> BuildModifier(size_t begin,
                                                         size_t end,
                                                         const std::string& component,
                                                         const std::string& line,
                                                         size_t linenum,
                                                         const std::filesystem::path& filepath,
                                                         const nemesis::SemanticManager& manager);
    };
}
