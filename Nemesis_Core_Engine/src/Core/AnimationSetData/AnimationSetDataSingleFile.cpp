#include <fstream>

#include "Core/AnimationSetData/AnimationSetDataSingleFile.h"

#include "Core/CollectionObject.h"
#include "Core/CompilationManager.h"
#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Core/Template/TemplateObject.h"

#include "Logger.h"
#include "NemesisInfo.h"

#include "Utilities/Algorithm.h"
#include "Utilities/Crc32.h"
#include "Utilities/File.h"

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
                                 + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
    }

    for (++stream; !stream.IsEoF() && !is_only_number(stream.GetToken().Value.ToString()); ++stream)
    {
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::NONE)
        {
            auto& token_value = stream.GetToken().Value;
            throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ", File: " + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
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
            std::ofstream file(filepath);

            if (!file.is_open())
            {
                std::error_code ec(errno, std::system_category());
                throw std::runtime_error("Failed to open file: \"" + to_utf8_string(filepath)
                                         + "\"\nMessage: " + ec.message());
            }

            std::string full_text;

            for (auto& line : lines)
            {
                std::string sline = line + "\n";
                file << sline;
                full_text.append(sline);
            }

            file.close();

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

            header_lines.emplace_back(nemesis::to_utf8_string(project->GetFilePath().stem()));

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
    CompileFileAs(target_path, state);
    return target_path;
}

void nemesis::AnimationSetDataSingleFile::CompileFileAs(const std::filesystem::path& filepath,
                                                        nemesis::CompileState& state) const
{
    auto path = PATH_TO_STRING(filepath);
    Logger::Log(LITERAL_PATH("Compiling Target File: ") + path);

    CompileFileCore(filepath, state, [] {}).get();

    Logger::Log(LITERAL_PATH("Compiled Target File: ") + path);
}

std::filesystem::path
nemesis::AnimationSetDataSingleFile::ScheduleCompileFile(nemesis::CompileState& state) const
{
    auto target_path = NemesisInfo::PatchOutputPath(TargetPath);
    ScheduleCompileFileAs(target_path, state);
    return target_path;
}

std::filesystem::path
nemesis::AnimationSetDataSingleFile::ScheduleCompileFile(nemesis::CompileState& state,
                                                         std::function<void()> callback) const
{
    auto target_path = NemesisInfo::PatchOutputPath(TargetPath);
    ScheduleCompileFileAs(target_path, state, callback);
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
    auto path = PATH_TO_STRING(filepath);
    Logger::Log(LITERAL_PATH("Compiling Target File: ") + path);

    auto future = CompileFileCore(filepath,
                                  state,
                                  [path, callback]()
                                  {
                                      callback();
                                      Logger::Log(LITERAL_PATH("Compiled Target File: ") + path);
                                  });

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
    auto& proj           = ProjectList.emplace_back(std::move(project));
    auto& name           = proj->GetName();
    ProjectNameMap[name] = proj.get();
    return proj;
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
    auto itr = ProjectNameMap.find(project_name);
    
    if (itr == ProjectNameMap.end()) return nullptr;

    return itr->second;
}

const nemesis::AnimationSetDataProject*
nemesis::AnimationSetDataSingleFile::GetProject(const std::string& project_name) const
{
    std::scoped_lock<std::mutex> lock(ProjectMutex);
    auto itr = ProjectNameMap.find(project_name);

    if (itr == ProjectNameMap.end()) return nullptr;

    return itr->second;
}

void nemesis::AnimationSetDataSingleFile::SerializeToDirectory(
    const std::filesystem::path& directory_path) const
{
    std::filesystem::create_directories(directory_path);

    for (size_t i = 0; i < ProjectList.size(); i++)
    {
        auto& project = ProjectList[i];
        project->SerializeToDirectory(directory_path / (std::to_string(i + 1) + "~" + project->GetName()));
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
        auto filename = nemesis::to_utf8_string(path.filename());
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
    Logger::Log(LITERAL_PATH("AnimationSetDataSingleFile: ") + PATH_TO_STRING(filepath));

    auto singlefile        = std::make_unique<nemesis::AnimationSetDataSingleFile>();
    auto filename          = PATH_TO_STRING(filepath.filename());
    singlefile->FilePath   = filepath;
    singlefile->TargetPath
        = filepath.parent_path()
          / (nemesis::istarts_with(filename, LITERAL_PATH("nemesis_")) ? filename.substr(8) : filename);
    singlefile->RelativePath
        = PATH_TO_STRING(singlefile->TargetPath).substr(PATH_TO_STRING(NemesisInfo::DataPath()).length() + 1);

    Logger::Log(LITERAL_PATH("Processing AnimationSetDataSingleFile: ")
                + PATH_TO_STRING(singlefile->GetFilePath()));

    nemesis::SemanticManager manager;
    VecNstr lines;
    GetFileLines(filepath, lines, false);
    nemesis::LineStream stream(lines.begin(), lines.end());

    VecNstr headers         = nemesis::AnimationSetDataSingleFile::ParseHeaders(stream, manager);
    singlefile->ProjectList = nemesis::AnimationSetDataProject::ParseObjects(stream, manager, headers);

    for (auto& proj : singlefile->ProjectList)
    {
        const std::string& name          = proj->GetName();
        singlefile->ProjectNameMap[name] = proj.get();
    }

    return singlefile;
}

UPtr<nemesis::AnimationSetDataSingleFile>
nemesis::AnimationSetDataSingleFile::ParseFromFile(const std::filesystem::path& filepath,
                                                   nemesis::ThreadPool& threadpool)
{
    Logger::Log(LITERAL_PATH("AnimationSetDataSingleFile: ") + PATH_TO_STRING(filepath));

    auto singlefile        = std::make_unique<nemesis::AnimationSetDataSingleFile>();
    auto filename          = PATH_TO_STRING(filepath.filename());
    singlefile->FilePath   = filepath;
    singlefile->TargetPath
        = filepath.parent_path()
          / (nemesis::istarts_with(filename, LITERAL_PATH("nemesis_")) ? filename.substr(8) : filename);
    singlefile->RelativePath
        = PATH_TO_STRING(singlefile->TargetPath).substr(PATH_TO_STRING(NemesisInfo::DataPath()).length() + 1);
    auto singlefile_ptr = singlefile.get();

    threadpool.enqueue(
        [singlefile_ptr]()
        {
            Logger::Log(LITERAL_PATH("Processing AnimationSetDataSingleFile: ")
                        + PATH_TO_STRING(singlefile_ptr->GetFilePath()));

            nemesis::SemanticManager manager;
            VecNstr lines;
            GetFileLines(singlefile_ptr->GetFilePath(), lines, false);
            nemesis::LineStream stream(lines.begin(), lines.end());

            VecNstr headers = nemesis::AnimationSetDataSingleFile::ParseHeaders(stream, manager);
            singlefile_ptr->ProjectList
                = nemesis::AnimationSetDataProject::ParseObjects(stream, manager, headers);

            for (auto& proj : singlefile_ptr->ProjectList)
            {
                const std::string& name              = proj->GetName();
                singlefile_ptr->ProjectNameMap[name] = proj.get();
            }
        });
    return singlefile;
}
