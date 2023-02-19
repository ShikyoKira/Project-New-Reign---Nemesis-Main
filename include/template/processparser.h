#pragma once

#include "base/importerbase.h"

#include "core/preprocessline.h"
#include "core/linkedpreprocessline.h"

namespace nemesis
{
    struct ProcessParser
    {
        using FuncPtr = void (nemesis::Process::*)(VecStr&, nemesis::ScopeInfo&) const;
        using RtnFunc = std::function<std::string(nemesis::ScopeInfo&)>;

    private:
        struct Position
        {
            size_t begin;
            size_t end;

            Position(nemesis::regex_iterator& itr, size_t index = 0);
            Position(nemesis::AnimVarPtr::Lexer::Iterator& itr);
            Position(size_t begin, size_t end);

            bool IsWithin(const Position& itr);
        };

        const nemesis::Line* lineptr;
        nemesis::ImporterBase& importer;

        SPtr<nemesis::LineProcess> lineprocessptr;

        public:
        Vec<UPtr<nemesis::Process>> processlist;

        void Reset();
        void StageProcess(UPtr<nemesis::Process>& process);
        nemesis::regex GetNodeIdRegex();
        nemesis::regex GetGroupNodeIdRegex();

        bool HasNemesisSyntax();
        void TryCountAnimation();
        void TryAddValidNodeID();

        bool TryStartCountingElement();
        bool TryCountingElement();
        bool TryStopCountingElement();
        bool IsCountingElement();

        bool TryRegisterAnimation();
        bool TryRegisterBehavior();

        void AutoEndAnim();
        void AutoIdUpdate();

        void NodeIdRegister();

        void StartCountingElement();
        void StopCountingElement();

        void TryCountState();

        void TryRegisterMultiChoice();

        void TryOptionAddVariable();
        void TryAddEventIdProcessor();
        void TryAddVariableIdProcessor();
        void TryAddAnimObject();
        void TryAddImportTemplateRef();

        void TryAddProcessWithLexerList();

        void TryAnimVarProcessor(const std::string& keyword, FuncPtr func);

        void TryGetImport(const std::string& line,
                          Vec<Pair<nemesis::ProcessParser::Position, int>>& importindex_list,
                          Vec<nemesis::ProcessParser::Position>& importnode_list);

        //void AddMainAnimEventProcessor(nemesis::AnimVarPtr::Lexer::Iterator& itr);
        //void AddFilePathProcessor(nemesis::AnimVarPtr::Lexer::Iterator& itr);
        void AddProcessByLexerIterator(nemesis::AnimVarPtr::Lexer::Iterator& itr, FuncPtr callback);
        void AddProcessByLexerIterator(nemesis::AnimVarPtr::Lexer::Iterator& itr,
                                       FuncPtr callback,
                                       RtnFunc selector);

        bool IsWithinAny(Position& pos, Vec<Position> installed_positions);

        void (nemesis::Process::*ptr)(VecStr&, nemesis::ScopeInfo&) const;
    public:
        ProcessParser(nemesis::ImporterBase& parser);

        void ToLine(nemesis::PreprocessLine& line);
        void SetLine(const nemesis::Line& lineref);
        void Parse();

        bool IsEmpty() const;

    private:
        static std::string HkxParamToRgxStr(std::string name);

        static UPtr<nemesis::Process> GenerateProcess(FuncPtr func);
        static UPtr<nemesis::Process> GenerateProcess(Position pos, FuncPtr func);
        static UPtr<nemesis::Process> GenerateProcess(size_t begin, size_t end, FuncPtr func);

        static nemesis::regex relative_rgx;
        static nemesis::regex time_rgx;
        static nemesis::regex nodeid_rgx;
        static nemesis::regex bhv_rgx;
        static nemesis::regex anim_rgx;
        static nemesis::regex import_rgx;
        static nemesis::regex multichoice_rgx;
        static nemesis::regex process_rgx;

        static UMap<std::string, nemesis::regex> idregex_byclass;
        static UMap<std::string, nemesis::regex> groupidregex_byclass;

    public:
        const char tab                 = '\t';
        const char process_indicator   = '$';
        const std::string count_syntax = "%$%";
        const std::string mc_syntax    = "$MC$";
        const std::string import_str   = "$import[";
        const std::string openParam    = "<hkparam name=\"";
        const std::string closeParam   = "</hkparam>";
        const std::string openObject   = "<hkobject>";
        const std::string refIdElement = "\t\t\t#";
        const std::string refIdPrefix  = "#";
        const std::string numelements  = "numelements=\"";
        const std::string computeFunc  = "<!-- COMPUTE -->";
        const std::string nodeidSuffix = "~([0-9]+)";
        const std::string grpidSuffix  = "~([0-9]+)";
        const std::string mae_str      = "main_anim_event";
        const std::string filepath_str = "FilePath";
        const std::string animobj_str  = "AnimObject/";
        const std::string nodeidPrefix = "#NID~";
    };
}
