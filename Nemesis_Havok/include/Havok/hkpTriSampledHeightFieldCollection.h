#pragma once

#include "Havok/hkpSampledHeightFieldShape.h"
#include "Havok/hkpShapeCollection.h"

namespace nemesis
{
    struct hkpTriSampledHeightFieldCollection : nemesis::hkpShapeCollection
    {
        static constexpr nemesis::hkClass Class{0xc291ddde,
                                                "hkpTriSampledHeightFieldCollection",
                                                &nemesis::hkpShapeCollection::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpTriSampledHeightFieldCollection);

        nemesis::hkRefPtr<nemesis::hkpSampledHeightFieldShape> m_heightfield;
        int m_childSize;
        float m_radius;
        nemesis::hkArray<unsigned short> m_weldingInfo;
        nemesis::hkVector4 m_triangleExtrusion;

    public:
        hkpTriSampledHeightFieldCollection() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpTriSampledHeightFieldCollection::Class;
}
