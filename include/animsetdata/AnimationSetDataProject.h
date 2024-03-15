#pragma once

#include "core/NObject.h"

#include "animsetdata/AnimationSetDataState.h"

namespace nemesis
{
    struct ForEachObject;

	struct AnimationSetDataProject : public nemesis::NObject
    {
    private:
        std::string Name;
        VecNstr Headers;
        UMap<std::string, UPtr<nemesis::AnimationSetDataState>> StateMap;

        std::mutex UpdaterMutex;

        static bool IsProjectEnd(nemesis::LineStream& stream, bool& start);

        static Vec<UPtr<nemesis::NObject>>
        ParseModObjects(nemesis::LineStream& stream,
                        nemesis::SemanticManager& manager,
                        std::function<void(nemesis::NLine*)> add_nline_event);
        static UPtr<nemesis::ForEachObject>
        ParseForEachObjects(nemesis::LineStream& stream,
                            nemesis::SemanticManager& manager,
                            std::function<void(nemesis::NLine*)> add_nline_event);
        static UPtr<nemesis::IfObject> ParseIfObjects(nemesis::LineStream& stream,
                                                      nemesis::SemanticManager& manager,
                                                      std::function<void(nemesis::NLine*)> add_nline_event);

    public:
        AnimationSetDataProject(const std::string& name) noexcept;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::AnimationSetDataProject> Clone() const;

        UPtr<nemesis::AnimationSetDataState>& AddState(UPtr<nemesis::AnimationSetDataState>&& state);

        const std::string& GetName() const noexcept;
        nemesis::AnimationSetDataState* GetState(const std::string& name);
        const nemesis::AnimationSetDataState* GetState(const std::string& name) const;

        void SerializeToDirectory(const std::filesystem::path& folder_path) const;
        static UPtr<nemesis::AnimationSetDataProject> DeserializeFromDirectory(const std::filesystem::path& directory_path);
        static UPtr<nemesis::AnimationSetDataProject> DeserializeFromDirectory(const std::filesystem::path& directory_path, const std::string project_name);

    private:
        static VecNstr ParseHeaders(nemesis::LineStream& stream, nemesis::SemanticManager& manager);

    public:
        static Vec<UPtr<nemesis::AnimationSetDataProject>> ParseObjects(nemesis::LineStream& stream,
                                                                        nemesis::SemanticManager& manager,
                                                                        const VecNstr& project_names);
    };
}
