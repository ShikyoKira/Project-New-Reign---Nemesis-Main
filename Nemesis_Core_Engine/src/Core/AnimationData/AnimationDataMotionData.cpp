#include "Core/AnimationData/AnimationDataMotionData.h"

#include "Core/CollectionObject.h"
#include "Core/ForEachObject.h"
#include "Core/IfObject.h"
#include "Core/ModClass.h"
#include "Core/ModLine.h"
#include "Core/ModObject.h"
#include "Core/NLine.h"

#include "Utilities/Algorithm.h"
#include "Utilities/File.h"
#include "Utilities/FileWriter.h"
#include "Utilities/ThreadPool.h"

nemesis::AnimationDataMotionData::AnimationDataMotionData(const std::string& code) noexcept
    : Code(code)
{
}

void nemesis::AnimationDataMotionData::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr templines;

    {
        std::scoped_lock<std::mutex> lock(DataMutex);
        MotionData->CompileTo(templines, state);
    }

    if (templines.empty()) return;

    if (templines.size() < 4)
    {
        auto& value = templines.back();
        throw std::runtime_error("Invalid nemesis::AnimationDataMotionData::CompileTo format (Line: "
                                 + std::to_string(value.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
    }

    CompileStage stage = nemesis::AnimationDataMotionData::CodeStage;
    size_t counter     = 0;
    nemesis::Line* ele_ptr;

    for (auto& line : templines)
    {
        switch (stage)
        {
            case nemesis::AnimationDataMotionData::CodeStage:
            {
                if (!is_only_number(line.ToString()))
                {
                    throw std::runtime_error("Invalid nemesis::AnimationDataMotionData::Code value");
                }

                stage = nemesis::AnimationDataMotionData::LengthStage;
                break;
            }
            case nemesis::AnimationDataMotionData::LengthStage:
            {
                if (!is_only_number(line.ToString()))
                {
                    throw std::runtime_error("Invalid nemesis::AnimationDataMotionData::Length value");
                }

                stage = nemesis::AnimationDataMotionData::PositionLengthStage;
                break;
            }
            case nemesis::AnimationDataMotionData::PositionLengthStage:
            {
                if (!is_only_number(line.ToString()))
                {
                    throw std::runtime_error(
                        "Invalid nemesis::AnimationDataMotionData::PositionInfo size value");
                }

                counter = 0;
                ele_ptr = &line;
                stage   = nemesis::AnimationDataMotionData::PositionInfoStage;
                break;
            }
            case nemesis::AnimationDataMotionData::PositionInfoStage:
            {
                if (!is_only_number(line.ToString()))
                {
                    ++counter;
                    break;
                }

                *ele_ptr = std::to_string(counter);
                stage    = nemesis::AnimationDataMotionData::DirectionLengthStage;
            }
            case nemesis::AnimationDataMotionData::DirectionLengthStage:
            {
                if (!is_only_number(line.ToString()))
                {
                    throw std::runtime_error(
                        "Invalid nemesis::AnimationDataMotionData::DirectionInfo size value");
                }

                counter = 0;
                ele_ptr = &line;
                stage   = nemesis::AnimationDataMotionData::DirectionInfoStage;
                break;
            }
            case nemesis::AnimationDataMotionData::DirectionInfoStage:
            {
                ++counter;
                break;
            }
        }
    }

    if (ele_ptr)
    {
        *ele_ptr = std::to_string(counter);
    }

    for (auto& line : templines)
    {
        lines.emplace_back(std::move(line));
    }

    lines.emplace_back("");
}

void nemesis::AnimationDataMotionData::SerializeTo(DeqNstr& lines) const
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    MotionData->SerializeTo(lines);
    lines.emplace_back("");
}

UPtr<nemesis::NObject> nemesis::AnimationDataMotionData::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationDataMotionData> nemesis::AnimationDataMotionData::Clone() const
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    auto clone        = std::make_unique<nemesis::AnimationDataMotionData>(Code);
    clone->FilePath   = FilePath;
    clone->MotionData = MotionData->Clone();
    return clone;
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::Clone(const nemesis::ModClass& mod_class,
                                        const std::filesystem::path& filepath) const
{
    auto clone    = Clone();
    auto new_data = std::make_unique<nemesis::CollectionObject>();
    auto mod_obj  = std::make_unique<nemesis::ModObject>(
        mod_class.GetCode(), 0, filepath, std::move(clone->MotionData));
    new_data->AddObject(std::move(mod_obj));
    clone->MotionData = std::move(new_data);
    return clone;
}

void nemesis::AnimationDataMotionData::MatchAndUpdate(const nemesis::AnimationDataMotionData& motion_data)
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    MotionData->MatchAndUpdate(*motion_data.MotionData);
}

void nemesis::AnimationDataMotionData::MatchAndUpdate(const std::string& mod_code,
                                                      const nemesis::AnimationDataMotionData& motion_data)
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    MotionData->MatchAndUpdate(mod_code, *motion_data.MotionData);
}

const std::string& nemesis::AnimationDataMotionData::GetCode() const noexcept
{
    return Code;
}

const std::filesystem::path& nemesis::AnimationDataMotionData::GetFilePath() const noexcept
{
    return FilePath;
}

