#include "Havok/hkbVariableBindingSetBinding.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbVariableBindingSetBinding,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbVariableBindingSetBinding::Class; });

nemesis::hkbVariableBindingSetBinding::hkbVariableBindingSetBinding() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbVariableBindingSetBinding::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbVariableBindingSetBinding::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("memberPath", m_memberPath);                    // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("memberClass", m_memberClass); // offset: 4/8 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("offsetInObjectPlusOne",
                                          m_offsetInObjectPlusOne); // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("offsetInArrayPlusOne",
                                          m_offsetInArrayPlusOne); // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("rootVariableIndex",
                                          m_rootVariableIndex);        // offset: 16/24 size: 4/4 align: 4/4
    serializer.WriteValue("variableIndex", m_variableIndex);           // offset: 20/28 size: 4/4 align: 4/4
    serializer.WriteValue("bitIndex", m_bitIndex);                     // offset: 24/32 size: 1/1 align: 1/1
    serializer.WriteValue("bindingType", m_bindingType);               // offset: 25/33 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("memberType", m_memberType); // offset: 26/34 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("variableType",
                                          m_variableType);   // offset: 27/35 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("flags", m_flags); // offset: 28/36 size: 1/1 align: 1/1
    serializer.Skip(3);                                      // offset: 29/37 size: 3/3
    // class size: 32/40 align: 4/8
}

void nemesis::hkbVariableBindingSetBinding::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("memberPath", m_memberPath);    // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("memberClass", m_memberClass); // offset: 4/8 size: 4/8 align: 4/8
    deserializer.ReadValue("offsetInObjectPlusOne",
                           m_offsetInObjectPlusOne); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("offsetInArrayPlusOne",
                           m_offsetInArrayPlusOne);                   // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadValue("rootVariableIndex", m_rootVariableIndex); // offset: 16/24 size: 4/4 align: 4/4
    deserializer.ReadValue("variableIndex", m_variableIndex);         // offset: 20/28 size: 4/4 align: 4/4
    deserializer.ReadValue("bitIndex", m_bitIndex);                   // offset: 24/32 size: 1/1 align: 1/1
    deserializer.ReadValue("bindingType", m_bindingType);             // offset: 25/33 size: 1/1 align: 1/1
    deserializer.ReadValue("memberType", m_memberType);               // offset: 26/34 size: 1/1 align: 1/1
    deserializer.ReadValue("variableType", m_variableType);           // offset: 27/35 size: 1/1 align: 1/1
    deserializer.ReadValue("flags", m_flags);                         // offset: 28/36 size: 1/1 align: 1/1
    deserializer.Skip(3);                                             // offset: 29/37 size: 3/3
    // class size: 32/40 align: 4/8
}
