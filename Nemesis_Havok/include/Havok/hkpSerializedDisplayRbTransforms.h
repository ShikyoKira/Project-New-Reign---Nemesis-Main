#pragma once

#include "Havok/hkpSerializedDisplayRbTransformsDisplayTransformPair.h"

namespace nemesis
{
    struct hkpSerializedDisplayRbTransforms : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc18650ac,
                                                "hkpSerializedDisplayRbTransforms",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSerializedDisplayRbTransforms);

        nemesis::hkArray<nemesis::hkpSerializedDisplayRbTransformsDisplayTransformPair> m_transforms;

    public:
        hkpSerializedDisplayRbTransforms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSerializedDisplayRbTransforms::Class;
}