void nemesis::AnimationDataMotionData::SerializeToFile(const std::filesystem::path& filepath) const
{
    DeqNstr lines = Serialize();
    FileWriter writer(filepath);

    if (lines.empty()) return;

    lines.pop_front();

    if (!writer.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: " + nemesis::to_utf8_string(filepath)
                                 + "\nMessage: " + ec.message());
    }

    for (auto& line : lines)
    {
        writer << line;
    }
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::DeserializeFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    return DeserializeFromFile(filepath, manager);
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                      const nemesis::TemplateClass* template_class)
{
    nemesis::SemanticManager manager;
    manager.SetCurrentTemplateClass(template_class);
    return DeserializeFromFile(filepath, manager);
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                      nemesis::SemanticManager& manager)
{
    std::string code = nemesis::to_utf8_string(filepath.stem());
    auto motion_data = std::make_unique<nemesis::AnimationDataMotionData>(code);

    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty())
    {
        throw std::runtime_error("Failed to deserialize AnimationDataClipData from file (File: "
                                 + nemesis::to_utf8_string(filepath) + ")");
    }

    nemesis::LineStream stream(lines.begin(), lines.end());
    motion_data->FilePath   = stream.GetToken().Value.GetFilePath();
    motion_data->MotionData = nemesis::NObject::ParseAsCollection(stream, manager);
    return motion_data;
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                      nemesis::ThreadPool& threadpool)
{
    std::string code     = nemesis::to_utf8_string(filepath.stem());
    auto motion_data     = std::make_unique<nemesis::AnimationDataMotionData>(code);
    auto motion_data_ptr = motion_data.get();

    threadpool.enqueue(
        [filepath, motion_data_ptr]()
        {
            nemesis::SemanticManager manager;
            VecNstr lines;
            GetFileLines(filepath, lines, false);

            if (lines.empty())
            {
                throw std::runtime_error("Failed to deserialize AnimationDataClipData from file (File: "
                                         + nemesis::to_utf8_string(filepath) + ")");
            }

            nemesis::LineStream stream(lines.begin(), lines.end());
            motion_data_ptr->FilePath   = stream.GetToken().Value.GetFilePath();
            motion_data_ptr->MotionData = nemesis::NObject::ParseAsCollection(stream, manager);
        });
    return motion_data;
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                      const nemesis::TemplateClass* template_class,
                                                      nemesis::ThreadPool& threadpool)
{
    std::string code     = nemesis::to_utf8_string(filepath.stem());
    auto motion_data     = std::make_unique<nemesis::AnimationDataMotionData>(code);
    auto motion_data_ptr = motion_data.get();

    threadpool.enqueue(
        [filepath, template_class, motion_data_ptr]()
        {
            nemesis::SemanticManager manager;
            manager.SetCurrentTemplateClass(template_class);
            VecNstr lines;
            GetFileLines(filepath, lines, false);

            if (lines.empty())
            {
                throw std::runtime_error("Failed to deserialize AnimationDataClipData from file (File: "
                                         + nemesis::to_utf8_string(filepath) + ")");
            }

            nemesis::LineStream stream(lines.begin(), lines.end());
            motion_data_ptr->FilePath   = stream.GetToken().Value.GetFilePath();
            motion_data_ptr->MotionData = nemesis::NObject::ParseAsCollection(stream, manager);
        });
    return motion_data;
}

Vec<UPtr<nemesis::AnimationDataMotionData>>
nemesis::AnimationDataMotionData::ParseObjects(nemesis::LineStream& stream, nemesis::SemanticManager& manager)
{
    Vec<UPtr<nemesis::AnimationDataMotionData>> motion_list;

    if (stream.IsEoF()) return motion_list;

    std::function<void(UPtr<nemesis::NObject>&&)> add_object;

    auto& size_token = stream.GetToken();
    auto& ssize      = size_token.Value;

    if (!is_only_number(ssize.ToString()))
    {
        throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                 + std::to_string(ssize.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(ssize.GetFilePath()) + ")");
    }

    size_t size = std::stoul(ssize.ToString());

    if (size == 0) return motion_list;

    auto* ftoken = stream.GetForwardToken(size);

    if (!ftoken || !ftoken->Value.empty())
    {
        throw std::runtime_error("Invalid nemesis::AnimationDataMotionData::ParseObjects size (Line: "
                                 + std::to_string(ssize.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(ssize.GetFilePath()) + ")");
    }

    UPtr<nemesis::AnimationDataMotionData> motion_data;

    for (size_t i = 0; i < size; i++)
    {
        ++stream;
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::NONE)
        {
            auto& value = token.Value;
            throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                     + std::to_string(value.GetLineNumber())
                                     + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
        }

        auto& value = token.Value;

        if (!motion_data)
        {
            motion_data             = std::make_unique<nemesis::AnimationDataMotionData>(value);
            motion_data->FilePath   = value.GetFilePath();
            motion_data->MotionData = std::make_unique<nemesis::CollectionObject>();
        }

        if (value.empty())
        {
            motion_list.emplace_back(std::move(motion_data));
            continue;
        }

        motion_data->MotionData->AddObject(
            std::make_unique<nemesis::NLine>(value, value.GetLineNumber(), value.GetFilePath(), manager));
    }

    return motion_list;
}
