#include "animdata/AnimationDataMotionData.h"

#include "core/NLine.h"
#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

#include "utilities/writetextfile.h"

nemesis::AnimationDataMotionData::AnimationDataMotionData(const std::string& code) noexcept
    : Code(code)
{
}

void nemesis::AnimationDataMotionData::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr templines;
    MotionData->CompileTo(templines, state);

    if (templines.empty()) return;

    if (templines.size() < 4)
    {
        auto& value = templines.back();
        throw std::runtime_error("Invalid nemesis::AnimationDataMotionData::CompileTo format (Line: "
                                 + std::to_string(value.GetLineNumber())
                                 + ". File: " + value.GetFilePath().string() + ")");
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
                if (!isOnlyNumber(line))
                {
                    throw std::runtime_error(
                        "Invalid nemesis::AnimationDataMotionData::Code value");
                }

                stage = nemesis::AnimationDataMotionData::LengthStage;
                break;
            }
            case nemesis::AnimationDataMotionData::LengthStage:
            {
                if (!isOnlyNumber(line))
                {
                    throw std::runtime_error(
                        "Invalid nemesis::AnimationDataMotionData::Length value");
                }

                stage = nemesis::AnimationDataMotionData::PositionLengthStage;
                break;
            }
            case nemesis::AnimationDataMotionData::PositionLengthStage:
            {
                if (!isOnlyNumber(line))
                {
                    throw std::runtime_error("Invalid nemesis::AnimationDataMotionData::PositionInfo size value");
                }

                counter = 0;
                ele_ptr = &line;
                stage   = nemesis::AnimationDataMotionData::PositionInfoStage;
                break;
            }
            case nemesis::AnimationDataMotionData::PositionInfoStage:
            {
                if (!isOnlyNumber(line))
                {
                    ++counter;
                    break;
                }

                *ele_ptr = std::to_string(counter);
                counter  = 0;
                ele_ptr  = &line;
                stage = nemesis::AnimationDataMotionData::DirectionInfoStage;
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
    MotionData->SerializeTo(lines);
    lines.emplace_back("");
}

UPtr<nemesis::NObject> nemesis::AnimationDataMotionData::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationDataMotionData> nemesis::AnimationDataMotionData::Clone() const
{
    auto clone        = std::make_unique<nemesis::AnimationDataMotionData>(Code);
    clone->MotionData = MotionData->Clone();
    return clone;
}

void nemesis::AnimationDataMotionData::MatchAndUpdate(const nemesis::AnimationDataMotionData& motion_data)
{
    MotionData->MatchAndUpdate(*motion_data.MotionData);
}

const std::string& nemesis::AnimationDataMotionData::GetCode() const
{
    return Code;
}

void nemesis::AnimationDataMotionData::SerializeToFile(const std::filesystem::path& filepath) const
{
    DeqNstr lines = Serialize();
    FileWriter writer(filepath);

    if (lines.empty()) return;

    lines.pop_front();

    if (!writer.is_open())
    {
        throw std::runtime_error("Failed to open file (File: " + filepath.string()
                                 + ", Error: " + std::string(std::strerror(errno)) + ")");
    }

    for (auto& line : lines)
    {
        writer << line;
    }
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::Deserialize(const std::string& code, nemesis::LineStream& stream)
{
    nemesis::SemanticManager manager;
    return Deserialize(code, stream, manager);
}

UPtr<nemesis::AnimationDataMotionData> nemesis::AnimationDataMotionData::Deserialize(
    const std::string& code, nemesis::LineStream& stream, nemesis::SemanticManager& manager)
{
    if (stream.IsEoF()) return nullptr;

    auto motion_data        = std::make_unique<nemesis::AnimationDataMotionData>(code);
    motion_data->MotionData = nemesis::NObject::ParseAsCollection(stream, manager);
    return motion_data;
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::DeserializeFromFile(const std::filesystem::path& filepath)
{
    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty())
    {
        throw std::runtime_error(
            "Failed to deserialize AnimationDataClipData from file (File: " + filepath.string() + ")");
    }

    nemesis::LineStream stream(lines.begin(), lines.end());
    return Deserialize(filepath.stem().string(), stream);
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                      const nemesis::TemplateClass* template_class)
{
    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty())
    {
        throw std::runtime_error(
            "Failed to deserialize AnimationDataClipData from file (File: " + filepath.string() + ")");
    }

    nemesis::LineStream stream(lines.begin(), lines.end());
    nemesis::SemanticManager manager;
    manager.SetCurrentTemplateClass(template_class);
    return Deserialize(filepath.stem().string(), stream, manager);
}

UPtr<nemesis::AnimationDataMotionData>
nemesis::AnimationDataMotionData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                      nemesis::SemanticManager& manager)
{
    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty())
    {
        throw std::runtime_error(
            "Failed to deserialize AnimationDataClipData from file (File: " + filepath.string() + ")");
    }

    nemesis::LineStream stream(lines.begin(), lines.end());
    return Deserialize(filepath.stem().string(), stream, manager);
}

Vec<UPtr<nemesis::AnimationDataMotionData>>
nemesis::AnimationDataMotionData::ParseObjects(nemesis::LineStream& stream, nemesis::SemanticManager& manager)
{
    Vec<UPtr<nemesis::AnimationDataMotionData>> motion_list;

    if (stream.IsEoF()) return motion_list;

    std::function<void(UPtr<nemesis::NObject> &&)> add_object;

    auto& size_token = stream.GetToken();
    auto& ssize      = size_token.Value;

    if (!isOnlyNumber(ssize))
    {
        throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                 + std::to_string(ssize.GetLineNumber())
                                 + ". File: " + ssize.GetFilePath().string() + ")");
    }

    size_t size  = std::stoul(ssize);

    if (size == 0) return motion_list;
    
    auto* ftoken = stream.GetForwardToken(size);

    if (ftoken == nullptr || !ftoken->Value.empty())
    {
        throw std::runtime_error("Invalid nemesis::AnimationDataMotionData::ParseObjects size (Line: "
                                 + std::to_string(ssize.GetLineNumber())
                                 + ". File: " + ssize.GetFilePath().string() + ")");
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
                                     + ". File: " + value.GetFilePath().string() + ")");
        }

        auto& value = token.Value;

        if (motion_data == nullptr)
        {
            motion_data             = std::make_unique<nemesis::AnimationDataMotionData>(value);
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
