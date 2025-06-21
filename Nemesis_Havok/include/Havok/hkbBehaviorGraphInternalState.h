#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbNodeInternalStateInfo.h"
#include "Havok/hkbVariableValueSet.h"

namespace nemesis
{
    struct hkbBehaviorGraphInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x8699b6eb,
                                                "hkbBehaviorGraphInternalState",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbBehaviorGraphInternalState);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbNodeInternalStateInfo>> m_nodeInternalStateInfos;
        nemesis::hkRefPtr<nemesis::hkbVariableValueSet> m_variableValueSet;

    public:
        hkbBehaviorGraphInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBehaviorGraphInternalState::Class;
}
