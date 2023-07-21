#include "animdata/animdata-singlefile.h"

#include "core/memoryfile.h"

#include "utilities/animtemplate.h"
#include "utilities/writetextfile.h"

nemesis::animdata::SingleFile::Exporter::Exporter(const nemesis::animdata::SingleFile& file)
    : singlefile(file)
    , nemesis::Exporter(file)
{
}

const VecNstr nemesis::animdata::SingleFile::Exporter::GetLines() const
{
    return lines;
}

const nemesis::AnimTemplate* nemesis::animdata::SingleFile::Exporter::GetAnimTemplate()
{
    return singlefile.animtemplate;
}

const nemesis::TemplateCategory*
nemesis::animdata::SingleFile::Exporter::GetTemplateCategory(const std::string& name)
{
    return nullptr;
}

void nemesis::animdata::SingleFile::Exporter::Export()
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

bool nemesis::animdata::SingleFile::Exporter::IsTemplateActive(const std::string& name)
{
    auto* querylist_ptr = GetQueriesByTemplate(name);
    return querylist_ptr && !querylist_ptr->empty();
}

const Vec<const nemesis::AnimQuery*>*
nemesis::animdata::SingleFile::Exporter::GetQueriesByTemplate(const std::string& name)
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

void nemesis::animdata::SingleFile::LoadFile(const std::filesystem::path& path)
{
    filepath = path;
    VecNstr lines = GetLines();
    size_t num_project = std::stoi(lines[0]); 
    projects.reserve(num_project);

    for (size_t i = 1; i < num_project; i++)
    {
        nemesis::animdata::LinkedProject link_proj;
        nemesis::animdata::AnimProject proj;
        proj.SetName(lines[i]);
        link_proj.SetRaw(proj);
        projects.push_back(link_proj);
    }
    
    if (projects.empty()) return;

    nemesis::animdata::LinkedProject* project_ptr = &projects.front();

    for (size_t i = num_project; i < lines.size(); i++)
    {
        size_t project_size = std::stoi(lines[i++]) + i;
        auto anim_proj_ptr  = project_ptr->GetRawPtr();

        if (std::stoi(lines[i++]) == 0) continue;

        size_t hkx_size = std::stoi(lines[i++]) + i;

        for (; i < hkx_size; i++)
        {
            anim_proj_ptr->AddHkxFile(lines[i]);
        }

        for (; i < project_size; i++)
        {
            anim_proj_ptr->AddAnimationData(lines[i]);
        }

        size_t mv_size = std::stoi(lines[i++]) + i;

        for (; i < mv_size; i++)
        {
            anim_proj_ptr->AddMovementData(lines[i]);
        }
    }
}

const nemesis::AnimTemplate* nemesis::animdata::SingleFile::GetAnimTemplate() const noexcept
{
    return animtemplate;
}

void nemesis::animdata::SingleFile::SaveAs(const std::filesystem::path& path) const
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

void nemesis::animdata::SingleFile::CompileAs(const std::filesystem::path& path) const
{
    Exporter exporter(*this);
    exporter.Export();
    auto& lines = exporter.GetLines();
    FileWriter writer(path);
    writer.WriteLines(lines);
}
