#pragma once

#include "Havok/Base/hkpShape.h"
#include "Havok/Base/hkpShapeContainer.h"

namespace nemesis
{
    struct hkpSingleShapeContainer : nemesis::hkpShapeContainer
    {
        static constexpr nemesis::hkClass Class{0x73aa1d38,
                                                "hkpSingleShapeContainer",
                                                &nemesis::hkpShapeContainer::Class,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSingleShapeContainer);

        nemesis::hkRefPtr<nemesis::hkpShape> m_childShape;

    public:
        hkpSingleShapeContainer() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSingleShapeContainer::Class;
}
