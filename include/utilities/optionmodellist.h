#pragma once

#include <unordered_set>
#include <map>

#include "utilities/optionmodel.h"

namespace nemesis
{
    struct Option;

    struct OptionModelList
    {
        class Parser
        {
        public:
            using FuncPtr = void(nemesis::OptionModelList::Parser::*)(const nemesis::Line&);

        private:
            OptionModelList& host;
            VecNstr storeline;
            VecNstr cacheadditionline;
            VecStr cachecompulsory;
            std::vector<VecStr> cachelink;
            std::pair<std::string, std::string> cacherules;

            static const VecStr fixedoptions;
            static const std::unordered_map<std::string, FuncPtr> mapfunc;

            nemesis::Line TryRemoveComment(const nemesis::Line& line);
            std::string GetTemplateClass() const;
            std::filesystem::path GetFilePath() const;

        public:
            void ErrorTemplate(int errcode, int linenum);

            void AddEvent(const nemesis::Line& line);
            void AddVariable(const nemesis::Line& line);
            void AddAnimObject(const nemesis::Line& line);
            void AddOptionLink(const nemesis::Line& line);
            void AddRules(const nemesis::Line& line);
            void AddStartState(const nemesis::Line& line);
            void AddMinimum(const nemesis::Line& line);
            void FlagIgnoreGroup(const nemesis::Line& line);
            void AddCompulsory(const nemesis::Line& line);
            void AddCore(const nemesis::Line& line);
            void AddAddition(const nemesis::Line& line);
            void AddOption(const nemesis::Line& line);
            void LinkedOption();
            void VariableAddition();
            void AddFixedOptions();

            void ParseLine(const nemesis::Line& line);

            int TryAddMultiRoot(const nemesis::Line& line);

            Parser(OptionModelList& host);

            void Run();
        };

        class Rules
        {
            std::shared_ptr<nemesis::OptionModel> begin;
            std::shared_ptr<nemesis::OptionModel> end;

        public:
            Rules(std::shared_ptr<nemesis::OptionModel> begin, std::shared_ptr<nemesis::OptionModel> end);

            bool HasBegin();
            bool HasEnd();
            std::shared_ptr<nemesis::OptionModel> GetBegin();
            std::shared_ptr<nemesis::OptionModel> GetEnd();
        };

    private:
        const nemesis::TemplateClass& templateclass;

        std::unordered_set<std::shared_ptr<nemesis::OptionModel>> options;
        std::unordered_set<std::shared_ptr<nemesis::OptionModel>> compulsory;
        std::multimap<uint, std::shared_ptr<nemesis::OptionModel>, std::greater<uint>> optionsmap;
        std::filesystem::path filepath;

        VecStr events;
        VecStr variables;

        using BehaviorFile = std::string;
        using NodeId = std::string;
        using State = uint;

        std::unordered_map<BehaviorFile, std::unordered_map<State, NodeId>> multistates;

        std::unique_ptr<uint> iState;
        std::unique_ptr<Rules> rules;
        std::unique_ptr<uint> iMin;
        std::unique_ptr<uint> iAnimObj;
        std::unique_ptr<std::string> corebehavior;
        bool ignore_group = false;

        void ReadOptionListFile();

    public:
        OptionModelList(const std::filesystem::path& filepath,
                        const nemesis::TemplateClass& _templateclass) noexcept;

        uint GetAnimObjectCount() const;
        bool Contains(std::string name) const noexcept;
        std::shared_ptr<nemesis::OptionModel> GetModel(std::string modelname) const noexcept;
        void AddOptionModel(const std::string& modelinfo);

        std::unique_ptr<nemesis::Option> CreateOption(const std::string& query) const noexcept;
    };
}
