#pragma once

#include "Havok/hkpConstraintInstance.h"

namespace nemesis
{
    struct hkpEntity;

    struct hkConstraintInternal : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xf056d145,
                                                "hkConstraintInternal",
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
        REGISTER_HAVOK_POINTER_HEADER(hkConstraintInternal);

        using ConstraintPriority = nemesis::hkInternalClassEnumItem<unsigned char>;
        using InstanceType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkRefPtr<nemesis::hkpConstraintInstance> m_constraint;
        std::array<nemesis::hkRefPtr<nemesis::hkpEntity>, 2> m_entities;
        nemesis::hkRefPtr<nemesis::hkpConstraintAtom> m_atoms;
        unsigned short m_atomsSize;
        unsigned char m_callbackRequest;
        nemesis::hkEnum<ConstraintPriority, unsigned char> m_priority = {
            {"PRIORITY_INVALID", 0},
            {"PRIORITY_PSI", 1},
            {"PRIORITY_SIMPLIFIED_TOI_UNUSED", 2},
            {"PRIORITY_TOI", 3},
            {"PRIORITY_TOI_HIGHER", 4},
            {"PRIORITY_TOI_FORCED", 5},
            {"NUM_PRIORITIES", 6},
        };
        unsigned short m_sizeOfSchemas;
        unsigned short m_numSolverResults;
        unsigned short m_numSolverElemTemps;
        unsigned char m_whoIsMaster;
        nemesis::hkEnum<InstanceType, unsigned char> m_constraintType = {
            {"TYPE_NORMAL", 0},
            {"TYPE_CHAIN", 1},
            {"TYPE_DISABLE_SPU", 2},
        };
        nemesis::hkRefPtr<nemesis::HavokObject> m_runtime;
        unsigned short m_runtimeSize;
        unsigned short m_slaveIndex;

    public:
        hkConstraintInternal() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkConstraintInternal::Class;
}
