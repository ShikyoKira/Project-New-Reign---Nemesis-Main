#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpPairCollisionFilterMapPairFilterKeyOverrideType : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x36195969,
                                                "hkpPairCollisionFilterMapPairFilterKeyOverrideType",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPairCollisionFilterMapPairFilterKeyOverrideType);

        nemesis::hkRefPtr<nemesis::HavokObject> m_elem;
        int m_numElems;
        int m_hashMod;

    public:
        hkpPairCollisionFilterMapPairFilterKeyOverrideType() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPairCollisionFilterMapPairFilterKeyOverrideType::Class;
}
