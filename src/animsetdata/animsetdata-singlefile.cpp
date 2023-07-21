#include "animsetdata/animsetdata-singlefile.h"

#include "core/memoryfile.h"

#include "utilities/animtemplate.h"
#include "utilities/writetextfile.h"

nemesis::animsetdata::SingleFile::Parser::Parser(nemesis::animsetdata::SingleFile& file)
    : file(file)
    , nemesis::Importer<nemesis::animsetdata::LinkedProject>(file)
{
}

nemesis::animsetdata::SingleFile::Exporter::Exporter(const nemesis::animsetdata::SingleFile& file)
    : singlefile(file)
    , nemesis::Exporter(file)
{
}

const VecNstr nemesis::animsetdata::SingleFile::Exporter::GetLines() const
{
    return lines;
}

const nemesis::AnimTemplate* nemesis::animsetdata::SingleFile::Exporter::GetAnimTemplate()
{
    return singlefile.animtemplate;
}

const nemesis::TemplateCategory*
nemesis::animsetdata::SingleFile::Exporter::GetTemplateCategory(const std::string& name)
{
    return nullptr;
}

void nemesis::animsetdata::SingleFile::Exporter::Export()
{
    VecNstr headers;
    lines.clear();

    for (auto& linkedproject : singlefile.projects)
    {
        auto proj_list = linkedproject.GetCompiledData(*scopeinfo);

        for (auto& proj : proj_list)
        {
            headers.emplace_back(std::string(proj->GetName()));
            auto temp_lines = proj->GetCompiledLines(*scopeinfo);
            lines.insert(lines.end(), temp_lines.begin(), temp_lines.end());
        }
    }

    headers.insert(headers.begin(), std::to_string(headers.size()));
    lines.insert(lines.begin(), headers.begin(), headers.end());
}

bool nemesis::animsetdata::SingleFile::Exporter::IsTemplateActive(const std::string& name)
{
    auto* querylist_ptr = GetQueriesByTemplate(name);
    return querylist_ptr && !querylist_ptr->empty();
}

const Vec<const nemesis::AnimQuery*>*
nemesis::animsetdata::SingleFile::Exporter::GetQueriesByTemplate(const std::string& name)
{
    auto* classptr = singlefile.animtemplate->GetClass(name);

    if (classptr == nullptr) ErrorMessage(1225, singlefile.GetFilePath(), name);

    for (auto& queryinfo : singlefile.queries)
    {
        if (classptr != &queryinfo.first->GetTemplateCategory()) continue;

        return &queryinfo.second;
    }

    return nullptr;
}

const nemesis::AnimTemplate* nemesis::animsetdata::SingleFile::GetAnimTemplate() const noexcept
{
    return animtemplate;
}

void nemesis::animsetdata::SingleFile::SaveAs(const std::filesystem::path& path) const
{
    VecNstr lines;
    lines.emplace_back(std::to_string(projects.size()));

    for (auto& project : projects)
    {
        project.GetRawName(lines);
    }

    for (auto& project : projects)
    {
        project.GetRawData(lines);
    }

    FileWriter writer(path);
    writer.WriteLines(lines);
}

void nemesis::animsetdata::SingleFile::CompileAs(const std::filesystem::path& path) const
{
    Exporter exporter(*this);
    exporter.Export();
    auto& lines = exporter.GetLines();
    FileWriter writer(path);
    writer.WriteLines(lines);
}
