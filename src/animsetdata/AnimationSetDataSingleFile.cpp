#include "animsetdata/AnimationSetDataSingleFile.h"

#include "core/SemanticManager.h"
#include "core/CollectionObject.h"

VecNstr nemesis::AnimationSetDataSingleFile::ParseHeaders(nemesis::LineStream& stream,
                                                          nemesis::SemanticManager& manager)
{
    VecNstr headers;

    if (stream.IsEoF()) return headers;

    auto& token = stream.GetToken();
    auto& value = token.Value;

    if (token.Type != nemesis::LineStream::NONE || !isOnlyNumber(value))
    {
        throw std::runtime_error("nemesis::AnimationSetDataSingleFile::ParseHeaders format error (Line: "
                                 + std::to_string(value.GetLineNumber())
                                 + ". File: " + value.GetFilePath().string() + ")");
    }

    for (++stream; !stream.IsEoF() && !isOnlyNumber(stream.GetToken().Value); ++stream)
    {
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::NONE)
        {
            auto& token_value = stream.GetToken().Value;
            throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ". File: " + token_value.GetFilePath().string() + ")");
        }

        headers.emplace_back(token.Value);
    }

    return headers;
}

void nemesis::AnimationSetDataSingleFile::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr project_contents;
    size_t count          = 0;
    auto& counter_element = lines.emplace_back("");

    for (auto& project : ProjectList)
    {
        size_t size = project_contents.size();
        project->CompileTo(project_contents, state);

        if (size >= project_contents.size()) continue;

        count++;
        lines.emplace_back(project->GetName());
    }

    counter_element = std::to_string(count);

    for (auto& line : project_contents)
    {
        lines.emplace_back(std::move(line));
    }
}

void nemesis::AnimationSetDataSingleFile::SerializeTo(DeqNstr& lines) const
{
    throw std::runtime_error("nemesis::AnimationSetDataSingleFile::SerializeTo is not supported");
}

UPtr<nemesis::NObject> nemesis::AnimationSetDataSingleFile::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationSetDataSingleFile> nemesis::AnimationSetDataSingleFile::Clone() const
{
    auto file     = std::make_unique<nemesis::AnimationSetDataSingleFile>();
    auto& map     = file->ProjectList;

    for (auto& project : ProjectList)
    {
        file->ProjectList.emplace_back(project->Clone());
    }

    return file;
}

UPtr<nemesis::AnimationSetDataProject>&
nemesis::AnimationSetDataSingleFile::AddProject(UPtr<nemesis::AnimationSetDataProject>&& project)
{
    std::scoped_lock<std::mutex> lock(UpdaterMutex);
    return ProjectList.emplace_back(std::move(project));
}

nemesis::AnimationSetDataProject*
nemesis::AnimationSetDataSingleFile::GetProject(const std::string& project_name)
{
    for (auto& project : ProjectList)
    {
        if (project->GetName() != project_name) continue;

        return project.get();
    }

    return nullptr;
}

const nemesis::AnimationSetDataProject*
nemesis::AnimationSetDataSingleFile::GetProject(const std::string& project_name) const
{
    for (auto& project : ProjectList)
    {
        if (project->GetName() != project_name) continue;

        return project.get();
    }

    return nullptr;
}

void nemesis::AnimationSetDataSingleFile::SerializeToDirectory(
    const std::filesystem::path& directory_path) const
{
    std::filesystem::create_directories(directory_path);

    for (size_t i = 0; i < ProjectList.size(); i++)
    {
        auto& project = ProjectList[i];
        std::filesystem::path project_dir = std::to_string(i + 1) + "~" + project->GetName();
        project->SerializeToDirectory(directory_path / project_dir);
    }
}

UPtr<nemesis::AnimationSetDataSingleFile>
nemesis::AnimationSetDataSingleFile::DeserializeFromDirectory(const std::filesystem::path& directory_path)
{
    auto singlefile = std::make_unique<nemesis::AnimationSetDataSingleFile>();
    Map<size_t, std::pair<std::filesystem::path, std::string>> project_dir;

    for (auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        if (!entry.is_directory()) continue;

        auto path     = entry.path();
        auto filename = path.filename().string();
        size_t pos    = filename.rfind("~");

        if (pos == NOT_FOUND) continue;

        std::string num = filename.substr(pos);

        if (isOnlyNumber(num)) continue;

        project_dir[std::stoul(num)] = std::make_pair(path, filename.substr(pos + 1));
    }

    for (auto& each : project_dir)
    {
        singlefile->AddProject(nemesis::AnimationSetDataProject::DeserializeFromDirectory(
            each.second.first, each.second.second));
    }

    return singlefile;
}

UPtr<nemesis::AnimationSetDataSingleFile>
nemesis::AnimationSetDataSingleFile::ParseFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    VecNstr lines;
    GetFileLines(filepath, lines, false);
    nemesis::LineStream stream(lines.begin(), lines.end());

    UPtr<nemesis::AnimationSetDataSingleFile> singlefile
        = std::make_unique<nemesis::AnimationSetDataSingleFile>();
    auto headers            = nemesis::AnimationSetDataSingleFile::ParseHeaders(stream, manager);
    singlefile->ProjectList = nemesis::AnimationSetDataProject::ParseObjects(stream, manager, headers);
    return singlefile;
}
