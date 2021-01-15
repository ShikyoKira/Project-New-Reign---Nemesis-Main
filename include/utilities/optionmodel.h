#pragma once

#include <unordered_set>

#include "utilities/option.h"

namespace nemesis
{
    struct Line;
    struct TemplateClass;
    struct OptionModelList;

    struct OptionModel : public std::enable_shared_from_this<OptionModel>
    {
    private:
        std::string name;
        std::string full;
        std::filesystem::path filepath;

        bool bArray = false;

        std::unordered_set<uint> varpoints;
        std::unordered_set<std::string> varnameset;
        std::unordered_map<std::string, VecStr> varblocks;
        std::vector<std::string> variables;
        std::string novarline;

        std::vector<std::shared_ptr<nemesis::OptionModel>> linkedmodel;

        const nemesis::TemplateClass& templateclass;

        void SetName(const nemesis::Line& modelinfo);
        void AddVarBlock(const VecStr& additioninfo);
        void TryAddVariables(const nemesis::Line& modelinfo);
        void AddLink(std::shared_ptr<nemesis::OptionModel> modelptr);

    public:
        OptionModel(const nemesis::Line& modelinfo,
                    std::filesystem::path& _filepath,
                    const nemesis::TemplateClass& _templateclass);

        std::string GetName() const noexcept;
        std::string GetFullName() const noexcept;
        bool IsArray() const noexcept;
        bool HasVariable(const std::string& var) const noexcept;

        std::unique_ptr<nemesis::Option> CreateOption(const std::string& query) const noexcept;

    private:
        static bool HasArraySyntax(const std::string& line) noexcept;

        friend nemesis::OptionModelList;
	};
}
