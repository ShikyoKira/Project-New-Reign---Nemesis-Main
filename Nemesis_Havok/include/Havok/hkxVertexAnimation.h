#pragma once

#include "Havok/hkxVertexAnimationUsageMap.h"
#include "Havok/hkxVertexBuffer.h"

namespace nemesis
{
    struct hkxVertexAnimation : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x27678cb3,
                                                "hkxVertexAnimation",
                                                &nemesis::hkReferencedObject::Class,
                                                192,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexAnimation);

        float m_time;
        nemesis::hkxVertexBuffer m_vertData;
        nemesis::hkArray<int> m_vertexIndexMap;
        nemesis::hkArray<nemesis::hkxVertexAnimationUsageMap> m_componentMap;

    public:
        hkxVertexAnimation() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexAnimation::Class;
}
