#include "Core/NObjectParser.h"
#include "Core/SemanticManager.h"
#include "Core/SubTemplateObject.h"
#include "Core/CompileState.h"

#include "Utilities/File.h"

#include "Logger.h"


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
    Logger::Log(L"Processing SubTemplate: " + filepath.wstring());

    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty()) return nullptr;

    UPtr<nemesis::SubTemplateObject> templt(new nemesis::SubTemplateObject());
    templt->Name  = filepath.stem().string();
    templt->Data  = std::make_unique<nemesis::CollectionObject>();
    auto& col_ref = *templt->Data;

    nemesis::SemanticManager manager;

    for (nemesis::LineStream stream(lines.begin(), lines.end()); !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::TokenType::MOD_OPEN)
        {
            col_ref.AddObject(nemesis::NObjectParser::ParseLine(stream, manager));
            continue;
        }

        auto objects = nemesis::NObjectParser::ParseModObjects(stream, manager);

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
    Logger::Log(L"Processing SubTemplate: " + filepath.wstring());

    VecNstr lines;
    GetFileLines(filepath, lines, false);

    if (lines.empty()) return nullptr;

    UPtr<nemesis::SubTemplateObject> templt(new nemesis::SubTemplateObject());
    templt->Name = filepath.stem().string();
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
                    col_ptr->AddObject(nemesis::NObjectParser::ParseLine(stream, manager));
                    continue;
                }

                auto objects = nemesis::NObjectParser::ParseModObjects(stream, manager);

                for (auto& object : objects)
                {
                    col_ptr->AddObject(std::move(object));
                }
            }
        });

    return templt;
}
