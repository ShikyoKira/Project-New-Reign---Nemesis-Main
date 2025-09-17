#include <fstream>
#include <sstream>

#include "Core/AnimationData/AnimationDataClipData.h"

#include "Core/CollectionObject.h"
#include "Core/ForEachObject.h"
#include "Core/IfObject.h"
#include "Core/ModClass.h"
#include "Core/ModLine.h"
#include "Core/ModObject.h"

#include "Core/Template/TemplateClass.h"

#include "Utilities/Algorithm.h"
#include "Utilities/File.h"
#include "Utilities/Sha256.h"
#include "Utilities/ThreadPool.h"

bool nemesis::AnimationDataClipData::IsCode(const std::string& value)
{
    return is_only_number(value);
}

bool nemesis::AnimationDataClipData::IsPlaybackSpeeed(const std::string& value)
{
    return is_only_number(value);
}

bool nemesis::AnimationDataClipData::IsCropStartAmountLocalTime(const std::string& value)
{
    return is_only_number(value);
}

bool nemesis::AnimationDataClipData::IsCropEndAmountLocalTime(const std::string& value)
{
    return is_only_number(value);
}

bool nemesis::AnimationDataClipData::IsEventCounter(const std::string& value)
{
    return is_only_number(value);
}

nemesis::AnimationDataClipData::AnimationDataClipData(const std::string& name,
                                                      const std::string& code) noexcept
    : Name(name)
    , Code(code)
{
}

std::string nemesis::AnimationDataClipData::GetHash() const
{
    if (!HashCache.empty()) return HashCache;

    auto lines = Serialize();
    std::ostringstream oss;
    oss << "AnimationDataClipData:" << Name << "/" << Code << "\n";

    for (auto& line : lines)
    {
        oss << line.ToString() << "\n";
    }

    return HashCache = nemesis::SHA256::hex(oss.str());
}

