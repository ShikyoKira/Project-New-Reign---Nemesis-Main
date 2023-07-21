#pragma once

#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/LineStream.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

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
        static Vec<UPtr<nemesis::NObject>> ParseHkxObjects(nemesis::LineStream& stream,
                                                           nemesis::SemanticManager& manager);
        static Vec<UPtr<nemesis::NObject>> ParseHkxModObjects(nemesis::LineStream& stream,
                                                              nemesis::SemanticManager& manager);
    };
}
