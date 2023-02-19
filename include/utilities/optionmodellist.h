#pragma once

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
            VecNstr cachecompulsory;
            Vec<VecStr> cachelink;
            Pair<std::string, std::string> cacherules;

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
            void AddOptionModel(const nemesis::Line& line);

            void Commit();
            void CommitLinkedOption();
            void CommitVariableAddition();
            void CommitCompulsoryVariable();

            void AddFixedOptions();

            void ParseLine(const nemesis::Line& line);

            int TryAddMultiRoot(const nemesis::Line& line);

            Parser(OptionModelList& host);

            void Run();

        private:
            static const VecStr fixedoptions;
            static const UMap<std::string, FuncPtr> mapfunc;
            const std::string animobj_prefix = "AnimObject/";
        };

        class Rules
        {
            SPtr<nemesis::OptionModel> begin;
            SPtr<nemesis::OptionModel> end;

        public:
            Rules(SPtr<nemesis::OptionModel> begin, SPtr<nemesis::OptionModel> end);

            bool HasBegin();
            bool HasEnd();
            SPtr<nemesis::OptionModel> GetBegin();
            SPtr<nemesis::OptionModel> GetEnd();
        };

    private:
        const nemesis::TemplateClass& templateclass;

        USetStr options;
        USetStr compulsory;
        std::multimap<size_t, SPtr<nemesis::OptionModel>, std::greater<size_t>> optionsmap;
        std::filesystem::path filepath;

        VecStr events;
        VecStr variables;

        using BehaviorFile = std::string;
        using NodeId = std::string;
        using State = size_t;

        UMap<BehaviorFile, UMap<State, NodeId>> multistates;

        UPtr<size_t> iState;
        UPtr<Rules> rules;
        UPtr<size_t> iMin;
        UPtr<size_t> iAnimObj;
        UPtr<std::string> corebehavior;
        bool ignore_group = false;

        void ReadOptionListFile();
        void AddOptionModel(const nemesis::Line& option);
        void SortOptionsByLength();

        nemesis::OptionModel* GetInnerModel(const std::string modelname) const noexcept;

    public:
        OptionModelList(const std::filesystem::path& filepath,
                        const nemesis::TemplateClass& _templateclass) noexcept;

        size_t GetAnimObjectCount() const;
        bool Contains(const std::string& name) const;
        nemesis::OptionModel* GetModel(const std::string& modelname) noexcept;
        const nemesis::OptionModel* GetModel(const std::string& modelname) const noexcept;
        bool IsCompulsoryMet(const VecStr& optionlist) const noexcept;
        Vec<const OptionModel*> GetOptionList() const;

        void InjectOptionModel(const std::string& modelinfo);

        UPtr<nemesis::Option> CreateOption(const std::string& name,
                                           const nemesis::AnimQuery& animquery) const noexcept;

    private:
        const std::string motion_str   = "motion";
        const std::string rotation_str = "rotation";
    };
}