void nemesis::AnimationDataClipData::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr templines;

    {
        std::scoped_lock<std::mutex> lock(DataMutex);
        ClipData->CompileTo(templines, state);
    }

    if (templines.empty()) return;

    if (templines.size() < 6)
    {
        auto& value = templines.back();
        throw nemesis::NObjectException("Invalid AnimationDataClipData. No data found (Code: " + Code + ", Line: " + std::to_string(value.GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
    }

    if (!IsCode(templines[1]))
    {
        auto& value = templines[1];
        throw nemesis::NObjectException("Invalid AnimationDataClipData. Code format (Code: " + Code
                                        + ", Line: " + std::to_string(value.GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
    }

    if (!IsPlaybackSpeeed(templines[2]))
    {
        auto& value = templines[2];
        throw nemesis::NObjectException("Invalid AnimationDataClipData. PlaybackSpeed format (Code: " + Code
                                        + ", Line: " + std::to_string(value.GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
    }

    if (!IsCropStartAmountLocalTime(templines[3]))
    {
        auto& value = templines[3];
        throw nemesis::NObjectException(
            "Invalid AnimationDataClipData. CropStartAmountLocalTime format (Code: " + Code
            + ", Line: " + std::to_string(value.GetLineNumber())
            + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
    }

    if (!IsCropEndAmountLocalTime(templines[4]))
    {
        auto& value = templines[4];
        throw nemesis::NObjectException(
            "Invalid AnimationDataClipData. CropEndAmountLocalTime format (Code: " + Code + ", Line: " + std::to_string(value.GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
    }

    if (!IsEventCounter(templines[5]))
    {
        auto& value = templines[5];
        throw nemesis::NObjectException("Invalid AnimationDataClipData. EventCounter format (Code: " + Code
                                        + ", Line: " + std::to_string(value.GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
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
    std::scoped_lock<std::mutex> lock(DataMutex);
    ClipData->SerializeTo(lines);
    lines.emplace_back("");
}

UPtr<nemesis::NObject> nemesis::AnimationDataClipData::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationDataClipData> nemesis::AnimationDataClipData::Clone() const
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    auto clone      = std::make_unique<nemesis::AnimationDataClipData>(Name, Code);
    clone->FilePath = FilePath;
    clone->ClipData = ClipData->Clone();
    return clone;
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::Clone(const nemesis::ModClass& mod_class,
                                      const std::filesystem::path& filepath) const
{
    auto clone    = Clone();
    auto new_data = std::make_unique<nemesis::CollectionObject>();
    auto mod_obj
        = std::make_unique<nemesis::ModObject>(mod_class.GetCode(), 0, filepath, std::move(clone->ClipData));
    new_data->AddObject(std::move(mod_obj));
    clone->ClipData = std::move(new_data);
    return clone;
}

void nemesis::AnimationDataClipData::MatchAndUpdate(const nemesis::AnimationDataClipData& clip_data)
{
    if (Name != clip_data.Name)
    {
        throw nemesis::NObjectException("Not matching AnimationDataClipData (Base: " + Name
                                        + " , Modified: " + clip_data.Name + ")");
    }

    if (Code != clip_data.Code)
    {
        throw nemesis::NObjectException("Not matching AnimationDataClipData (Base: " + Code
                                        + " , Modified: " + clip_data.Code + ")");
    }

    std::scoped_lock<std::mutex> lock(DataMutex);
    ClipData->MatchAndUpdate(*clip_data.ClipData);
    HashCache.clear();
}

void nemesis::AnimationDataClipData::MatchAndUpdate(const std::string& mod_code,
                                                    const nemesis::AnimationDataClipData& clip_data)
{
    if (Name != clip_data.Name)
    {
        throw nemesis::NObjectException("Not matching AnimationDataClipData (Base: " + Name
                                        + " , Modified: " + clip_data.Name + ")");
    }

    if (Code != clip_data.Code)
    {
        throw nemesis::NObjectException("Not matching AnimationDataClipData (Base: " + Code
                                        + " , Modified: " + clip_data.Code + ")");
    }

    std::scoped_lock<std::mutex> lock(DataMutex);
    ClipData->MatchAndUpdate(mod_code, *clip_data.ClipData);
    HashCache.clear();
}

const std::string& nemesis::AnimationDataClipData::GetName() const noexcept
{
    return Name;
}

const std::string& nemesis::AnimationDataClipData::GetCode() const noexcept
{
    return Code;
}

const std::filesystem::path& nemesis::AnimationDataClipData::GetFilePath() const noexcept
{
    return FilePath;
}

void nemesis::AnimationDataClipData::SerializeToFile(const std::filesystem::path& filepath) const
{
    DeqNstr lines = Serialize();
    std::ofstream file(filepath);

    if (lines.empty()) return;

    lines.pop_back();
    lines.pop_back();

    if (!file.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw nemesis::NObjectException("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                                        + "\"\nMessage: " + ec.message());
    }

    for (auto& line : lines)
    {
        file << line + "\n";
    }

    file.close();
}

void nemesis::AnimationDataClipData::Deserialize(nemesis::CollectionObject& collection,
                                                 nemesis::LineStream& stream,
                                                 nemesis::SemanticManager& manager)
{
    if (stream.IsEoF()) return;

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
                    collection.AddObject(std::move(each));
                }

                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = nemesis::NObject::ParseForEachObject(stream, manager);
                collection.AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = nemesis::NObject::ParseIfObject(stream, manager);
                collection.AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                collection.AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw nemesis::NObjectException(
                    "Syntax Error: Unsupport syntax (Line: " + std::to_string(value.GetLineNumber())
                    + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
            }
        }
    }
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::DeserializeFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    return DeserializeFromFile(filepath, manager);
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                    const nemesis::TemplateClass* template_class)
{
    nemesis::SemanticManager manager;
    manager.SetCurrentTemplateClass(template_class);
    return DeserializeFromFile(filepath, manager);
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                    nemesis::SemanticManager& manager)
{
    std::string filename = nemesis::to_utf8_string(filepath.stem());
    std::string name     = filename.substr(0, filename.rfind("~"));
    std::string code     = filename.substr(name.size() + 1);
    auto collection      = std::make_unique<nemesis::CollectionObject>();
    auto state           = std::make_unique<nemesis::AnimationDataClipData>(name, code);
    state->FilePath      = filepath;
    state->ClipData      = std::move(collection);

    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty())
    {
        throw nemesis::NObjectException("Failed to deserialize AnimationDataClipData from file (File: "
                                        + nemesis::to_utf8_string(filepath) + ")");
    }

    nemesis::LineStream stream(lines.begin(), lines.end());
    Deserialize(*collection, stream, manager);
    return state;
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                    nemesis::ThreadPool& threadpool)
{
    std::string filename = nemesis::to_utf8_string(filepath.stem());
    std::string name     = filename.substr(0, filename.rfind("~"));
    std::string code     = filename.substr(name.size() + 1);
    auto collection      = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr         = collection.get();
    auto state           = std::make_unique<nemesis::AnimationDataClipData>(name, code);
    state->FilePath      = filepath;
    state->ClipData      = std::move(collection);

    threadpool.enqueue(
        [filepath, col_ptr]()
        {
            VecNstr lines;
            GetFileLines(filepath, lines, false);

            if (lines.empty())
            {
                throw nemesis::NObjectException(
                    "Failed to deserialize AnimationDataClipData from file (File: "
                    + nemesis::to_utf8_string(filepath) + ")");
            }

            nemesis::LineStream stream(lines.begin(), lines.end());
            nemesis::SemanticManager manager;
            Deserialize(*col_ptr, stream, manager);
        });
    return state;
}

UPtr<nemesis::AnimationDataClipData>
nemesis::AnimationDataClipData::DeserializeFromFile(const std::filesystem::path& filepath,
                                                    const nemesis::TemplateClass* template_class,
                                                    nemesis::ThreadPool& threadpool)
{
    std::string filename = nemesis::to_utf8_string(filepath.stem());
    std::string name     = filename.substr(0, filename.rfind("~"));
    std::string code     = filename.substr(name.size() + 1);
    auto collection      = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr         = collection.get();
    auto state           = std::make_unique<nemesis::AnimationDataClipData>(name, code);
    state->FilePath      = filepath;
    state->ClipData      = std::move(collection);

    threadpool.enqueue(
        [filepath, template_class, col_ptr]()
        {
            VecNstr lines;
            GetFileLines(filepath, lines, false);

            if (lines.empty())
            {
                throw nemesis::NObjectException(
                    "Failed to deserialize AnimationDataClipData from file (File: "
                    + nemesis::to_utf8_string(filepath) + ")");
            }

            nemesis::LineStream stream(lines.begin(), lines.end());
            nemesis::SemanticManager manager;
            manager.SetCurrentTemplateClass(template_class);
            Deserialize(*col_ptr, stream, manager);
        });
    return state;
}

Vec<UPtr<nemesis::AnimationDataClipData>>
nemesis::AnimationDataClipData::ParseObjects(nemesis::LineStream& stream,
                                             nemesis::SemanticManager& manager,
                                             const nemesis::LineStream::Token* end_token)
{
    Vec<UPtr<nemesis::AnimationDataClipData>> clip_list;
    std::function<void(UPtr<nemesis::NObject>&&)> add_object;
    auto* token_ptr = &stream.GetToken();

    if (!token_ptr) return clip_list;

    auto& active_val = token_ptr->Value;

    if (token_ptr->Type != nemesis::LineStream::NONE)
    {
        throw nemesis::NObjectException(
            "Syntax Error: Unsupport syntax (Line: " + std::to_string(active_val.GetLineNumber())
            + ", File: " + nemesis::to_utf8_string(active_val.GetFilePath()) + ")");
    }

    if (active_val == "0") return clip_list;

    if (active_val != "1")
    {
        throw nemesis::NObjectException(
            "Invalid AnimationDataClipData::ParseObject (Line: " + std::to_string(active_val.GetLineNumber())
            + ", File: " + nemesis::to_utf8_string(active_val.GetFilePath()) + ")");
    }

    nemesis::CollectionObject* collection_ptr = nullptr;

    for (++stream; !stream.IsEoF(); ++stream)
    {
        token_ptr = &stream.GetToken();

        if (token_ptr == end_token) return clip_list;

        auto& token_val = token_ptr->Value;

        if (token_ptr->Type != nemesis::LineStream::NONE)
        {
            throw nemesis::NObjectException(
                "Syntax Error: Unsupport syntax (Line: " + std::to_string(token_val.GetLineNumber())
                + ", File: " + nemesis::to_utf8_string(token_val.GetFilePath()) + ")");
        }

        if (token_ptr->Value.empty())
        {
            collection_ptr = nullptr;
            continue;
        }

        if (!collection_ptr)
        {
            auto& name = token_ptr->Value;
            ++stream;

            if (stream.IsEoF())
            {
                throw nemesis::NObjectException("Invalid AnimationDataClipData::ParseObject. Missing "
                                                "Code and Event Counter (Clip: "
                                                + name.ToString() + ")");
            }

            token_ptr  = &stream.GetToken();
            auto& code = token_ptr->Value;

            if (stream.IsEoF())
            {
                throw nemesis::NObjectException(
                    "Invalid AnimationDataClipData::ParseObject. Missing Event Counter (Clip: "
                    + name.ToString() + ")");
            }

            auto clip_data      = std::make_unique<nemesis::AnimationDataClipData>(name, code);
            clip_data->FilePath = name.GetFilePath();
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
