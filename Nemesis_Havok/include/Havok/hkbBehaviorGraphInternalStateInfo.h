#pragma once

#include "Havok/hkbAuxiliaryNodeInfo.h"
#include "Havok/hkbBehaviorGraphInternalState.h"

namespace nemesis
{
    struct hkbBehaviorGraphInternalStateInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x645f898b,
                                                "hkbBehaviorGraphInternalStateInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBehaviorGraphInternalStateInfo);

        uint64_t m_characterId;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraphInternalState> m_internalState;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbAuxiliaryNodeInfo>> m_auxiliaryNodeInfo;
        nemesis::hkArray<short> m_activeEventIds;
        nemesis::hkArray<short> m_activeVariableIds;

    public:
        hkbBehaviorGraphInternalStateInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBehaviorGraphInternalStateInfo::Class;
}
