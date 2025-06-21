#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkArray.h"
#include "Havok/hkbVariableBindingSetBinding.h"

namespace nemesis
{
    struct hkbVariableBindingSet : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x338ad4ff,
                                                "hkbVariableBindingSet",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbVariableBindingSet);

        nemesis::hkArray<nemesis::hkbVariableBindingSetBinding> m_bindings;
        int m_indexOfBindingToEnable;
        bool m_hasOutputBinding;

    public:
        hkbVariableBindingSet() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbVariableBindingSet::Class;
}
