#include "animdata/AnimationDataClipData.h"

#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

#include "core/Template/TemplateClass.h"

#include "utilities/writetextfile.h"

bool nemesis::AnimationDataClipData::IsCode(const std::string& value)
{
    return isOnlyNumber(value);
}

bool nemesis::AnimationDataClipData::IsPlaybackSpeeed(const std::string& value)
{
    return isOnlyNumber(value);
}

bool nemesis::AnimationDataClipData::IsCropStartAmountLocalTime(const std::string& value)
{
    return isOnlyNumber(value);
}

bool nemesis::AnimationDataClipData::IsCropEndAmountLocalTime(const std::string& value)
{
    return isOnlyNumber(value);
}

bool nemesis::AnimationDataClipData::IsEventCounter(const std::string& value)
{
    return isOnlyNumber(value);
}

nemesis::AnimationDataClipData::AnimationDataClipData(const std::string& name,
                                                      const std::string& code) noexcept
    : Name(name)
    , Code(code)
{
}

void nemesis::AnimationDataClipData::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr templines;
    ClipData->CompileTo(templines, state);

    if (templines.empty()) return;

    if (templines.size() < 6)
    {
        throw std::runtime_error("Invalid AnimationDataClipData. No data found (Code: " + Code + ")");
    }

    if (!IsCode(templines[1]))
    {
        throw std::runtime_error("Invalid AnimationDataClipData. Code format (Code: " + Code + ")");
    }
    
    if (!IsPlaybackSpeeed(templines[2]))
    {
        throw std::runtime_error("Invalid AnimationDataClipData. PlaybackSpeed format (Code: " + Code + ")");
    }
    
    if (!IsCropStartAmountLocalTime(templines[3]))
    {
        throw std::runtime_error("Invalid AnimationDataClipData. CropStartAmountLocalTime format (Code: " + Code + ")");
    }
    
    if (!IsCropEndAmountLocalTime(templines[4]))
    {
        throw std::runtime_error("Invalid AnimationDataClipData. CropEndAmountLocalTime format (Code: " + Code + ")");
    }
    
    if (!IsEventCounter(templines[5]))
    {
        throw std::runtime_error("Invalid AnimationDataClipData. EventCounter format (Code: " + Code + ")");
    }

    for (size_t i = 0; i < 5; ++i)
    {
        lines.emplace_back(std::move(templines[i]));
    }
    
    lines.emplace_back(std::to_string(templines.size() - 6));

    for (size_t i = 6; i < templines.size(); ++i)
    {
        lines.emplace_back(std::move(templines[i]));
    }

    lines.emplace_back("");
}

void nemesis::AnimationDataClipData::SerializeTo(DeqNstr& lines) const
{
    ClipData->SerializeTo(lines);
    lines.emplace_back("");
}

UPtr<nemesis::NObject> nemesis::AnimationDataClipData::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationDataClipData> nemesis::AnimationDataClipData::Clone() const
{
    auto clone = std::make_unique<nemesis::AnimationDataClipData>(Name, Code);
    clone->ClipData = ClipData->Clone();
    return clone;
}

void nemesis::AnimationDataClipData::MatchAndUpdate(const nemesis::AnimationDataClipData& clip_data)
{
    if (Name != clip_data.Name)
    {
        throw std::runtime_error("Not matching AnimationDataClipData (Base: " + Name
                                 + " , Modified: " + clip_data.Name + ")");
    }

    if (Code != clip_data.Code)
    {
        throw std::runtime_error("Not matching AnimationDataClipData (Base: " + Code
                                 + " , Modified: " + clip_data.Code + ")");
    }

    ClipData->MatchAndUpdate(*clip_data.ClipData);
}

const std::string& nemesis::AnimationDataClipData::GetName() const
{
    return Name;
}

const std::string& nemesis::AnimationDataClipData::GetCode() const
{
    return Code;
}

