#include "Core/AnimationSetData/AnimationSetDataSingleFile.h"

#include "Core/CompileState.h"
#include "Core/SemanticManager.h"
#include "Core/CollectionObject.h"
#include "Core/CompilationManager.h"

#include "Core/Template/TemplateObject.h"

#include "Logger.h"
#include "NemesisInfo.h"

#include "Utilities/File.h"
#include "Utilities/Crc32.h"
#include "Utilities/Algorithm.h"
#include "Utilities/FileWriter.h"


VecNstr nemesis::AnimationSetDataSingleFile::ParseHeaders(nemesis::LineStream& stream,
                                                          nemesis::SemanticManager& manager)
{
    VecNstr headers;

    if (stream.IsEoF()) return headers;

    auto& token = stream.GetToken();
    auto& value = token.Value;

    if (token.Type != nemesis::LineStream::NONE || !is_only_number(value.ToString()))
    {
        throw std::runtime_error("nemesis::AnimationSetDataSingleFile::ParseHeaders format error (Line: "
                                 + std::to_string(value.GetLineNumber())
                                 + ", File: " + value.GetFilePath().string() + ")");
    }

    for (++stream; !stream.IsEoF() && !is_only_number(stream.GetToken().Value.ToString()); ++stream)
    {
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::NONE)
        {
            auto& token_value = stream.GetToken().Value;
            throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ", File: " + token_value.GetFilePath().string() + ")");
        }

        headers.emplace_back(token.Value);
    }

    return headers;
}

std::future<void> nemesis::AnimationSetDataSingleFile::CompileFileCore(const std::filesystem::path& filepath,
                                                                       nemesis::CompileState& state,
                                                                       std::function<void()> callback) const
{
    std::filesystem::create_directories(filepath.parent_path());

    return std::async(
        [this, filepath, &state, callback]
        {
            DeqNstr lines = Compile(state);
            FileWriter writer(filepath);
            std::string full_text;

            for (auto& line : lines)
            {
                writer.LockFreeWriteLine(line);
                full_text.append(line + "\n");
            }

            writer.Close();
            static nemesis::CRC32 crc32;
            size_t checksum = crc32.FullCRC(full_text);
            state.AddCheckSum(TargetPath, std::to_string(checksum));
            callback();
        });
}

void nemesis::AnimationSetDataSingleFile::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr header_lines;
    DeqNstr project_contents;

    {
        std::scoped_lock lock(ProjectMutex, ProjectTemplateMutex);

        for (auto& project : ProjectList)
        {
            DeqNstr temp_lines;
            project->CompileTo(temp_lines, state);

            if (temp_lines.empty()) continue;

            header_lines.emplace_back(project->GetName());

            for (auto& line : temp_lines)
            {
                project_contents.emplace_back(std::move(line));
            }
        }

        for (auto& project : ProjectTemplateList)
        {
            DeqNstr temp_lines;
            project->CompileTo(temp_lines, state);

            if (temp_lines.empty()) continue;

            header_lines.emplace_back(project->GetFilePath().stem().string());

            for (auto& line : temp_lines)
            {
                project_contents.emplace_back(std::move(line));
            }
        }
    }

    lines.emplace_back(std::to_string(header_lines.size()));

    for (auto& line : header_lines)
    {
        lines.emplace_back(std::move(line));
    }

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
    std::scoped_lock lock(ProjectMutex, ProjectTemplateMutex);
    auto file = std::make_unique<nemesis::AnimationSetDataSingleFile>();

    for (auto& project : ProjectList)
    {
        file->ProjectList.emplace_back(project->Clone());
    }

    for (auto& project : ProjectTemplateList)
    {
        file->ProjectTemplateList.emplace_back(project->SClone());
    }

    return file;
}

std::filesystem::path nemesis::AnimationSetDataSingleFile::CompileFile(nemesis::CompileState& state) const
{
    auto target_path = NemesisInfo::PatchOutputPath(TargetPath);
    Logger::Log(L"Compiling Target File: " + target_path.wstring());

    CompileFileAs(target_path, state);

    Logger::Log(L"Compiled Target File: " + target_path.wstring());
    return target_path;
}

void nemesis::AnimationSetDataSingleFile::CompileFileAs(const std::filesystem::path& filepath,
                                                        nemesis::CompileState& state) const
{
    CompileFileCore(filepath, state, [] {}).get();
}

std::filesystem::path
nemesis::AnimationSetDataSingleFile::ScheduleCompileFile(nemesis::CompileState& state) const
{
    auto target_path = NemesisInfo::PatchOutputPath(TargetPath);
    Logger::Log(L"Compiling Target File: " + target_path.wstring());

    ScheduleCompileFileAs(target_path,
                          state,
                          [target_path] { Logger::Log(L"Compiled Target File: " + target_path.wstring()); });
    return target_path;
}

