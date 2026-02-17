#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxSplineControlPoint.h"

namespace nemesis
{
    struct hkxSpline : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x5ad44352,
                                                "hkxSpline",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxSpline);

        nemesis::hkArray<nemesis::hkxSplineControlPoint> m_controlPoints;
        bool m_isClosed{};

    public:
        hkxSpline() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxSpline::Class;
}
