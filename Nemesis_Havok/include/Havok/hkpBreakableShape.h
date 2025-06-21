#pragma once

#include "Havok/Base/hkpBreakableMaterial.h"
#include "Havok/Base/hkcdShape.h"

namespace nemesis
{
    struct hkpBreakableShape : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0,
                                                "hkpBreakableShape",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpBreakableShape);

        nemesis::hkRefPtr<nemesis::hkcdShape> m_physicsShape;
        nemesis::hkArray<nemesis::hkpBreakableMaterial> m_material;

    public:
        hkpBreakableShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBreakableShape::Class;
}
