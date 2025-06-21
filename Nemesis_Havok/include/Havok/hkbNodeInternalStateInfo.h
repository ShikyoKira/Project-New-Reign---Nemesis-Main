#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbGeneratorSyncInfo.h"

namespace nemesis
{
    struct hkbNodeInternalStateInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x7db9971d,
                                                "hkbNodeInternalStateInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                120,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbNodeInternalStateInfo);

        nemesis::hkbGeneratorSyncInfo m_syncInfo;
        nemesis::hkStringPtr m_name;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_internalState;
        short m_nodeId;
        bool m_hasActivateBeenCalled;

    public:
        hkbNodeInternalStateInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbNodeInternalStateInfo::Class;
}
