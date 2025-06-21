#pragma once

#include "Core/IfObject.h"
#include "Core/ModObject.h"
#include "Core/ForEachObject.h"
#include "Core/CollectionObject.h"

#include "Utilities/ThreadPool.h"

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

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::SubTemplateObject> Clone() const;

        const std::string& GetName() const noexcept;

        static UPtr<nemesis::SubTemplateObject> ParseFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::SubTemplateObject> ParseFromFile(const std::filesystem::path& filepath, nemesis::ThreadPool& threadpool);
    };
}
