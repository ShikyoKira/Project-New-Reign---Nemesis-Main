#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct BSLookAtModifierBoneData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x29efee59,
                                                "BSLookAtModifierBoneData",
                                                nullptr,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSLookAtModifierBoneData);

        short m_index{};
        nemesis::hkVector4 m_fwdAxisLS;
        float m_limitAngleDegrees{};
        float m_onGain{};
        float m_offGain{};
        bool m_enabled{};
        nemesis::hkVector4 m_currentFwdAxisLS;

    public:
        BSLookAtModifierBoneData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSLookAtModifierBoneData::Class;
}
