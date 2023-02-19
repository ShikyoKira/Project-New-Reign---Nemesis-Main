#include "animdata/AnimDataProjectFactory.h"


nemesis::animdata::AnimDataProjectFactory::AnimDataProjectFactory(Vec<SPtr<nemesis::animdata::LinkedProject>>& list)
    : nemesis::Importer<nemesis::animdata::LinkedProject>(nfile)
{
    stream.emplace_back(&list);
}

void nemesis::animdata::AnimDataProjectFactory::AddLine(const nemesis::Line& line)
{
    if (line.ToString()._Starts_with(nemesis::syntax::DeleteLine())) return;


}

void nemesis::animdata::AnimDataProjectFactory::ParseLines(const VecNstr& lines)
{
    if (lines.empty()) return;

    auto& rline = lines.front();

    if (!isOnlyNumber(rline))
    {
        ErrorMessage(3015, rline.GetFilePath(), rline.GetLineNumber());
    }

    for (size_t i = 1; i < lines.size(); i++)
    {
        auto& line = lines[i];

    }
}

//UPtr<nemesis::animdata::SingleFile>
//nemesis::animdata::AnimDataProjectFactory::GenerateSingleFile(const nemesis::File& file)
//{
//    auto filelines = file.GetLines();
//
//    for (auto& line : filelines)
//    {
//
//    }
//
//    //return nullptr;
//    return UPtr<nemesis::animdata::SingleFile>();
//}
