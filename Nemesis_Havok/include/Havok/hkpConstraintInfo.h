#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpConstraintInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xd6421f19,
                                                "hkpConstraintInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpConstraintInfo);

        int m_maxSizeOfSchema;
        int m_sizeOfSchemas;
        int m_numSolverResults;
        int m_numSolverElemTemps;

    public:
        hkpConstraintInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintInfo::Class;
}
