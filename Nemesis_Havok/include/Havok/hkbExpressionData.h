#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbExpressionData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x6740042a,
                                                "hkbExpressionData",
                                                nullptr,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbExpressionData);

        using ExpressionEventMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkStringPtr m_expression;
        int m_assignmentVariableIndex{};
        int m_assignmentEventIndex{};
        nemesis::hkEnum<ExpressionEventMode, char> m_eventMode = {
            {"EVENT_MODE_SEND_ONCE", 0},
            {"EVENT_MODE_SEND_ON_TRUE", 1},
            {"EVENT_MODE_SEND_ON_FALSE_TO_TRUE", 2},
            {"EVENT_MODE_SEND_EVERY_FRAME_ONCE_TRUE", 3},
        };
        bool m_raisedEvent{};
        bool m_wasTrueInPreviousFrame{};

    public:
        hkbExpressionData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbExpressionData::Class;
}
