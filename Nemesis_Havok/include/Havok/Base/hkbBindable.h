#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbVariableBindingSet.h"

namespace nemesis
{
    struct hkbBindable : nemesis::hkReferencedObject
    {
    protected:
        static constexpr nemesis::hkClass Class{0x2c1432d7,
                                                "hkbBindable",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

        nemesis::hkRefPtr<nemesis::hkbVariableBindingSet> m_variableBindingSet;
        nemesis::hkArray<nemesis::HavokObject> m_cachedBindables;
        bool m_areBindablesCached{};

    public:
        hkbBindable(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBindable::Class;
}
