#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkLocalFrameGroup : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xb1a96c2f,
                                                "hkLocalFrameGroup",
                                                &nemesis::hkReferencedObject::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkLocalFrameGroup);

        nemesis::hkStringPtr m_name;

    public:
        hkLocalFrameGroup() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkLocalFrameGroup::Class;
}
