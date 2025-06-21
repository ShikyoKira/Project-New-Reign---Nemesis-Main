#pragma once

#include "Havok/Base/hkpBvTreeShape.h"

#include "Havok/hkpMoppCode.h"

namespace nemesis
{
    struct hkMoppBvTreeShapeBase : nemesis::hkpBvTreeShape
    {
        static constexpr nemesis::hkClass Class{0x7c338c66,
                                                "hkMoppBvTreeShapeBase",
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
        REGISTER_HAVOK_POINTER_HEADER(hkMoppBvTreeShapeBase);

        nemesis::hkRefPtr<nemesis::hkpMoppCode> m_code;
        nemesis::hkRefPtr<nemesis::HavokObject> m_moppData;
        unsigned int m_moppDataSize;
        nemesis::hkVector4 m_codeInfoCopy;

    public:
        hkMoppBvTreeShapeBase() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkMoppBvTreeShapeBase::Class;
}
