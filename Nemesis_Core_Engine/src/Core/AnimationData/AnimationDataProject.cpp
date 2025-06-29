#include <fstream>

#include "Core/AnimationData/AnimationDataProject.h"

#include "Core/CoreObject.h"

#include "Utilities/Algorithm.h"
#include "Utilities/File.h"
#include "Utilities/ThreadPool.h"

const std::filesystem::path& nemesis::AnimationDataProject::Headers::GetFilePath() const noexcept
{
    return FilePath;
}

UPtr<nemesis::CollectionObject> nemesis::AnimationDataProject::Headers::Clone() const
{
    auto collection      = std::make_unique<nemesis::AnimationDataProject::Headers>();
    collection->FilePath = FilePath;

    for (auto& object : Objects)
    {
        collection->AddObject(object->CloneNObject());
    }

    return collection;
}

UPtr<nemesis::AnimationDataProject::Headers> nemesis::AnimationDataProject::Headers::CloneHeaders() const
{
    return UPtr<nemesis::AnimationDataProject::Headers>(
        static_cast<nemesis::AnimationDataProject::Headers*>(Clone().release()));
}

UPtr<nemesis::AnimationDataProject> nemesis::AnimationDataProject::ParseProject(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, const std::string& project_name)
{
    auto& stoken = stream.GetToken();
    auto& ssize  = stoken.Value;

    if (stoken.Type != nemesis::LineStream::NONE || !is_only_number(ssize.ToString()))
    {
        throw std::runtime_error("Invalid nemesis::AnimationDataProject::ParseProject size (Line: "
                                 + std::to_string(ssize.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(ssize.GetFilePath()) + ")");
    }

    size_t fsize;
    size_t size     = std::stoul(ssize.ToString());
    auto* end_token = stream.GetForwardToken(size);

    if (!end_token)
    {
        throw std::runtime_error("Invalid nemesis::AnimationDataProject::ParseProject size (Line: "
                                 + std::to_string(ssize.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(ssize.GetFilePath()) + ")");
    }

    ParseStage type = nemesis::AnimationDataProject::IsActiveStage;
    auto project    = std::make_unique<nemesis::AnimationDataProject>(project_name);

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();
        auto& value = token.Value;

        if (token.Type != nemesis::LineStream::NONE)
        {
            throw std::runtime_error("Syntax Error: Unexpected syntax (Line: "
                                     + std::to_string(value.GetLineNumber())
                                     + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
        }

        if (end_token == &token)
        {
            if (project->ClipDataList.empty()) return project;

            type = nemesis::AnimationDataProject::MotionDataStage;
            continue;
        }

        switch (type)
        {
            case nemesis::AnimationDataProject::IsActiveStage:
            {
                type = nemesis::AnimationDataProject::HkxFileCountStage;
                break;
            }
            case nemesis::AnimationDataProject::HkxFileCountStage:
            {
                auto& value = token.Value;

                if (!is_only_number(value.ToString()))
                {
                    throw std::runtime_error(
                        "Invalid nemesis::AnimationDataProject::ParseProject format (Line: "
                        + std::to_string(value.GetLineNumber())
                        + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
                }

                fsize             = std::stoul(value.ToString());
                project->HkxFiles = std::make_unique<nemesis::AnimationDataProject::Headers>();

                if (fsize > 0)
                {
                    type = nemesis::AnimationDataProject::HkxFileStage;
                    break;
                }

                type = nemesis::AnimationDataProject::ClipDataStage;
                break;
            }
            case nemesis::AnimationDataProject::HkxFileStage:
            {
                auto& value = token.Value;
                project->HkxFiles->AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));

                if (project->HkxFiles->Size() < fsize) break;

                type = nemesis::AnimationDataProject::ClipDataStage;
                break;
            }
            case nemesis::AnimationDataProject::ClipDataStage:
            {
                project->ClipDataList
                    = nemesis::AnimationDataClipData::ParseObjects(stream, manager, end_token);

                if (project->ClipDataList.empty()) return project;

                type = nemesis::AnimationDataProject::MotionDataStage;
                break;
            }
            case nemesis::AnimationDataProject::MotionDataStage:
            {
                auto& value = token.Value;

                if (!is_only_number(value.ToString()))
                {
                    throw std::runtime_error(
                        "Invalid nemesis::AnimationDataProject::ParseObjects format (Line: "
                        + std::to_string(value.GetLineNumber())
                        + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
                }

                project->MotionDataList = nemesis::AnimationDataMotionData::ParseObjects(stream, manager);
                return project;
            }
        }
    }

    return project;
}

nemesis::AnimationDataProject::AnimationDataProject(const std::string& name) noexcept
    : Name(name)
{
}

void nemesis::AnimationDataProject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr hkxfile_lines;
    DeqNstr clipdata_lines;
    DeqNstr motiondata_lines;

    {
        std::scoped_lock lock(
            HkxFileMutex, ClipDataMutex, MotionDataMutex, ClipTemplateMutex, MotionTemplateMutex);
        HkxFiles->CompileTo(hkxfile_lines, state);

        for (auto& clip_data : ClipDataList)
        {
            clip_data->CompileTo(clipdata_lines, state);
        }

        for (auto& motion_data : MotionDataList)
        {
            motion_data->CompileTo(motiondata_lines, state);
        }

        state.ClearAllConditionCache();

        for (auto& clip_data : ClipDataTemplateList)
        {
            auto& requests = state.GetRequests(clip_data->GetClassName());

            for (auto& request : requests)
            {
                state.SetBaseRequest(request);
                clip_data->CompileTo(clipdata_lines, state);
            }
        }

        state.SetBaseRequest(nullptr);
        state.ClearAllConditionCache();

        for (auto& motion_data : MotionDataTemplateList)
        {
            auto& requests = state.GetRequests(motion_data->GetClassName());

            for (auto& request : requests)
            {
                state.SetBaseRequest(request);
                motion_data->CompileTo(motiondata_lines, state);
            }
        }

        state.SetBaseRequest(nullptr);
        state.ClearAllConditionCache();
    }

    if (hkxfile_lines.empty() && clipdata_lines.empty()) return;

    lines.emplace_back(std::to_string(hkxfile_lines.size() + clipdata_lines.size() + 3));
    lines.emplace_back("1");
    lines.emplace_back(std::to_string(hkxfile_lines.size()));

    for (auto& line : hkxfile_lines)
    {
        lines.emplace_back(std::move(line));
    }

    if (clipdata_lines.empty())
    {
        lines.emplace_back("0");
        return;
    }

    lines.emplace_back("1");

    for (auto& line : clipdata_lines)
    {
        lines.emplace_back(std::move(line));
    }

    lines.emplace_back(std::to_string(motiondata_lines.size()));

    for (auto& line : motiondata_lines)
    {
        lines.emplace_back(std::move(line));
    }
}

void nemesis::AnimationDataProject::SerializeTo(DeqNstr& lines) const
{
    throw std::runtime_error("nemesis::AnimationDataProject::SerializeTo is not supported");
}

UPtr<nemesis::NObject> nemesis::AnimationDataProject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationDataProject> nemesis::AnimationDataProject::Clone() const
{
    std::scoped_lock lock(
        HkxFileMutex, ClipDataMutex, MotionDataMutex, ClipTemplateMutex, MotionTemplateMutex);
    auto project      = std::make_unique<nemesis::AnimationDataProject>(Name);
    project->HkxFiles = HkxFiles->CloneHeaders();

    for (auto& clip_data : ClipDataList)
    {
        project->ClipDataList.emplace_back(clip_data->Clone());
    }

    for (auto& motion_data : MotionDataList)
    {
        project->MotionDataList.emplace_back(motion_data->Clone());
    }

    for (auto& clip_data : ClipDataTemplateList)
    {
        project->ClipDataTemplateList.emplace_back(clip_data->SClone());
    }

    for (auto& motion_data : MotionDataTemplateList)
    {
        project->MotionDataTemplateList.emplace_back(motion_data->SClone());
    }

    return project;
}

void nemesis::AnimationDataProject::MatchAndUpdateHeader(const nemesis::CollectionObject& hkxfiles)
{
    std::scoped_lock<std::mutex> lock(HkxFileMutex);
    HkxFiles->MatchAndUpdate(hkxfiles);
}

void nemesis::AnimationDataProject::MatchAndUpdateHeader(const std::string& mod_code,
                                                         const nemesis::CollectionObject& hkxfiles)
{
    std::scoped_lock<std::mutex> lock(HkxFileMutex);
    HkxFiles->MatchAndUpdate(mod_code, hkxfiles);
}

const std::string& nemesis::AnimationDataProject::GetName() const
{
    return Name;
}

nemesis::AnimationDataClipData* nemesis::AnimationDataProject::GetClipData(const std::string& name,
                                                                           const std::string& code)
{
    std::scoped_lock<std::mutex> lock(ClipDataMutex);

    for (auto& clip_data : ClipDataList)
    {
        if (clip_data->GetName() != name || clip_data->GetCode() != code) continue;

        return clip_data.get();
    }

    return nullptr;
}

nemesis::AnimationDataMotionData* nemesis::AnimationDataProject::GetMotionData(const std::string& code)
{
    std::scoped_lock<std::mutex> lock(MotionDataMutex);

    for (auto& motiondata : MotionDataList)
    {
        if (motiondata->GetCode() != code) continue;

        return motiondata.get();
    }

    return nullptr;
}

UPtr<nemesis::AnimationDataClipData>&
nemesis::AnimationDataProject::AddClipData(UPtr<nemesis::AnimationDataClipData>&& clip_data)
{
    std::scoped_lock<std::mutex> lock(ClipDataMutex);
    return ClipDataList.emplace_back(std::move(clip_data));
}

UPtr<nemesis::AnimationDataMotionData>&
nemesis::AnimationDataProject::AddMotionData(UPtr<nemesis::AnimationDataMotionData>&& motion_data)
{
    std::scoped_lock<std::mutex> lock(MotionDataMutex);
    return MotionDataList.emplace_back(std::move(motion_data));
}

SPtr<nemesis::TemplateObject>&
nemesis::AnimationDataProject::AddClipDataTemplate(const SPtr<nemesis::TemplateObject>& templt_obj)
{
    std::scoped_lock<std::mutex> lock(ClipTemplateMutex);
    return ClipDataTemplateList.emplace_back(templt_obj);
}

SPtr<nemesis::TemplateObject>&
nemesis::AnimationDataProject::AddMotionDataTemplate(const SPtr<nemesis::TemplateObject>& templt_obj)
{
    std::scoped_lock<std::mutex> lock(MotionTemplateMutex);
    return MotionDataTemplateList.emplace_back(templt_obj);
}

void nemesis::AnimationDataProject::SerializeToDirectory(const std::filesystem::path& directory_path) const
{
    std::filesystem::create_directories(directory_path);
    DeqNstr lines = HkxFiles->Serialize();

    std::filesystem::path filepath = directory_path / "$header$.txt";
    std::ofstream file(filepath);

    if (!file.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    for (auto& line : lines)
    {
        file << line + "\n";
    }

    file.close();

    for (auto& clip_data : ClipDataList)
    {
        std::filesystem::path filepath = clip_data->GetName() + "~" + clip_data->GetCode() + ".txt";
        clip_data->SerializeToFile(directory_path / filepath);
    }

    for (auto& motion_data : MotionDataList)
    {
        std::filesystem::path filepath = motion_data->GetCode() + ".txt";
        motion_data->SerializeToFile(directory_path / filepath);
    }
}

UPtr<nemesis::AnimationDataProject>
nemesis::AnimationDataProject::DeserializeFromDirectory(const std::filesystem::path& directory_path)
{
    return DeserializeFromDirectory(directory_path, nemesis::to_utf8_string(directory_path.stem()));
}

UPtr<nemesis::AnimationDataProject>
nemesis::AnimationDataProject::DeserializeFromDirectory(const std::filesystem::path& directory_path,
                                                        const std::string project_name)
{
    auto project = std::make_unique<nemesis::AnimationDataProject>(project_name);

    for (auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        if (!entry.is_regular_file()) continue;

        auto path = entry.path();

        if (path.extension() != ".txt") continue;

        if (nemesis::iequals(PATH_TO_STRING(path.stem()), LITERAL_PATH("$header$")))
        {
            project->HkxFiles = DeserializeHeaderFromFile(path);
            continue;
        }

        if (PATH_TO_STRING(path.stem()).rfind(LITERAL_PATH("~")) != NOT_FOUND)
        {
            auto clip_data = nemesis::AnimationDataClipData::DeserializeFromFile(path);
            project->AddClipData(std::move(clip_data));
            continue;
        }

        auto motion_data = nemesis::AnimationDataMotionData::DeserializeFromFile(path);
        project->AddMotionData(std::move(motion_data));
    }

    return project;
}

UPtr<nemesis::AnimationDataProject>
nemesis::AnimationDataProject::DeserializeFromDirectory(const std::filesystem::path& directory_path,
                                                        nemesis::ThreadPool& threadpool)
{
    return DeserializeFromDirectory(
        directory_path, nemesis::to_utf8_string(directory_path.stem()), threadpool);
}

UPtr<nemesis::AnimationDataProject>
nemesis::AnimationDataProject::DeserializeFromDirectory(const std::filesystem::path& directory_path,
                                                        const std::string project_name,
                                                        nemesis::ThreadPool& threadpool)
{
    auto project = std::make_unique<nemesis::AnimationDataProject>(project_name);

    for (auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        if (!entry.is_regular_file()) continue;

        auto path = entry.path();

        if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".txt"))) continue;

        if (nemesis::iequals(PATH_TO_STRING(path.stem()), LITERAL_PATH("$header$")))
        {
            project->HkxFiles = DeserializeHeaderFromFile(path, threadpool);
            continue;
        }

        if (PATH_TO_STRING(path.stem()).rfind(LITERAL_PATH("~")) != NOT_FOUND)
        {
            auto clip_data = nemesis::AnimationDataClipData::DeserializeFromFile(path, threadpool);
            project->AddClipData(std::move(clip_data));
            continue;
        }

        auto motion_data = nemesis::AnimationDataMotionData::DeserializeFromFile(path, threadpool);
        project->AddMotionData(std::move(motion_data));
    }

    return project;
}

