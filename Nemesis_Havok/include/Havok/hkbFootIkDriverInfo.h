#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbFootIkDriverInfoLeg.h"

namespace nemesis
{
    struct hkbFootIkDriverInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc6a09dbf,
                                                "hkbFootIkDriverInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkDriverInfo);

        nemesis::hkArray<nemesis::hkbFootIkDriverInfoLeg> m_legs;
        float m_raycastDistanceUp{};
        float m_raycastDistanceDown{};
        float m_originalGroundHeightMS{};
        float m_verticalOffset{};
        unsigned int m_collisionFilterInfo{};
        float m_forwardAlignFraction{};
        float m_sidewaysAlignFraction{};
        float m_sidewaysSampleWidth{};
        bool m_lockFeetWhenPlanted{};
        bool m_useCharacterUpVector{};
        bool m_isQuadrupedNarrow{};
        bool m_keepSourceFootEndAboveGround{};

    public:
        hkbFootIkDriverInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkDriverInfo::Class;
}
