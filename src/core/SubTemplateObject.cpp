#include "core/NObjectParser.h"
#include "core/SemanticManager.h"
#include "core/SubTemplateObject.h"

#include "core/CompileState.h"

void nemesis::SubTemplateObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    Data->CompileTo(lines, state);
}

void nemesis::SubTemplateObject::SerializeTo(DeqNstr& lines) const
{
    Data->SerializeTo(lines);
}

const std::string& nemesis::SubTemplateObject::GetName() const noexcept
{
    return Name;
}

UPtr<nemesis::SubTemplateObject>
nemesis::SubTemplateObject::CreateFromFile(const std::filesystem::path& filepath)
{
    VecNstr lines;
    GetFileLines(filepath, lines, false);
    return nemesis::SubTemplateObject::CreateFromLines(filepath.stem().string(), lines);
}

UPtr<nemesis::SubTemplateObject> nemesis::SubTemplateObject::CreateFromLines(const std::string& name,
                                                                             const VecNstr& lines)
{
    UPtr<nemesis::SubTemplateObject> templt(new nemesis::SubTemplateObject());
    templt->Name  = name;
    templt->Data  = std::make_unique<nemesis::CollectionObject>();
    auto& col_ref = *templt->Data;

    nemesis::SemanticManager manager;

    for (nemesis::LineStream stream(lines.begin(), lines.end()); !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::TokenType::MOD_OPEN)
        {
            col_ref.AddObject(std::move(nemesis::NObjectParser::ParseLine(stream, manager)));
            continue;
        }

        auto objects = std::move(nemesis::NObjectParser::ParseModObjects(stream, manager));

        for (auto& object : objects)
        {
            col_ref.AddObject(std::move(object));
        }
    }

    return templt;
}
