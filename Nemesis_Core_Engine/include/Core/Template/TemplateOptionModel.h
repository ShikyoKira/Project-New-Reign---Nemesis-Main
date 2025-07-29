#pragma once

#include <filesystem>
#include <functional>

#include "Core/Template/TemplateOption.h"

namespace nemesis
{
    struct TemplateClass;

    struct TemplateOptionModel
    {
    private:
        std::string Name;

        VecStr Aliases;
        VecStr Variables;

        bool bArray;

        void AddVariablesToOption(nemesis::TemplateOption& opt,
                                  const std::string& expr,
                                  size_t linenum,
                                  const std::filesystem::path& filepath) const;
    public:
        TemplateOptionModel(const std::string name,
                            const VecStr& aliases,
                            const VecStr& variables,
                            bool b_array);

        const std::string& GetName() const noexcept;
        const VecStr& GetAliases() const noexcept;
        bool HasAlias(const std::string& alias) const noexcept;
        bool HasVariable(const std::string& variable_name) const noexcept;
        bool IsArray() const noexcept;

        UPtr<nemesis::TemplateOption> TryCreateOption(const std::string& expression, size_t linenum, const std::filesystem::path& filepath) const;
    };
}