UPtr<nemesis::AnimationDataProject::Headers>
nemesis::AnimationDataProject::DeserializeHeaderFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    VecNstr lines;
    GetFileLines(filepath, lines);

    nemesis::LineStream stream(lines.begin(), lines.end());
    UPtr<nemesis::AnimationDataProject::Headers> headers
        = std::make_unique<nemesis::AnimationDataProject::Headers>();

    for (; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::MOD_OPEN:
            {
                auto list = nemesis::NObject::ParseModObjects(stream, manager);

                for (auto& each : list)
                {
                    headers->AddObject(std::move(each));
                }

                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = nemesis::NObject::ParseForEachObject(stream, manager);
                headers->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = nemesis::NObject::ParseIfObject(stream, manager);
                headers->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                headers->AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
            }
        }
    }

    return headers;
}

UPtr<nemesis::AnimationDataProject::Headers>
nemesis::AnimationDataProject::DeserializeHeaderFromFile(const std::filesystem::path& filepath,
                                                         nemesis::ThreadPool& threadpool)
{
    UPtr<nemesis::AnimationDataProject::Headers> headers
        = std::make_unique<nemesis::AnimationDataProject::Headers>();
    auto* headers_ptr = headers.get();

    threadpool.enqueue(
        [headers_ptr, filepath]()
        {
            nemesis::SemanticManager manager;
            VecNstr lines;
            GetFileLines(filepath, lines);
            nemesis::LineStream stream(lines.begin(), lines.end());

            for (; !stream.IsEoF(); ++stream)
            {
                auto& token = stream.GetToken();

                switch (token.Type)
                {
                    case nemesis::LineStream::MOD_OPEN:
                    {
                        auto list = nemesis::NObject::ParseModObjects(stream, manager);

                        for (auto& each : list)
                        {
                            headers_ptr->AddObject(std::move(each));
                        }

                        break;
                    }
                    case nemesis::LineStream::FOR_EACH:
                    {
                        auto fe_obj = nemesis::NObject::ParseForEachObject(stream, manager);
                        headers_ptr->AddObject(std::move(fe_obj));
                        break;
                    }
                    case nemesis::LineStream::IF:
                    {
                        auto if_obj = nemesis::NObject::ParseIfObject(stream, manager);
                        headers_ptr->AddObject(std::move(if_obj));
                        break;
                    }
                    case nemesis::LineStream::NONE:
                    {
                        auto& value = token.Value;
                        headers_ptr->AddObject(std::make_unique<nemesis::NLine>(
                            value, value.GetLineNumber(), value.GetFilePath(), manager));
                        break;
                    }
                    default:
                    {
                        auto& value = token.Value;
                        throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                                 + std::to_string(value.GetLineNumber())
                                                 + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
                    }
                }
            }
        });

    return headers;
}

Vec<UPtr<nemesis::AnimationDataProject>> nemesis::AnimationDataProject::ParseObjects(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, const VecNstr& project_names)
{
    Vec<UPtr<nemesis::AnimationDataProject>> project_list;
    auto name_itr = project_names.begin();

    for (; !stream.IsEoF(); ++stream)
    {
        if (name_itr == project_names.end())
        {
            throw std::runtime_error(
                "nemesis::AnimationDataProject::ParseObjects parsing error. Project name count "
                "and project body count do not match");
        }

        auto project = ParseProject(stream, manager, (*name_itr++).ToString());
        project_list.emplace_back(std::move(project));
    }

    return project_list;
}
