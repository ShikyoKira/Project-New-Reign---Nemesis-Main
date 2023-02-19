#pragma once

#include "patch.h"

namespace nemesis
{
    struct TemplatePatch : public nemesis::Patch
    {
        struct TemplatePatchParser : nemesis::Patch::PatchParser
        {
            TemplatePatchParser(nemesis::TemplatePatch& patch);

            void ReadFile(const std::filesystem::path& filepath) override;
        };

    private:
        std::filesystem::path datapath;

    public:
        TemplatePatch(const std::string& code,
                      const std::filesystem::path& filepath,
                      const std::filesystem::path& datapath);

        nemesis::CondType GetConditionType() const override;
        std::filesystem::path GetDataPath() const;
    };
}
