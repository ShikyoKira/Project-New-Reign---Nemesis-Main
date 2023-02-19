#include "update/templatepatch.h"

nemesis::TemplatePatch::TemplatePatchParser::TemplatePatchParser(nemesis::TemplatePatch& patch) : nemesis::Patch::PatchParser(patch)
{
}

void nemesis::TemplatePatch::TemplatePatchParser::ReadFile(const std::filesystem::path& filepath)
{
    DebugLogging(L"Parsing template patch: " + filepath.wstring());
    nemesis::Patch::PatchParser::ReadFile(filepath);
}

nemesis::TemplatePatch::TemplatePatch(const std::string& code,
                                      const std::filesystem::path& filepath,
                                      const std::filesystem::path& datapath)
    : nemesis::Patch(code, filepath), datapath(datapath)
{
    nemesis::TemplatePatch::PatchParser parser(*this);
    parser.ReadFile(filepath);
    parser.ParseFile();
}

nemesis::CondType nemesis::TemplatePatch::GetConditionType() const
{
    return nemesis::CondType::IF;
}

std::filesystem::path nemesis::TemplatePatch::GetDataPath() const
{
    return datapath;
}
