#include "animdata/AnimationDataProject.h"

#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/ForEachObject.h"
#include "core/SemanticManager.h"
#include "core/CollectionObject.h"

#include "utilities/writetextfile.h"

UPtr<nemesis::CollectionObject>
nemesis::AnimationDataProject::DeserailizeHeaderFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    VecNstr lines;
    GetFileLines(filepath, lines);

    nemesis::LineStream stream(lines.begin(), lines.end());
    UPtr<nemesis::CollectionObject> collection = std::make_unique<nemesis::CollectionObject>();

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
                    collection->AddObject(std::move(each));
                }

                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = nemesis::NObject::ParseForEachObject(stream, manager);
                collection->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = nemesis::NObject::ParseIfObject(stream, manager);
                collection->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                collection->AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ". File: " + value.GetFilePath().string() + ")");
            }
        }
    }

    return collection;
}

UPtr<nemesis::AnimationDataProject> nemesis::AnimationDataProject::ParseProject(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, const std::string& project_name)
{
    auto& stoken = stream.GetToken();
    auto& ssize  = stoken.Value;

    if (stoken.Type != nemesis::LineStream::NONE || !isOnlyNumber(ssize))
    {
        throw std::runtime_error("Invalid nemesis::AnimationDataProject::ParseProject size (Line: "
                                 + std::to_string(ssize.GetLineNumber())
                                 + ". File: " + ssize.GetFilePath().string() + ")");
    }

    size_t fsize;
    size_t size     = std::stoul(ssize);
    auto* end_token = stream.GetForwardToken(size);

    if (end_token == nullptr)
    {
        throw std::runtime_error("Invalid nemesis::AnimationDataProject::ParseProject size (Line: "
                                 + std::to_string(ssize.GetLineNumber())
                                 + ". File: " + ssize.GetFilePath().string() + ")");
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
                                     + ". File: " + value.GetFilePath().string() + ")");
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

                if (!isOnlyNumber(value))
                {
                    throw std::runtime_error(
                        "Invalid nemesis::AnimationDataProject::ParseProject format (Line: "
                        + std::to_string(value.GetLineNumber()) + ". File: " + value.GetFilePath().string()
                        + ")");
                }

                fsize             = std::stoul(value);
                project->HkxFiles = std::make_unique<nemesis::CollectionObject>();

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

                if (!isOnlyNumber(value))
                {
                    throw std::runtime_error(
                        "Invalid nemesis::AnimationDataProject::ParseObjects format (Line: "
                        + std::to_string(value.GetLineNumber()) + ". File: " + value.GetFilePath().string()
                        + ")");
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
    HkxFiles->CompileTo(hkxfile_lines, state);

    for (auto& clip_data : ClipDataList)
    {
        clip_data->CompileTo(clipdata_lines, state);
    }

    for (auto& motion_data : MotionDataList)
    {
        motion_data->CompileTo(motiondata_lines, state);
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
    auto project      = std::make_unique<nemesis::AnimationDataProject>(Name);
    project->HkxFiles = HkxFiles->Clone();

    for (auto& clip_data : ClipDataList)
    {
        project->ClipDataList.emplace_back(clip_data->Clone());
    }

    for (auto& motion_data : MotionDataList)
    {
        project->MotionDataList.emplace_back(motion_data->Clone());
    }

    return project;
}

const std::string& nemesis::AnimationDataProject::GetName() const
{
    return Name;
}

nemesis::AnimationDataClipData* nemesis::AnimationDataProject::GetClipData(const std::string& name,
                                                                           const std::string& code)
{
    for (auto& clip_data : ClipDataList)
    {
        if (clip_data->GetName() != name || clip_data->GetCode() != code) continue;

        return clip_data.get();
    }

    return nullptr;
}

nemesis::AnimationDataMotionData* nemesis::AnimationDataProject::GetMotionData(const std::string& code)
{
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
    return ClipDataList.emplace_back(std::move(clip_data));
}

UPtr<nemesis::AnimationDataMotionData>&
nemesis::AnimationDataProject::AddMotionData(UPtr<nemesis::AnimationDataMotionData>&& motion_data)
{
    return MotionDataList.emplace_back(std::move(motion_data));
}

void nemesis::AnimationDataProject::SerializeToDirectory(const std::filesystem::path& directory_path) const
{
    std::filesystem::create_directories(directory_path);
    DeqNstr lines = HkxFiles->Serialize();

    std::filesystem::path filepath = directory_path / "Header.txt";
    FileWriter writer(filepath);

    if (!writer.is_open())
    {
        throw std::runtime_error("Failed to open file (File: " + filepath.string()
                                 + ", Error: " + std::string(std::strerror(errno)) + ")");
    }

    for (auto& line : lines)
    {
        writer << line;
    }

    writer.Close();

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
    return DeserializeFromDirectory(directory_path, directory_path.stem().string());
}

UPtr<nemesis::AnimationDataProject>
nemesis::AnimationDataProject::DeserializeFromDirectory(const std::filesystem::path& directory_path,
                                                        const std::string project_name)
{
    auto project  = std::make_unique<nemesis::AnimationDataProject>(project_name);

    for (auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        if (!entry.is_regular_file()) continue;

        auto path = entry.path();

        if (path.extension() != ".txt") continue;

        if (nemesis::iequals(path.filename().string(), "header.txt"))
        {
            project->HkxFiles = DeserailizeHeaderFromFile(path);
            continue;
        }

        if (path.stem().string().rfind("~") != NOT_FOUND)
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

        auto project = ParseProject(stream, manager, *name_itr++);
        project_list.emplace_back(std::move(project));
    }

    return project_list;
}
