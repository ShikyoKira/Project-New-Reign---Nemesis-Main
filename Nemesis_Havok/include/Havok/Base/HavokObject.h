#pragma once

#include <functional>

#include "Enums/HavokVersion.h"

namespace nemesis
{
    struct Deserializer;
    struct Serializer;
    struct hkClass;
    struct hkPackfile;

    struct HavokObject
    {
    protected:
        unsigned int Signature;

    public:
        constexpr HavokObject(unsigned int signature) noexcept
            : Signature(signature)
        {
        }

        constexpr virtual ~HavokObject() = default;

        virtual const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const;
        virtual void SerializeTo(nemesis::Serializer& serializer) const
            = 0;
        virtual void DeserializeFrom(nemesis::Deserializer& deserializer) = 0;

        constexpr unsigned int GetSignature() const noexcept
        {
            return Signature;
        }

        static UMap<std::string, std::function<nemesis::HavokObject*()>>& GetCreationMap();
        static UMap<std::string, std::function<const nemesis::hkClass*(nemesis::HavokVersion)>>&
        GetClassMap();

    protected:
        template<size_t N>
        struct StringLiteral
        {
            char value[N];

            constexpr StringLiteral(const char(&str)[N])
            {
                std::copy_n(str, N, value);
            }
        };

        template <typename T, StringLiteral str, typename ST = nemesis::HavokObject>
        struct Registration
        {
            Registration(std::function<const nemesis::hkClass*(nemesis::HavokVersion)> get_class)
            {
                GetCreationMap().insert({str.value, []() { return static_cast<ST*>(new T()); }});
                GetClassMap().insert({str.value, get_class});
            }
        };
    };
}

#define REGISTER_HAVOK_POINTER_HEADER(type) static Registration<type, #type> Reg
#define REGISTER_HAVOK_POINTER_HEADER_2(type, type2) static Registration<type, #type, nemesis::type2> Reg

#define REGISTER_HAVOK_POINTER_SOURCE(type, get_class) \
    nemesis::HavokObject::Registration<nemesis::type, #type> nemesis::type::Reg( \
        get_class)
#define REGISTER_HAVOK_POINTER_SOURCE_2(type, type2, get_class) \
    nemesis::HavokObject::Registration<nemesis::type, #type, nemesis::type2> nemesis::type::Reg(get_class)
