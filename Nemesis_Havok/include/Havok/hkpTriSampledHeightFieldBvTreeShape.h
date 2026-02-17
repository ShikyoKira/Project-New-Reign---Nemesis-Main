#pragma once

#include "Havok/Base/hkpBvTreeShape.h"

#include "Havok/hkpSingleShapeContainer.h"

namespace nemesis
{
    struct hkpTriSampledHeightFieldBvTreeShape : nemesis::hkpBvTreeShape
    {
        static constexpr nemesis::hkClass Class{0x58e1e585,
                                                "hkpTriSampledHeightFieldBvTreeShape",
                                                &nemesis::hkpBvTreeShape::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpTriSampledHeightFieldBvTreeShape);

        nemesis::hkpSingleShapeContainer m_childContainer;
        int m_childSize              = 0;
        bool m_wantAabbRejectionTest = false;
        std::array<unsigned char, 12> m_padding{};

    public:
        hkpTriSampledHeightFieldBvTreeShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpTriSampledHeightFieldBvTreeShape::Class;
}
