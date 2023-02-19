#include "update/modpatch.h"

nemesis::ModPatch::ModPatchParser::ModPatchParser(nemesis::ModPatch& patch) : nemesis::Patch::PatchParser(patch)
{
}

void nemesis::ModPatch::ModPatchParser::ReadFile(const std::filesystem::path& filepath)
{
    DebugLogging(L"Parsing mod patch: " + filepath.wstring());
    nemesis::Patch::PatchParser::ReadFile(filepath);
}

nemesis::ModPatch::ModPatch(const std::string& code, const std::filesystem::path& filepath)
    : nemesis::Patch(code, filepath)
{
    nemesis::ModPatch::ModPatchParser parser(*this);
    parser.ReadFile(filepath);
    parser.ParseFile();
}

nemesis::CondType nemesis::ModPatch::GetConditionType() const
{
    return nemesis::CondType::MOD_CODE;
}
