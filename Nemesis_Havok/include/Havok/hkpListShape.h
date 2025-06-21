#pragma once

#include "Havok/hkpListShapeChildInfo.h"
#include "Havok/hkpShapeCollection.h"

namespace nemesis
{
    struct hkpListShape : nemesis::hkpShapeCollection
    {
        static constexpr nemesis::hkClass Class{0xa1937cbd,
                                                "hkpListShape",
                                                &nemesis::hkpShapeCollection::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpListShape);

        nemesis::hkArray<nemesis::hkpListShapeChildInfo> m_childInfo;
        unsigned short m_flags;
        unsigned short m_numDisabledChildren;
        nemesis::hkVector4 m_aabbHalfExtents;
        nemesis::hkVector4 m_aabbCenter;
        std::array<unsigned int, 8> m_enabledChildren;

    public:
        hkpListShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpListShape::Class;
}
