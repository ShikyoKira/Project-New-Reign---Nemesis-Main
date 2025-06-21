#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpConstraintAtom : virtual nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x59d67ef6,
                                                "hkpConstraintAtom",
                                                nullptr,
                                                2,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        using AtomType = nemesis::hkInternalClassEnumItem<unsigned short>;

        nemesis::hkEnum<AtomType, unsigned short> m_type = {
            {"TYPE_INVALID", 0},
            {"TYPE_BRIDGE", 1},
            {"TYPE_SET_LOCAL_TRANSFORMS", 2},
            {"TYPE_SET_LOCAL_TRANSLATIONS", 3},
            {"TYPE_SET_LOCAL_ROTATIONS", 4},
            {"TYPE_BALL_SOCKET", 5},
            {"TYPE_STIFF_SPRING", 6},
            {"TYPE_LIN", 7},
            {"TYPE_LIN_SOFT", 8},
            {"TYPE_LIN_LIMIT", 9},
            {"TYPE_LIN_FRICTION", 10},
            {"TYPE_LIN_MOTOR", 11},
            {"TYPE_2D_ANG", 12},
            {"TYPE_ANG", 13},
            {"TYPE_ANG_LIMIT", 14},
            {"TYPE_TWIST_LIMIT", 15},
            {"TYPE_CONE_LIMIT", 16},
            {"TYPE_ANG_FRICTION", 17},
            {"TYPE_ANG_MOTOR", 18},
            {"TYPE_RAGDOLL_MOTOR", 19},
            {"TYPE_PULLEY", 20},
            {"TYPE_RACK_AND_PINION", 21},
            {"TYPE_COG_WHEEL", 22},
            {"TYPE_SETUP_STABILIZATION", 23},
            {"TYPE_3D_ANG", 24},
            {"TYPE_DEFORMABLE_3D_LIN", 25},
            {"TYPE_DEFORMABLE_3D_ANG", 26},
            {"TYPE_OVERWRITE_PIVOT", 27},
            {"TYPE_WHEEL_FRICTION", 28},
            {"TYPE_CONTACT", 29},
            {"FIRST_MODIFIER_TYPE", 30},
            {"TYPE_MODIFIER_SOFT_CONTACT", 30},
            {"TYPE_MODIFIER_MASS_CHANGER", 31},
            {"TYPE_MODIFIER_VISCOUS_SURFACE", 32},
            {"TYPE_MODIFIER_MOVING_SURFACE", 33},
            {"TYPE_MODIFIER_IGNORE_CONSTRAINT", 34},
            {"TYPE_MODIFIER_CENTER_OF_MASS_CHANGER", 35},
            {"LAST_MODIFIER_TYPE", 35},
            {"TYPE_MAX", 36},
        };

    public:
        hkpConstraintAtom(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintAtom::Class;
}
