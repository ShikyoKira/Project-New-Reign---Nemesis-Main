#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpCompressedMeshShapeBigTriangle : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xcbfc95a4,
                                                "hkpCompressedMeshShapeBigTriangle",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpCompressedMeshShapeBigTriangle);

        unsigned short m_a{};
        unsigned short m_b{};
        unsigned short m_c{};
        unsigned int m_material{};
        unsigned short m_weldingInfo{};
        unsigned short m_transformIndex{};

    public:
        hkpCompressedMeshShapeBigTriangle() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCompressedMeshShapeBigTriangle::Class;
}
