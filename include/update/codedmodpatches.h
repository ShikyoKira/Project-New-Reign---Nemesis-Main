#pragma once

#include <string>
#include <filesystem>

#include "utilities/types.h"

#include "update/animdata/animdatacond.h"

namespace nemesis
{
	struct CodedModPatches
    {
        std::string code;

        Vec<std::filesystem::path> patches;
        UPtr<MasterAnimData::ModPatch> animdata_patches;

    };
}
