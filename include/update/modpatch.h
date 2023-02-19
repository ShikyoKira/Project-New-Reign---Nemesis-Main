#pragma once

#include "patch.h"

namespace nemesis
{
	struct ModPatch : public nemesis::Patch
    {
        struct ModPatchParser : nemesis::Patch::PatchParser
        {
            ModPatchParser(nemesis::ModPatch& patch);

            void ReadFile(const std::filesystem::path& filepath) override;
        };

        ModPatch(const std::string& code, const std::filesystem::path& filepath);

        nemesis::CondType GetConditionType() const override;
    };
}
