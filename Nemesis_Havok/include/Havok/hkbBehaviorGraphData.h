#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbBehaviorGraphStringData.h"
#include "Havok/hkbEventInfo.h"
#include "Havok/hkbVariableBounds.h"
#include "Havok/hkbVariableInfo.h"
#include "Havok/hkbVariableValue.h"
#include "Havok/hkbVariableValueSet.h"

namespace nemesis
{
    struct hkbBehaviorGraphData : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x95aca5d,
                                                "hkbBehaviorGraphData",
                                                &nemesis::hkReferencedObject::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBehaviorGraphData);

        nemesis::hkArray<float> m_attributeDefaults;
        nemesis::hkArray<nemesis::hkbVariableInfo> m_variableInfos;
        nemesis::hkArray<nemesis::hkbVariableInfo> m_characterPropertyInfos;
        nemesis::hkArray<nemesis::hkbEventInfo> m_eventInfos;
        nemesis::hkArray<nemesis::hkbVariableBounds> m_variableBounds;
        nemesis::hkArray<nemesis::hkbVariableValue> m_wordMinVariableValues;
        nemesis::hkArray<nemesis::hkbVariableValue> m_wordMaxVariableValues;
        nemesis::hkRefPtr<nemesis::hkbVariableValueSet> m_variableInitialValues;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraphStringData> m_stringData;

    public:
        hkbBehaviorGraphData() noexcept;

        const nemesis::hkbBehaviorGraphStringData* GetStringData() const;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBehaviorGraphData::Class;
}
