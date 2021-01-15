#pragma once

#include <vector>

#include "utilities/linkedvar.h"
#include "utilities/templateline.h"

namespace nemesis
{
    struct Process;
    struct TemplateClass;

    struct Template : public std::enable_shared_from_this<Template>
    {
        class Parser
        {
            class LineParser
            {
            private:
                // Persistences at line level
                // They only last as long as the LINE being parsed
                const nemesis::Line& line;
                Parser& parser;
                std::shared_ptr<nemesis::LineProcess> pLineProcess;
                std::vector<std::unique_ptr<nemesis::Process>> processlist;

                void StagingProcess(std::unique_ptr<nemesis::Process>& process);
                void ResetStagingProcess();

            public:
                LineParser(const nemesis::Line& line, Parser& parser);

                std::shared_ptr<nemesis::LineProcess> GetLineProcess();
                void CommitProcess(const nemesis::Line& line);

                bool HasNemesisSyntax();
                void TryCountAnimation();
                void TryAddValidNodeID();

                bool TryStartCountingElement();
                bool TryCountingElement();
                bool TryStopCountingElement();
                bool IsCountingElement() const;

                bool TryRegisterAnimation();
                bool TryRegisterBehavior();

                void AutoEndAnim();
                void AutoIdUpdate();

                void NodeIdRegister();

                void StartCountingElement();
                void StopCountingElement();

            private:
                using funcptr = void (nemesis::Process::*)(VecStr&, const AnimThreadInfo&) const;

                static std::string HkxParamToRgxStr(std::string name);

                static std::unique_ptr<nemesis::Process> GenerateProcess(funcptr func);
                static std::unique_ptr<nemesis::Process> GenerateProcess(uint begin, uint end, funcptr func);
            };

        private:
            // Persistences at template level
            // They only last as long as the TEMPLATE being parsed
            nemesis::Template& rTemplate;
            bool bCountElement = false;
            bool bEnding       = false;
            int iElementRange  = false;
            std::shared_ptr<LineParser> pLineParser;

            bool IsTemplateImport() const noexcept;
            const std::string& GetTemplateClassName() const;
            const std::filesystem::path& GetFilePath() const noexcept;
            void ParseLines(const VecNstr& storeline);
            void ParseLine(const nemesis::Line& line);

        public:
            Parser(nemesis::Template& host, const std::filesystem::path& filepath);
            Parser(nemesis::Template& host, const VecNstr& storeline);
        };

    private:
        std::vector<std::unique_ptr<nemesis::TemplateLine>> contents;
        std::weak_ptr<const nemesis::TemplateClass> pTemplateClass;
        std::wstring behaviorfile;
        std::filesystem::path filepath;
        bool bGroup = false;
        bool bMaster = false;
        bool bImport = false;

        void SetBehaviorFile(const std::filesystem::path& behaviorfile);
        void SetFilePath(const std::filesystem::path& behaviorfile);

    public:
        Template() noexcept;    // for import template
        Template(const nemesis::TemplateClass& templateclass) noexcept;     // for common template

        bool IsGroup() const noexcept;
        bool IsMaster() const noexcept;
        bool IsImport() const noexcept;
        const std::string& GetTemplateClassName() const;
        const std::filesystem::path& GetFilePath() const noexcept;
        const std::wstring& GetBehaviorFileW() const noexcept;

        void ReadFile(const std::filesystem::path& filepath);
        void AddLine(std::unique_ptr<nemesis::TemplateLine>& pline);
    };
}
