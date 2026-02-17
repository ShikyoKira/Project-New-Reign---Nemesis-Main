#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpConstraintInstanceSmallArraySerializeOverrideType.h"
#include "Havok/hkpConstraintOwner.h"
#include "Havok/hkpModifierConstraintAtom.h"
#include "Havok/hkpConstraintListener.h"

namespace nemesis
{
    struct hkpEntity;
    struct hkConstraintInternal;

    struct hkpConstraintInstance : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x034eba5f,
                                                "hkpConstraintInstance",
                                                &nemesis::hkReferencedObject::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpConstraintInstance);

        using ConstraintPriority = nemesis::hkInternalClassEnumItem<unsigned char>;
        using OnDestructionRemapInfo = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkRefPtr<nemesis::hkpConstraintOwner> m_owner;
        nemesis::hkRefPtr<nemesis::hkpConstraintData> m_data;
        nemesis::hkRefPtr<nemesis::hkpModifierConstraintAtom> m_constraintModifiers;
        std::array<nemesis::hkRefPtr<nemesis::hkpEntity>, 2> m_entities;
        nemesis::hkEnum<ConstraintPriority, unsigned char> m_priority = {
            {"PRIORITY_INVALID", 0},
            {"PRIORITY_PSI", 1},
            {"PRIORITY_SIMPLIFIED_TOI_UNUSED", 2},
            {"PRIORITY_TOI", 3},
            {"PRIORITY_TOI_HIGHER", 4},
            {"PRIORITY_TOI_FORCED", 5},
            {"NUM_PRIORITIES", 6},
        };
        bool m_wantRuntime{};
        nemesis::hkEnum<OnDestructionRemapInfo, unsigned char> m_destructionRemapInfo = {
            {"ON_DESTRUCTION_REMAP", 0},
            {"ON_DESTRUCTION_REMOVE", 1},
            {"ON_DESTRUCTION_RESET_REMOVE", 2},
        };
        nemesis::hkpConstraintInstanceSmallArraySerializeOverrideType m_listeners;
        nemesis::hkStringPtr m_name;
        nemesis::hkUlong m_userData;
        nemesis::hkConstraintInternal* m_internal = nullptr;
        unsigned int m_uid{};

    public:
        hkpConstraintInstance() noexcept;
        hkpConstraintInstance(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintInstance::Class;
}