void nemesis::AnimationSetDataSingleFile::ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                                                nemesis::CompileState& state) const
{
    ScheduleCompileFileAs(filepath, state, [] {});
}

void nemesis::AnimationSetDataSingleFile::ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                                                nemesis::CompileState& state,
                                                                std::function<void()> callback) const
{
    auto future = CompileFileCore(filepath, state, callback);

    std::scoped_lock<std::mutex> lock(CompileFutureMutex);
    CompileFuture.emplace_back(std::move(future));
}

void nemesis::AnimationSetDataSingleFile::WaitForCompleteCompilation() const
{
    std::scoped_lock<std::mutex> lock(CompileFutureMutex);

    for (auto& future : CompileFuture)
    {
        future.get();
    }

    CompileFuture.clear();
}

std::filesystem::path nemesis::AnimationSetDataSingleFile::GetFilePath() const
{
    return FilePath;
}

std::filesystem::path nemesis::AnimationSetDataSingleFile::GetTargetPath() const
{
    return TargetPath;
}

UPtr<nemesis::AnimationSetDataProject>&
nemesis::AnimationSetDataSingleFile::AddProject(UPtr<nemesis::AnimationSetDataProject>&& project)
{
    std::scoped_lock<std::mutex> lock(ProjectMutex);
    return ProjectList.emplace_back(std::move(project));
}

SPtr<nemesis::TemplateObject>&
nemesis::AnimationSetDataSingleFile::AddProjectTemplate(const SPtr<nemesis::TemplateObject>& templt_obj)
{
    std::scoped_lock<std::mutex> lock(ProjectTemplateMutex);
    return ProjectTemplateList.emplace_back(templt_obj);
}

nemesis::AnimationSetDataProject*
nemesis::AnimationSetDataSingleFile::GetProject(const std::string& project_name)
{
    std::scoped_lock<std::mutex> lock(ProjectMutex);

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
    std::scoped_lock<std::mutex> lock(ProjectMutex);

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

        if (is_only_number(num)) continue;

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
    Logger::Log(L"AnimationSetDataSingleFile: " + filepath.wstring());

    auto singlefile        = std::make_unique<nemesis::AnimationSetDataSingleFile>();
    singlefile->FilePath   = filepath;
    singlefile->TargetPath = filepath.parent_path()
                             / (nemesis::istarts_with(filepath.filename().wstring(), L"nemesis_")
                                    ? filepath.filename().wstring().substr(8)
                                    : filepath.filename().wstring());
    singlefile->RelativePath
        = singlefile->TargetPath.wstring().substr(NemesisInfo::DataPath().wstring().length() + 1);

    Logger::Log(L"Processing AnimationSetDataSingleFile: " + singlefile->GetFilePath().wstring());

    nemesis::SemanticManager manager;
    VecNstr lines;
    GetFileLines(filepath, lines, false);
    nemesis::LineStream stream(lines.begin(), lines.end());

    VecNstr headers         = nemesis::AnimationSetDataSingleFile::ParseHeaders(stream, manager);
    singlefile->ProjectList = nemesis::AnimationSetDataProject::ParseObjects(stream, manager, headers);
    return singlefile;
}

UPtr<nemesis::AnimationSetDataSingleFile>
nemesis::AnimationSetDataSingleFile::ParseFromFile(const std::filesystem::path& filepath,
                                                   nemesis::ThreadPool& threadpool)
{
    Logger::Log(L"AnimationSetDataSingleFile: " + filepath.wstring());

    auto singlefile        = std::make_unique<nemesis::AnimationSetDataSingleFile>();
    singlefile->FilePath   = filepath;
    singlefile->TargetPath = filepath.parent_path()
                             / (nemesis::istarts_with(filepath.filename().wstring(), L"nemesis_")
                                    ? filepath.filename().wstring().substr(8)
                                    : filepath.filename().wstring());
    singlefile->RelativePath
        = singlefile->TargetPath.wstring().substr(NemesisInfo::DataPath().wstring().length() + 1);
    auto singlefile_ptr = singlefile.get();

    threadpool.enqueue(
        [singlefile_ptr]()
        {
            Logger::Log(L"Processing AnimationSetDataSingleFile: " + singlefile_ptr->GetFilePath().wstring());

            nemesis::SemanticManager manager;
            VecNstr lines;
            GetFileLines(singlefile_ptr->GetFilePath(), lines, false);
            nemesis::LineStream stream(lines.begin(), lines.end());

            VecNstr headers = nemesis::AnimationSetDataSingleFile::ParseHeaders(stream, manager);
            singlefile_ptr->ProjectList
                = nemesis::AnimationSetDataProject::ParseObjects(stream, manager, headers);
        });
    return singlefile;
}
