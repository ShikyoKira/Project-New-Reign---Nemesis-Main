#pragma once

#include "utilities/regex.h"
#include "utilities/option.h"

namespace nemesis
{
    struct Line;
    struct TemplateClass;
    struct OptionModelList;

    struct OptionModel : public std::enable_shared_from_this<OptionModel>
    {
        struct Variable
        {
        private:
            UPtr<std::string> defaultvalue;
            std::string name;

            void AddName(const std::string& name);
            void AddDefaultValue(const std::string& value);

        public:
            Variable(std::string variable);

            std::string_view GetName() const;
            std::string_view GetDefaultValue() const;
            bool HasDefault() const;
        };

    private:
        std::string name;
        std::string full;
        std::filesystem::path filepath;

        bool bArray = false;
        bool bErrorCheck = true;

        nemesis::regex rgx;
        Vec<Variable> variables;
        UMap<std::string, VecStr> varblocks;

        Vec<nemesis::OptionModel*> linkedmodel;

        const nemesis::TemplateClass& templateclass;

        void AddModelInfoNoCheck(const nemesis::Line& modelinfo);
        void SetName(const nemesis::Line& modelinfo);
        void SetNameNoCheck(const nemesis::Line& modelinfo);
        void AddVarBlock(const VecStr& additioninfo);
        void TryAddVariables(const nemesis::Line& modelinfo);
        void AddLink(nemesis::OptionModel* modelptr);

    public:
        OptionModel(const nemesis::Line& modelinfo,
                    std::filesystem::path& _filepath,
                    const nemesis::TemplateClass& _templateclass);
        OptionModel(std::filesystem::path& _filepath,
                    const nemesis::TemplateClass& _templateclass);

        std::string_view GetName() const noexcept;
        std::string_view GetFullName() const noexcept;
        bool IsArray() const noexcept;
        bool HasVariable() const noexcept;
        bool HasVariable(const std::string& name) const noexcept;

        const nemesis::OptionModel::Variable* GetVariablePtr(const std::string name) const noexcept;
        Vec<const nemesis::OptionModel::Variable*> GetVariablesList() const;

        void AddModelInfo(const nemesis::Line& modelinfo);

        UMap<std::string, std::string> ParseVariables(const std::string& query) const noexcept;

        UPtr<nemesis::Option> CreateOption(const std::string& query,
                                           const nemesis::AnimQuery& animquery) const noexcept;

    private:
        static bool HasArraySyntax(const std::string& line) noexcept;

        static const nemesis::regex nameexclusion_rgx;

        friend nemesis::OptionModelList;
	};
}
