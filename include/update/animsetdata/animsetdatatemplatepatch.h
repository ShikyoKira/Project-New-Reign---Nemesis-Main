#pragma once

#include "base/file.h"

namespace nemesis
{
	struct AnimSetDataTemplatePatch : nemesis::File
    {
        std::string code;
        std::string project;

    public:
        AnimSetDataTemplatePatch(const std::string& code,
                                 const std::string& project,
                                 const std::filesystem::path& path);

        bool IsNew() const;
        std::string_view GetCode() const;
        std::string_view GetProject() const;
    };
}
