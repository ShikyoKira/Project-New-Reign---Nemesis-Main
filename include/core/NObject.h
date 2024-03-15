#pragma once

#include "utilities/line.h"

namespace nemesis
{
    struct NLine;
    struct IfObject;
    struct LineStream;
    struct CompileState;
    struct ForEachObject;
    struct SemanticManager;
    struct CollectionObject;

	struct NObject
    {
    public:
        static Vec<UPtr<nemesis::NObject>> ParseModObjects(nemesis::LineStream& stream,
                                                           nemesis::SemanticManager& manager);
        static Vec<UPtr<nemesis::NObject>>
        ParseModObjects(nemesis::LineStream& stream,
                        nemesis::SemanticManager& manager,
                        std::function<void(const nemesis::Line&)> add_nline_event);

        static UPtr<nemesis::ForEachObject> ParseForEachObject(nemesis::LineStream& stream,
                                                               nemesis::SemanticManager& manager);
        static UPtr<nemesis::ForEachObject>
        ParseForEachObject(nemesis::LineStream& stream,
                           nemesis::SemanticManager& manager,
                           std::function<void(const nemesis::Line&)> add_nline_event);

        static UPtr<nemesis::IfObject> ParseIfObject(nemesis::LineStream& stream,
                                                     nemesis::SemanticManager& manager);
        static UPtr<nemesis::IfObject>
        ParseIfObject(nemesis::LineStream& stream,
                      nemesis::SemanticManager& manager,
                      std::function<void(const nemesis::Line&)> add_nline_event);

        static UPtr<nemesis::CollectionObject> ParseAsCollection(nemesis::LineStream& stream,
                                                                 nemesis::SemanticManager& manager);
        static UPtr<nemesis::CollectionObject>
        ParseAsCollection(nemesis::LineStream& stream,
                          nemesis::SemanticManager& manager,
                          std::function<void(const nemesis::Line&)> add_nline_event);

        virtual void CompileTo(DeqNstr&, nemesis::CompileState&) const = 0;
        virtual void SerializeTo(DeqNstr&) const                       = 0;

        virtual DeqNstr Compile(nemesis::CompileState& state) const;
        virtual DeqNstr Serialize() const;

        virtual UPtr<nemesis::NObject> CloneNObject() const = 0;
    };
}
