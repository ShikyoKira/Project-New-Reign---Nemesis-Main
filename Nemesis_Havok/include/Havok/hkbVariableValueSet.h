#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkArray.h"
#include "Havok/hkClass.h"
#include "Havok/hkRefPtr.h"
#include "Havok/hkbVariableValue.h"

#include "Havok/Types/hkVector4.h"

namespace nemesis
{
    struct hkbVariableValueSet : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x27812d8d,
                                                "hkbVariableValueSet",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbVariableValueSet);

        nemesis::hkArray<nemesis::hkbVariableValue> m_wordVariableValues;
        nemesis::hkArray<nemesis::hkVector4> m_quadVariableValues;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkReferencedObject>> m_variantVariableValues;

    public:
        hkbVariableValueSet() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbVariableValueSet::Class;
}
