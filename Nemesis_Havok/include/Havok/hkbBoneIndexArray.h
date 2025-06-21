#pragma once

#include "Havok/Base/hkbBindable.h"

namespace nemesis
{
    struct hkbBoneIndexArray : nemesis::hkbBindable
    {
        static constexpr nemesis::hkClass Class{0xaa8619,
                                                "hkbBoneIndexArray",
                                                &nemesis::hkbBindable::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBoneIndexArray);

        nemesis::hkArray<short> m_boneIndices;

    public:
        hkbBoneIndexArray() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBoneIndexArray::Class;
}
