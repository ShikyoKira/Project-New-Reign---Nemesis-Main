#include "Core/SubTemplateObject.h"
#include "Core/CompileState.h"
#include "Core/LineStream.h"
#include "Core/SemanticManager.h"

#include "Utilities/Algorithm.h"
#include "Utilities/File.h"

#include "Logger.h"

std::string nemesis::SubTemplateObject::GetHash() const
{
    if (!HashCache.empty()) return HashCache;

    auto lines = Serialize();
    std::ostringstream oss;
    oss << "SubTemplateObject:" << Name << "\n";

    for (auto& line : lines)
    {
        oss << line.ToString() << "\n";
    }

    return HashCache = nemesis::SHA256::hex(oss.str());
}

void nemesis::SubTemplateObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    Data->CompileTo(lines, state);
}

void nemesis::SubTemplateObject::SerializeTo(DeqNstr& lines) const
{
    Data->SerializeTo(lines);
}

UPtr<nemesis::NObject> nemesis::SubTemplateObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::SubTemplateObject> nemesis::SubTemplateObject::Clone() const
{
    auto subtmplt  = std::make_unique<nemesis::SubTemplateObject>();
    subtmplt->Name = Name;
    subtmplt->Data = Data->Clone();
    return subtmplt;
}

const std::string& nemesis::SubTemplateObject::GetName() const noexcept
{
    return Name;
}

UPtr<nemesis::SubTemplateObject>
nemesis::SubTemplateObject::ParseFromFile(const std::filesystem::path& filepath)
{
    Logger::Log(LITERAL_PATH("Processing SubTemplate: ") + PATH_TO_STRING(filepath));

    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty()) return nullptr;

    UPtr<nemesis::SubTemplateObject> templt(new nemesis::SubTemplateObject());
    templt->Name  = nemesis::to_utf8_string(filepath.stem());
    templt->Data  = std::make_unique<nemesis::CollectionObject>();
    auto& col_ref = *templt->Data;

    nemesis::SemanticManager manager;

    for (nemesis::LineStream stream(lines.begin(), lines.end()); !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::TokenType::MOD_OPEN)
        {
            col_ref.AddObject(nemesis::NObject::ParseLine(stream, manager));
            continue;
        }

        auto objects = nemesis::NObject::ParseModObjects(stream, manager);

        for (auto& object : objects)
        {
            col_ref.AddObject(std::move(object));
        }
    }

    return templt;
}

UPtr<nemesis::SubTemplateObject>
nemesis::SubTemplateObject::ParseFromFile(const std::filesystem::path& filepath,
                                          nemesis::ThreadPool& threadpool)
{
    Logger::Log(LITERAL_PATH("Processing SubTemplate: ") + PATH_TO_STRING(filepath));

    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty()) return nullptr;

    UPtr<nemesis::SubTemplateObject> templt(new nemesis::SubTemplateObject());
    templt->Name = nemesis::to_utf8_string(filepath.stem());
    templt->Data = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr = templt->Data.get();

    threadpool.enqueue(
        [lines, col_ptr]
        {
            nemesis::SemanticManager manager;

            for (nemesis::LineStream stream(lines.begin(), lines.end()); !stream.IsEoF(); ++stream)
            {
                auto& token = stream.GetToken();

                if (token.Type != nemesis::LineStream::TokenType::MOD_OPEN)
                {
                    col_ptr->AddObject(nemesis::NObject::ParseLine(stream, manager));
                    continue;
                }

                auto objects = nemesis::NObject::ParseModObjects(stream, manager);

                for (auto& object : objects)
                {
                    col_ptr->AddObject(std::move(object));
                }
            }
        });

    return templt;
}
