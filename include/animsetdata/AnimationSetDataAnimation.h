//#pragma once
//
//#include "core/NLine.h"
//#include "core/IfObject.h"
//#include "core/LineStream.h"
//#include "core/CollectionObject.h"
//
//namespace nemesis
//{
//
//	struct AnimationSetDataAnimation : public nemesis::NObject
//    {
//    private:
//        struct Parser
//        {
//        private:
//            enum ParsingObjectType
//            {
//                EVENT_NAME,
//                UNKNOWN,
//                ANIMATION_LIST_COUNT,
//                ANIMATION_NAME,
//            };
//
//            struct Writer
//            {
//            private:
//                size_t Index = 0;
//                Vec<UPtr<nemesis::NObject>> StagedObjects;
//                ParsingObjectType ParseType;
//
//            public:
//                Writer(ParsingObjectType parse_type);
//
//                void Prepare(UPtr<nemesis::NObject>&& object);
//                void Consume();
//                void WriteTo(UPtr<nemesis::NObject>& target);
//                bool IsEnd() const;
//            };
//
//            UPtr<nemesis::AnimationSetDataAnimation> AnimationObject;
//            UPtr<nemesis::CollectionObject> AnimationNameList;
//            Vec<ParsingObjectType> ParseTypeLayer;
//            Vec<Writer> Writers;
//
//            UPtr<nemesis::NObject> ParseLine(nemesis::LineStream& stream,
//                                                    nemesis::SemanticManager& manager);
//            UPtr<nemesis::IfObject> ParseIfObject(nemesis::LineStream& stream,
//                                                         nemesis::SemanticManager& manager);
//            UPtr<nemesis::ForEachObject> ParseForEachObject(nemesis::LineStream& stream,
//                                                                   nemesis::SemanticManager& manager);
//            Vec<UPtr<nemesis::NObject>> ParseModObjects(nemesis::LineStream& stream,
//                                                               nemesis::SemanticManager& manager);
//
//            bool IsAnimationEnd(nemesis::LineStream& stream);
//            void SetValue(nemesis::LineStream& stream, nemesis::SemanticManager& manager);
//            void SetModValue(nemesis::LineStream& stream, nemesis::SemanticManager& manager);
//
//        public:
//            UPtr<nemesis::NObject> ParseObject(nemesis::LineStream& stream,
//                                                      nemesis::SemanticManager& manager);
//        };
//
//        UPtr<nemesis::NObject> EventName;
//        UPtr<nemesis::NObject> Unknown;
//        UPtr<nemesis::CollectionObject> AnimationNameList;
//
//        static bool IsAnimationEnd(nemesis::LineStream& stream);
//
//
//    public:
//        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
//        void SerializeTo(DeqNstr& lines) const override;
//
//        void MatchAndUpdate(nemesis::AnimationSetDataAnimation& animation);
//
//        static UPtr<nemesis::NObject> ParseObject(nemesis::LineStream& stream,
//                                                  nemesis::SemanticManager& manager);
//    };
//
//    struct AnimationSetDataAnimationList : public nemesis::NObject
//    {
//    private:
//        Vec<UPtr<nemesis::NObject>> AnimationList;
//
//    public:
//        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
//        void SerializeTo(DeqNstr& lines) const override;
//
//        void AddAnimation(UPtr<nemesis::NObject>&& animation);
//        void MatchAndUpdate(nemesis::AnimationSetDataAnimationList& animation_list);
//
//        static UPtr<nemesis::NObject> ParseObject(nemesis::LineStream& stream,
//                                                  nemesis::SemanticManager& manager);
//    };
//}
