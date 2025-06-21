#pragma once

#include "Core/IfObject.h"
#include "Core/ModObject.h"
#include "Core/LineStream.h"
#include "Core/ForEachObject.h"
#include "Core/CollectionObject.h"

namespace nemesis
{
	struct NObjectParser
    {
    public:
        static UPtr<nemesis::NObject> ParseLine(nemesis::LineStream& stream,
                                                nemesis::SemanticManager& manager);
        static UPtr<nemesis::IfObject> ParseIfObject(nemesis::LineStream& stream,
                                                     nemesis::SemanticManager& manager);
        static UPtr<nemesis::ForEachObject> ParseForEachObject(nemesis::LineStream& stream,
                                                               nemesis::SemanticManager& manager);
        static Vec<UPtr<nemesis::NObject>> ParseModObjects(nemesis::LineStream& stream,
                                                           nemesis::SemanticManager& manager);
        static Vec<UPtr<nemesis::NObject>>
        ParseModObjects(nemesis::LineStream& stream,
                        nemesis::SemanticManager& manager,
                        std::function<bool(nemesis::LineStream&)> terminator);
        static Vec<UPtr<nemesis::NObject>> ParseHkxObjects(nemesis::LineStream& stream,
                                                           nemesis::SemanticManager& manager);
        static Vec<UPtr<nemesis::NObject>> ParseHkxModObjects(nemesis::LineStream& stream,
                                                              nemesis::SemanticManager& manager);
    };
}
