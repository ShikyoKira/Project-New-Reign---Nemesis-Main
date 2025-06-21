#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkAlignSceneToNodeOptions : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x207cb01,
                                                "hkAlignSceneToNodeOptions",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkAlignSceneToNodeOptions);

        bool m_invert;
        bool m_transformPositionX;
        bool m_transformPositionY;
        bool m_transformPositionZ;
        bool m_transformRotation;
        bool m_transformScale;
        bool m_transformSkew;
        int m_keyframe;
        mutable nemesis::hkArray<char> m_nodeName_0;
        mutable nemesis::hkStringPtr m_nodeName_1;

    public:
        hkAlignSceneToNodeOptions() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkAlignSceneToNodeOptions::Class;
}