void nemesis::AnimationDataClipData::SerializeToFile(const std::filesystem::path& filepath) const
{
    DeqNstr lines = Serialize();
    FileWriter writer(filepath);

    if (lines.empty()) return;

    lines.pop_back();
    lines.pop_back();

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

UPtr<nemesis::AnimationDataClipData> nemesis::AnimationDataClipData::Deserialize(const std::string& name,
                                                                                 const std::string& code,
                                                                                 nemesis::LineStream& stream)
{
    nemesis::SemanticManager manager;
    return Deserialize(name, code, stream, manager);
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::Deserialize(const std::string& name,
                                            const std::string& code,
                                            nemesis::LineStream& stream,
                                            nemesis::SemanticManager& manager)
{
    if (stream.IsEoF()) return nullptr;

    auto collection = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr    = collection.get();
    auto state      = std::make_unique<nemesis::AnimationDataClipData>(name, code);
    state->ClipData = std::move(collection);

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
                    col_ptr->AddObject(std::move(each));
                }

                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = nemesis::NObject::ParseForEachObject(stream, manager);
                col_ptr->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = nemesis::NObject::ParseIfObject(stream, manager);
                col_ptr->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                col_ptr->AddObject(std::make_unique<nemesis::NLine>(
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

    return state;
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::DeserializeFromFile(const std::filesystem::path& filepath)
{
    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty())
    {
        throw std::runtime_error(
            "Failed to deserialize AnimationDataClipData from file (File: " + filepath.string() + ")");
    }

    nemesis::LineStream stream(lines.begin(), lines.end());
    std::string filename = filepath.stem().string();
    std::string name     = filename.substr(0, filename.rfind("~"));
    std::string code     = filename.substr(name.size() + 1);
    return Deserialize(name, code, stream);
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::DeserializeFromFile(const std::filesystem::path& filepath,
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
    std::string filename = filepath.stem().string();
    std::string name     = filename.substr(0, filename.rfind("~"));
    std::string code     = filename.substr(name.size() + 1);

    nemesis::SemanticManager manager;
    manager.SetCurrentTemplateClass(template_class);
    return Deserialize(name, code, stream, manager);
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::DeserializeFromFile(const std::filesystem::path& filepath,
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
    std::string filename = filepath.stem().string();
    std::string name     = filename.substr(0, filename.rfind("~"));
    std::string code     = filename.substr(name.size() + 1);
    return Deserialize(name, code, stream, manager);
}

Vec<UPtr<nemesis::AnimationDataClipData>>
nemesis::AnimationDataClipData::ParseObjects(nemesis::LineStream& stream,
                                             nemesis::SemanticManager& manager,
                                             const nemesis::LineStream::Token* end_token)
{
    Vec<UPtr<nemesis::AnimationDataClipData>> clip_list;
    std::function<void(UPtr<nemesis::NObject> &&)> add_object;

    bool has_new_state = false;
    auto* token_ptr    = &stream.GetToken();

    if (token_ptr == nullptr) return clip_list;

    auto& active_val = token_ptr->Value;

    if (token_ptr->Type != nemesis::LineStream::NONE)
    {
        throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                 + std::to_string(active_val.GetLineNumber())
                                 + ". File: " + active_val.GetFilePath().string() + ")");
    }

    if (active_val == "0") return clip_list;

    if (active_val != "1")
    {
        throw std::runtime_error("Invalid AnimationDataClipData::ParseObject (Line: "
                                 + std::to_string(active_val.GetLineNumber())
                                 + ". File: " + active_val.GetFilePath().string() + ")");
    }

    nemesis::CollectionObject* collection_ptr = nullptr;

    for (++stream; !stream.IsEoF(); ++stream)
    {
        token_ptr = &stream.GetToken();

        if (token_ptr == end_token) return clip_list;

        auto& token_val = token_ptr->Value;

        if (token_ptr->Type != nemesis::LineStream::NONE)
        {
            throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                     + std::to_string(token_val.GetLineNumber())
                                     + ". File: " + token_val.GetFilePath().string() + ")");
        }

        if (token_ptr->Value.empty())
        {
            collection_ptr = nullptr;
            continue;
        }

        if (collection_ptr == nullptr)
        {
            auto& name = token_ptr->Value;
            ++stream;

            if (stream.IsEoF())
            {
                throw std::runtime_error("Invalid AnimationDataClipData::ParseObject. Missing "
                                         "Code and Event Counter (Clip: "
                                         + name.ToString() + ")");
            }

            token_ptr  = &stream.GetToken();
            auto& code = token_ptr->Value;

            if (stream.IsEoF())
            {
                throw std::runtime_error(
                    "Invalid AnimationDataClipData::ParseObject. Missing Event Counter (Clip: "
                    + name.ToString() + ")");
            }

            auto clip_data      = std::make_unique<nemesis::AnimationDataClipData>(name, code);
            clip_data->ClipData = std::make_unique<nemesis::CollectionObject>();
            collection_ptr      = clip_data->ClipData.get();
            collection_ptr->AddObject(
                std::make_unique<nemesis::NLine>(name, name.GetLineNumber(), name.GetFilePath(), manager));
            collection_ptr->AddObject(
                std::make_unique<nemesis::NLine>(code, code.GetLineNumber(), code.GetFilePath(), manager));
            clip_list.emplace_back(std::move(clip_data));
            continue;
        }

        auto& token_value = token_ptr->Value;
        collection_ptr->AddObject(std::make_unique<nemesis::NLine>(
            token_value, token_value.GetLineNumber(), token_value.GetFilePath(), manager));
    }

    return clip_list;
}
