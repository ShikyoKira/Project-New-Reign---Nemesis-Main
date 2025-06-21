#pragma once

#include "Havok/hkClassEnum.h"
#include "Havok/hkxEnum.h"
#include "Havok/hkxSparselyAnimatedInt.h"

namespace nemesis
{
    struct hkxSparselyAnimatedEnum : nemesis::hkxSparselyAnimatedInt
    {
        static constexpr nemesis::hkClass Class{0x68a47b64,
                                                "hkxSparselyAnimatedEnum",
                                                &nemesis::hkxSparselyAnimatedInt::Class,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxSparselyAnimatedEnum);

        nemesis::hkRefPtr<nemesis::hkClassEnum> m_type;
        nemesis::hkRefPtr<nemesis::hkxEnum> m_enum;

    public:
        hkxSparselyAnimatedEnum() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxSparselyAnimatedEnum::Class;
}
