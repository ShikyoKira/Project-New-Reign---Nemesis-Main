#pragma once

#include "Havok/Base/hkLocalFrame.h"

#include "Havok/hkClass.h"
#include "Havok/hkLocalFrameGroup.h"

namespace nemesis
{
    struct hkSimpleLocalFrame : nemesis::hkLocalFrame
    {
        static constexpr nemesis::hkClass Class{0xe758f63c,
                                                "hkSimpleLocalFrame",
                                                &nemesis::hkLocalFrame::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkSimpleLocalFrame);

        nemesis::hkTransform m_transform;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkLocalFrame>> m_children;
        nemesis::hkRefPtr<nemesis::hkLocalFrame> m_parentFrame;
        nemesis::hkRefPtr<nemesis::hkLocalFrameGroup> m_group;
        nemesis::hkStringPtr m_name;

    public:
        hkSimpleLocalFrame() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkSimpleLocalFrame::Class;
}
