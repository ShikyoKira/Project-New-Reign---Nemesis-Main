#pragma once

#include "Havok/Base/hkReferencedObject.h"
#include "Havok/Base/hkpMoppModifier.h"

namespace nemesis
{
    struct hkpRemoveTerminalsMoppModifier : nemesis::hkReferencedObject, nemesis::hkpMoppModifier
    {
        static constexpr nemesis::hkClass Class{0x91367f03,
                                                "hkpRemoveTerminalsMoppModifier",
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

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpRemoveTerminalsMoppModifier);

        nemesis::hkArray<unsigned int> m_removeInfo;
        nemesis::hkRefPtr<nemesis::HavokObject> m_tempShapesToRemove;

    public:
        hkpRemoveTerminalsMoppModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpRemoveTerminalsMoppModifier::Class;
}
