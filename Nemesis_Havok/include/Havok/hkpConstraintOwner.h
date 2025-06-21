#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkpConstraintInfo.h"

namespace nemesis
{
    struct hkpConstraintOwner : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0,
                                                "hkpConstraintOwner",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpConstraintOwner);

        nemesis::hkpConstraintInfo m_constraintInfo;

    public:
        hkpConstraintOwner() noexcept;
        hkpConstraintOwner(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintOwner::Class;
}
