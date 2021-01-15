#pragma once

#include <map>

#include "utilities/process.h"

struct AnimThreadInfo;

namespace nemesis
{
    struct TemplateLine;

    struct LineProcess
    {
    private:
        using BlockGroup = std::vector<std::unique_ptr<nemesis::Process>>;

        std::map<uint, BlockGroup> blockGroupBySize;
        std::weak_ptr<TemplateLine> pTemplateLine;

        void BlockScopeValidation(const nemesis::Process& process) const;

    public:
        LineProcess() = default;
        bool IsEmpty();
        void AddTemplateLinePtr(std::shared_ptr<TemplateLine> pTemplateLine);
        void AddProcess(std::unique_ptr<nemesis::Process>& process);

    private:
        static bool IsBlockOutOfScope(const nemesis::Process& process, uint begin, uint end);
    };
}
