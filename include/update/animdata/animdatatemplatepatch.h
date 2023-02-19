#pragma once

#include "base/file.h"

namespace nemesis
{
    struct AnimDataTemplatePatch : nemesis::File
    {
        std::string character;
        std::string header;

        AnimDataTemplatePatch(const std::filesystem::path& filepath);
    };
}
