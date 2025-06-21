#pragma once

#include "Havok/hkArray.h"
#include "Havok/hkRefPtr.h"

template <typename T>
inline nemesis::hkSmallArray<T>& nemesis::Deserializer::ReadObject(const std::string& name,
                                                                   nemesis::hkSmallArray<T>& array)
{
    return static_cast<nemesis::hkSmallArray<T>&>(ReadArrayObject(name, array));
}

template <typename T>
inline nemesis::hkArray<T>& nemesis::Deserializer::ReadObject(const std::string& name,
                                                              nemesis::hkArray<T>& array)
{
    return static_cast<nemesis::hkArray<T>&>(ReadArrayObject(name, array));
}

template <typename T, bool Fixed>
inline nemesis::hkRefPtr<T, Fixed>& nemesis::Deserializer::ReadObject(const std::string& name,
                                                                         nemesis::hkRefPtr<T, Fixed>& ref_obj)
{
    return static_cast<nemesis::hkRefPtr<T, Fixed>&>(ReadRefObject(name, ref_obj));
}
