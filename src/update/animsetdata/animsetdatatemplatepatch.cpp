#include "update/animsetdata/animsetdatatemplatepatch.h"

nemesis::AnimSetDataTemplatePatch::AnimSetDataTemplatePatch(const std::string& code,
                                                            const std::string& project,
                                                            const std::filesystem::path& path)
    : code(code)
    , project(project)
    , nemesis::File(path)
{
}

bool nemesis::AnimSetDataTemplatePatch::IsNew() const
{
    std::string filename = filepath.stem().string();
    return filename.length() > 2 && filename.front() == '$' && filename.back() == '$';
}

std::string_view nemesis::AnimSetDataTemplatePatch::GetCode() const
{
    return code;
}

std::string_view nemesis::AnimSetDataTemplatePatch::GetProject() const
{
    return project;
}
