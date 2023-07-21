#pragma once

#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

namespace nemesis
{
	struct SubTemplateObject : public nemesis::NObject
    {
    private:
        std::string Name;
        UPtr<nemesis::CollectionObject> Data;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        const std::string& GetName() const noexcept;

        static UPtr<nemesis::SubTemplateObject> CreateFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::SubTemplateObject> CreateFromLines(const std::string& name,
                                                                const VecNstr& lines);
    };
}
