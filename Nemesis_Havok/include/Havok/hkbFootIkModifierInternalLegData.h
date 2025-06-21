#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbFootIkModifierInternalLegData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xe5ca3677,
                                                "hkbFootIkModifierInternalLegData",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkModifierInternalLegData);

        nemesis::hkVector4 m_groundPosition;
        nemesis::hkRefPtr<nemesis::HavokObject> m_footIkSolver;

    public:
        hkbFootIkModifierInternalLegData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkModifierInternalLegData::Class;
}
