#pragma once

#include <mutex>
#include <span>

#include "Havok/Base/HavokObject.h"

namespace nemesis
{
    struct hkPackfile;
    struct hkClassEnum;
    struct hkClassMember;
    struct hkCustomAttributes;

    struct hkClass : nemesis::HavokObject
    {
    private:
        //static UMap<std::string, nemesis::hkClass*> ClassMap;
        //static std::mutex ClassMapMutex;

        REGISTER_HAVOK_POINTER_HEADER(hkClass);

    public:
        enum FlagValues
		{
			FLAGS_NONE = 0,
			ALIGN_8 = 128,
            ALIGN_16 = 256,
            NOT_OWNED = 512,
            SERIALIZE_IGNORED = 1024,
			ALIGN_32 = 2048,
            ALIGN_REAL = ALIGN_16
		};

        //static const UMap<std::string, nemesis::hkClass*>& GetClassMap();

    protected:
        const char* m_name = nullptr;
        const nemesis::hkClass* m_parent = nullptr;
        int m_objectSize{};
        int m_numImplementedInterfaces{};
        std::span<const nemesis::hkClassEnum*> m_declaredEnums;
        std::span<const nemesis::hkClassMember*> m_declaredMembers;
        void* m_defaults = nullptr;
        const nemesis::hkCustomAttributes* m_attributes = nullptr;
        FlagValues m_flags;
        int m_describedVersion{};

    public:
        constexpr hkClass();

        constexpr hkClass(unsigned int signature,
                          const char* name,
                          const nemesis::hkClass* parent,
                          int objectSize,
                          const nemesis::hkClass** implementedInterfaces,
                          int numImplementedInterfaces,
                          std::span<const nemesis::hkClassEnum*> declaredEnums,
                          std::span<const nemesis::hkClassMember*> declaredMembers,
                          void* defaults,
                          const nemesis::hkCustomAttributes* attributes,
                          FlagValues flags,
                          int describedVersion)
            : nemesis::HavokObject(signature)
            , m_name(name)
            , m_parent(parent)
            , m_objectSize(objectSize)
            , m_numImplementedInterfaces(numImplementedInterfaces)
            , m_declaredEnums(declaredEnums)
            , m_declaredMembers(declaredMembers)
            , m_defaults(defaults)
            , m_attributes(attributes)
            , m_flags(flags)
            , m_describedVersion(describedVersion)
        {
            //std::scoped_lock lock(ClassMapMutex);
            //auto itr = ClassMap.find(name);

            //if (itr != ClassMap.end()) return;

            //ClassMap.insert({name, this});
        }

        constexpr const char* GetName() const noexcept
        {
            return m_name;
        }

        constexpr unsigned int GetSignature() const noexcept
        {
            return Signature;
        }

        void SerializeFullTo(nemesis::Serializer& serializer) const;
        void DeserializeFullFrom(nemesis::Deserializer& deserializer);
        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        static const nemesis::hkClass* GetCurrentClass();
    };

    constexpr nemesis::hkClass Class{0x75585EF6,
                                     "hkClass",
                                     nullptr,
                                     80,
                                     nullptr,
                                     0,
                                     {},
                                     {},
                                     nullptr,
                                     nullptr,
                                     nemesis::hkClass::FlagValues::FLAGS_NONE,
                                     0};
}
