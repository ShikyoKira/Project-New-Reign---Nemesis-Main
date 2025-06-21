#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpConvexPieceStreamData : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa5bd1d6e,
                                                "hkpConvexPieceStreamData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpConvexPieceStreamData);

        nemesis::hkArray<unsigned int> m_convexPieceStream;
        nemesis::hkArray<unsigned int> m_convexPieceOffsets;
        nemesis::hkArray<unsigned int> m_convexPieceSingleTriangles;

    public:
        hkpConvexPieceStreamData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexPieceStreamData::Class;
}
