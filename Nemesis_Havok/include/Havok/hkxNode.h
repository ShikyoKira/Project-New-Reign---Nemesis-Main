#pragma once

#include "Havok/Base/hkxAttributeHolder.h"
#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxNode.h"
#include "Havok/hkxNodeAnnotationData.h"

namespace nemesis
{
    struct hkxNode : nemesis::hkxAttributeHolder
    {
        static constexpr nemesis::hkClass Class{0x5a218502,
                                                "hkxNode",
                                                &nemesis::hkxAttributeHolder::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxNode);

        nemesis::hkStringPtr m_name;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_object;
        nemesis::hkArray<nemesis::hkMatrix4> m_keyFrames;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxNode>> m_children;
        nemesis::hkArray<nemesis::hkxNodeAnnotationData> m_annotations;
        nemesis::hkArray<float> m_linearKeyFrameHints;
        nemesis::hkStringPtr m_userProperties;
        bool m_selected;
        bool m_bone;

    public:
        hkxNode() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxNode::Class;
}
