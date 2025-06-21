#include "Havok/hkClass.h"
#include "Havok/hkClassEnum.h"
#include "Havok/hkClassMember.h"

#include "Serialize/Serializer.h"
#include "Serialize/Deserializer.h"

//UMap<std::string, nemesis::hkClass*> nemesis::hkClass::ClassMap;
//std::mutex nemesis::hkClass::ClassMapMutex;
//
//nemesis::hkClass nemesis::hkClass::Class(
//    0x75585EF6, "hkClass", nullptr, 80, nullptr, 0, {}, {}, nullptr, FlagValues::FLAGS_NONE, 0);
//
//const UMap<std::string, nemesis::hkClass*>& nemesis::hkClass::GetClassMap()
//{
//    return ClassMap;
//}
//
//constexpr nemesis::hkClass::hkClass(unsigned int signature,
//                          const std::string& name,
//                          const nemesis::hkClass* parent,
//                          int objectSize,
//                          const nemesis::hkClass** implementedInterfaces,
//                          int numImplementedInterfaces,
//                          std::initializer_list<nemesis::hkClassEnum> declaredEnums,
//                          std::initializer_list<nemesis::hkClassMember> declaredMembers,
//                          void* defaults,
//                          FlagValues flags,
//                          int describedVersion)
//    : nemesis::HavokObject(signature)
//    , m_name(name)
//    , m_parent(parent)
//    , m_objectSize(objectSize)
//    , m_numImplementedInterfaces(numImplementedInterfaces)
//    , m_declaredEnums(declaredEnums)
//    , m_declaredMembers(declaredMembers)
//    , m_defaults(defaults)
//    , m_flags(flags)
//    , m_describedVersion(describedVersion)
//{
//    std::scoped_lock lock(ClassMapMutex);
//    auto itr = ClassMap.find(name);
//
//    if (itr != ClassMap.end()) return;
//
//    ClassMap.insert({name, this});
//}
//
//const std::string& nemesis::hkClass::GetName() const noexcept
//{
//    return m_name;
//}
//
//unsigned int nemesis::hkClass::GetSignature() const noexcept
//{
//    return Signature;
//}

REGISTER_HAVOK_POINTER_SOURCE(hkClass,
                              [](nemesis::HavokVersion version)
                              { return nemesis::hkClass::GetCurrentClass(); });

constexpr nemesis::hkClass::hkClass()
    : nemesis::HavokObject(0x00)
{
    throw std::runtime_error("hkClass cannot be initialized");
}

void nemesis::hkClass::SerializeFullTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", nemesis::hkCString(m_name));

    if (m_parent)
    {
        m_parent->SerializeFullTo(serializer);
    }

    serializer.WriteValue("objectSize", m_objectSize);
    serializer.WriteValue("numImplementedInterfaces", m_numImplementedInterfaces);

    for (auto& member : m_declaredMembers)
    {
        member->SerializeTo(serializer);
    }

    for (auto& decl_enum : m_declaredEnums)
    {
        decl_enum->SerializeTo(serializer);
    }

    serializer.WriteObject("attributes", nemesis::hkRefPtr<nemesis::HavokObject>() /*m_attributes*/);
    serializer.WriteValue("flags", static_cast<unsigned int>(m_flags));
    serializer.WriteValue("describedVersion", m_describedVersion);
}

void nemesis::hkClass::DeserializeFullFrom(nemesis::Deserializer& deserializer)
{
    throw std::runtime_error("DeserializeFullFrom hkClass is not supported");
}

void nemesis::hkClass::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", Signature);
    serializer.WriteValue("", 0x09);
    serializer.WriteValue("", nemesis::hkCString(m_name));
}

void nemesis::hkClass::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    throw std::runtime_error("DeserializeFrom hkClass is not supported");
}

const nemesis::hkClass* nemesis::hkClass::GetCurrentClass()
{
    return &Class;
}
