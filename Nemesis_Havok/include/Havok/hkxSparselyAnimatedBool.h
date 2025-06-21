#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxSparselyAnimatedBool : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x7a894596,
                                                "hkxSparselyAnimatedBool",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxSparselyAnimatedBool);

        nemesis::hkArray<bool> m_bools;
        nemesis::hkArray<float> m_times;

    public:
        hkxSparselyAnimatedBool() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxSparselyAnimatedBool::Class;
}
