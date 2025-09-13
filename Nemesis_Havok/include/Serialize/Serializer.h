#pragma once

#include <filesystem>
#include <functional>
#include <ostream>
#include <type_traits>

#include "Havok/Types/hkAlign.h"
#include "Havok/Types/hkAttributeId.h"
#include "Havok/Types/hkCharacterPropertyId.h"
#include "Havok/Types/hkCString.h"
#include "Havok/Types/hkEnum.h"
#include "Havok/Types/hkEventId.h"
#include "Havok/Types/hkHalf.h"
#include "Havok/Types/hkLong.h"
#include "Havok/Types/hkMatrix4.h"
#include "Havok/Types/hkQsTransform.h"
#include "Havok/Types/hkStringPtr.h"
#include "Havok/Types/hkTransform.h"
#include "Havok/Types/hkUFloat8.h"
#include "Havok/Types/hkUlong.h"
#include "Havok/Types/hkVariableId.h"
#include "Havok/Types/hkVector8.h"

#include "Enums/HavokVersion.h"

namespace nemesis
{
    template <typename T>
    struct hkSmallArray;
    
    template <typename T>
    struct hkArray;

    template <typename EnumType, typename StorageType>
    struct hkEnum;

    struct HavokObject;
    struct hkSmallArrayBase;
    struct hkArrayBase;
    struct hkClass;
    struct hkCString;
    struct hkPackfile;
    struct hkPackfileSection;
    struct hkRefVariant;
    struct hkRootLevelContainer;

    struct Serializer
    {
    protected:
        std::ostream* Stream = nullptr;

        unsigned char PointerSize;
        unsigned char Endian;
        unsigned char PaddingOption;
        std::string ContentsVersionString;
        nemesis::HavokVersion ContentsVersion;

        static std::string StringToHex(const std::string& str);

        template <typename T, std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>, int> = 0>
        static std::string NumberToHex(T number, bool is_uppercase = false)
        {
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(sizeof(T));

            if (is_uppercase)
            {
                ss << std::uppercase;
            }

            ss << number;
            return "0x" + ss.str();
        }

    public:
        virtual void WriteFileHeader() = 0;

        virtual void Serialize(const nemesis::hkPackfile& packfile) = 0;
        virtual void Serialize(const nemesis::hkPackfileSection& section_data) = 0;
        virtual void Serialize(const nemesis::hkRootLevelContainer& root) = 0;

        virtual void Save(const std::filesystem::path& filepath) = 0;
        virtual std::string RawData() const                      = 0;

        unsigned char GetPointerSize() const noexcept;
        unsigned char GetEndian() const noexcept;
        unsigned char GetPaddingOption() const noexcept;
        const std::string& GetContentsVersionString() const noexcept;
        nemesis::HavokVersion GetContentsVersion() const noexcept;

        virtual int GetCurrentPosition() const;
        virtual void Pad(unsigned int size);
        virtual void Pad(unsigned int size, unsigned char pad_ch);
        virtual void PadNode();
        virtual void TryPadToPointerSize();

        virtual void Skip(unsigned int step);

        virtual void WriteValue(const std::string& name, bool val)               = 0;
        virtual void WriteValue(const std::string& name, char val)               = 0;
        virtual void WriteValue(const std::string& name, unsigned char val)      = 0;
        virtual void WriteValue(const std::string& name, short val)              = 0;
        virtual void WriteValue(const std::string& name, unsigned short val)     = 0;
        virtual void WriteValue(const std::string& name, int val)                = 0;
        virtual void WriteValue(const std::string& name, unsigned int val)       = 0;
        virtual void WriteValue(const std::string& name, long val)               = 0;
        virtual void WriteValue(const std::string& name, unsigned long val)      = 0;
        virtual void WriteValue(const std::string& name, long long val)          = 0;
        virtual void WriteValue(const std::string& name, unsigned long long val) = 0;
        virtual void WriteValue(const std::string& name, Float16 val)            = 0;
        virtual void WriteValue(const std::string& name, float val)              = 0;
        virtual void WriteValue(const std::string& name, double val)             = 0;

        virtual void WriteValue(const std::string& name, const std::string& val, bool terminate = false) = 0;
        virtual void
        WriteValue(const std::string& name, const std::string& val, std::size_t size, char padding = '\0')
            = 0;

        virtual void WriteValue(const std::string& name, const nemesis::hkEventId& event_id)            = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkAttributeId& attr_id)         = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkVariableId& var_id)           = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkCharacterPropertyId& prop_id) = 0;

        virtual void WriteValue(const std::string& name, const nemesis::hkCString& cstring) = 0;
        virtual void
        WriteValue(const std::string& name, const nemesis::hkCString& cstring, unsigned int padding_size);
        virtual void WriteValue(const std::string& name, const nemesis::hkStringPtr& string_ptr) = 0;
        virtual void WriteValue(const std::string& name,
                                const nemesis::hkStringPtr& string_ptr,
                                unsigned int padding_size);
        virtual void WriteValue(const std::string& name, const nemesis::hkEnumBase& val)             = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkVector4& vec4)             = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkVector8& vec8)             = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkQuaternion& quaternion)    = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkQsTransform& qs_transform) = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkUFloat8& ufloat8)          = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkLong& _long)               = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkUlong& ulong)              = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkHalf& half)                = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkTransform& transform)      = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkMatrix3& matrix3)          = 0;
        virtual void WriteValue(const std::string& name, const nemesis::hkMatrix4& matrix4)          = 0;

        virtual void WriteObject(const std::string& name, const nemesis::hkSmallArrayBase& array)         = 0;
        virtual void WriteObject(const std::string& name, const nemesis::hkArrayBase& array)              = 0;
        virtual void WriteObject(const std::string& name, const nemesis::hkRefVariant& ref_obj)           = 0;
        virtual void WriteObject(const std::string& name, const nemesis::HavokObject& havok_obj)          = 0;
        virtual void WriteObject(const std::string& name, const nemesis::HavokObject* const* ptr_obj_ptr) = 0;
        virtual void WriteObject(const std::string& name, const void* obj_ptr);

        virtual void WriteNullPointer();

        virtual void WriteSerializeIgnoredValue(const std::string& name, bool val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, char val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, unsigned char val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, short val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, unsigned short val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, int val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, unsigned int val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, long val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, unsigned long val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, long long val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, unsigned long long val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, Float16 val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, float val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, double val);

        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkEventId& event_id);
        virtual void WriteSerializeIgnoredValue(const std::string& name,
                                                const nemesis::hkAttributeId& attr_id);
        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkVariableId& var_id);
        virtual void WriteSerializeIgnoredValue(const std::string& name,
                                                const nemesis::hkCharacterPropertyId& prop_id);

        virtual void WriteSerializeIgnoredValue(const std::string& name,
                                               const nemesis::hkCString& cstring);
        virtual void WriteSerializeIgnoredValue(const std::string& name,
                                               const nemesis::hkStringPtr& string_ptr);
        virtual void WriteSerializeIgnoredValue(const std::string& name,
                                                const nemesis::hkStringPtr& string_ptr,
                                                unsigned int padding_size);
        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkEnumBase& val);
        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkVector4& vec4);
        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkVector8& vec8);
        virtual void WriteSerializeIgnoredValue(const std::string& name,
                                                const nemesis::hkQuaternion& quaternion);
        virtual void WriteSerializeIgnoredValue(const std::string& name,
                                                const nemesis::hkQsTransform& qs_transform);
        virtual void WriteSerializeIgnoredValue(const std::string& name,
                                                const nemesis::hkTransform& transform);
        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkUFloat8& ufloat8);
        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkLong& _long);
        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkUlong& ulong);
        virtual void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkHalf& half);

        virtual void WriteSerializeIgnoredObject(const std::string& name,
                                                const nemesis::hkSmallArrayBase& array);
        virtual void WriteSerializeIgnoredObject(const std::string& name,
                                                const nemesis::hkArrayBase& array);
        virtual void WriteSerializeIgnoredObject(const std::string& name,
                                                const nemesis::hkRefVariant& ref_obj);
        virtual void WriteSerializeIgnoredObject(const std::string& name,
                                                const nemesis::HavokObject& havok_obj);
        virtual void WriteSerializeIgnoredObject(const std::string& name,
                                                 const nemesis::HavokObject* const* ptr_obj_ptr);
        virtual void WriteSerializeIgnoredObject(const std::string& name, void* obj_ptr);

        virtual void WriteUSize(uint64_t val);

    protected:
        virtual void
        WriteArrayValue(const std::string& name, const void* (&list)[], size_t size, size_t type_size)
            = 0;
        virtual void WriteArrayValue(const std::string& name, const nemesis::hkCString* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkStringPtr* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkEnumBase* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkVector4* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkVector8* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkQuaternion* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkQsTransform* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkUFloat8* (&list)[], size_t size)
            = 0;
        virtual void WriteArrayValue(const std::string& name, const nemesis::hkLong* (&list)[], size_t size)
            = 0;
        virtual void WriteArrayValue(const std::string& name, const nemesis::hkUlong* (&list)[], size_t size)
            = 0;
        virtual void WriteArrayValue(const std::string& name, const nemesis::hkHalf* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkTransform* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkMatrix3* (&list)[], size_t size)
            = 0;
        virtual void
        WriteArrayValue(const std::string& name, const nemesis::hkMatrix4* (&list)[], size_t size)
            = 0;

        virtual void
        WriteArrayObject(const std::string& name, const nemesis::HavokObject* (&list)[], size_t size)
            = 0;

        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const void* (&list)[],
                                                     size_t size,
                                                     size_t type_size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkCString* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkStringPtr* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkEnumBase* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkVector4* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkVector8* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkQuaternion* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkQsTransform* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkUFloat8* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkLong* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkUlong* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkHalf* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkTransform* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkMatrix3* (&list)[],
                                                     size_t size);
        virtual void WriteSerializeIgnoredArrayValue(const std::string& name,
                                                     const nemesis::hkMatrix4* (&list)[],
                                                     size_t size);

        virtual void WriteSerializeIgnoreArrayObject(const std::string& name,
                                                     const nemesis::HavokObject* (&list)[],
                                                     size_t size);

    public:
        template <typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
        inline void WriteValue(const std::string& name, const T& enum_val)
        {
            using UnderlyingType = typename std::underlying_type_t<T>;
            WriteValue(name, static_cast<UnderlyingType>(enum_val));
        }

        template <typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
        inline void WriteSerializeIgnoredValue(const std::string& name, const T& enum_val)
        {
            using UnderlyingType = typename std::underlying_type_t<T>;
            WriteSerializeIgnoredValue(name, static_cast<UnderlyingType>(enum_val));
        }

        template <typename T, std::enable_if_t<is_complete_v<T>, int> = 0>
        inline void WriteObject(const std::string& name, const T* const& obj_ptr)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>, "Only havok objects are allowed");

            const nemesis::HavokObject* hkx_ptr = static_cast<const nemesis::HavokObject*>(obj_ptr);
            WriteObject(name, &hkx_ptr);
        }

        template <typename T, std::enable_if_t<is_complete_v<T>, int> = 0>
        inline void WriteSerializeIgnoredObject(const std::string& name, const T* const& obj_ptr)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>, "Only havok objects are allowed");

            const nemesis::HavokObject* hkx_ptr = static_cast<const nemesis::HavokObject*>(obj_ptr);
            WriteSerializeIgnoredObject(name, &hkx_ptr);
        }

        template <typename T, size_t N>
        void WriteValue(const std::string& name, const std::array<T, N>& array)
        {
            static_assert(std::is_arithmetic_v<T> || std::is_same_v<Float16, T>
                              || std::is_base_of_v<nemesis::hkVariant, T>,
                          "Only primitive numeric types are allowed");

            if constexpr (std::is_base_of_v<nemesis::hkVariant, T>)
            {
                const T* p_array[N];

                for (size_t i = 0; i < N; ++i)
                {
                    p_array[i] = &array[i];
                }

                WriteArrayValue(name, p_array, N);
            }
            else
            {
                const void* c_array[N];

                for (size_t i = 0; i < N; ++i)
                {
                    c_array[i] = &array[i];
                }

                WriteArrayValue(name, c_array, N, sizeof(T));
            }
        }

        template <typename T, size_t N>
        void WriteSerializeIgnoredValue(const std::string& name, const std::array<T, N>& array)
        {
            static_assert(std::is_arithmetic_v<T> || std::is_same_v<Float16, T>
                              || std::is_base_of_v<nemesis::hkVariant, T>,
                          "Only primitive types are allowed");

            if constexpr (std::is_base_of_v<nemesis::hkVariant, T>)
            {
                const T* p_array[N];

                for (size_t i = 0; i < N; ++i)
                {
                    p_array[i] = &array[i];
                }

                WriteSerializeIgnoredArrayValue(name, p_array, N);
            }
            else
            {
                const void* c_array[N];

                for (size_t i = 0; i < N; ++i)
                {
                    c_array[i] = &array[i];
                }

                WriteSerializeIgnoredArrayValue(name, c_array, N, sizeof(T));
            }
        }

        template <typename T,
                  size_t N,
                  std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<Float16, T>
                                       || std::is_base_of_v<nemesis::hkVariant, T>,
                                   int>
                  = 0>
        void WriteValue(const std::string& name, const nemesis::hkAlign<N, T>& aligned_obj)
        {
            Pad(N);
            WriteValue(name, aligned_obj.Data);
        }

        template <typename T,
                  size_t N,
                  std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<Float16, T>
                                       || std::is_base_of_v<nemesis::hkVariant, T>,
                                   int>
                  = 0>
        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkAlign<N, T>& aligned_obj)
        {
            Pad(N);
            WriteSerializeIgnoredValue(name, aligned_obj.Data);
        }

        template <typename T, size_t N>
        void WriteObject(const std::string& name, const std::array<T*, N>& array)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>, "Only havok objects are allowed");

            const nemesis::HavokObject* p_array[N];

            for (size_t i = 0; i < N; ++i)
            {
                p_array[i] = array[i];
            }

            WriteArrayObject(name, p_array, N);
        }

        template <typename T, size_t N>
        void WriteObject(const std::string& name, const std::array<T, N>& array)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>, "Only havok objects are allowed");

            const nemesis::HavokObject* p_array[N];

            for (size_t i = 0; i < N; ++i)
            {
                p_array[i] = &array[i];
            }

            WriteArrayObject(name, p_array, N);
        }

        template <typename T, size_t N>
        void WriteSerializeIgnoredObject(const std::string& name, const std::array<T, N>& array)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>, "Only havok objects are allowed");

            const nemesis::HavokObject* p_array[N];

            for (size_t i = 0; i < N; ++i)
            {
                p_array[i] = &array[i];
            }

            WriteSerializeIgnoreArrayObject(name, p_array, N);
        }

        template <typename T,
                  size_t N,
                  std::enable_if_t<std::is_base_of_v<nemesis::HavokObject, T>,
                                   int>
                  = 0>
        void WriteObject(const std::string& name, const nemesis::hkAlign<N, T>& aligned_obj)
        {
            Pad(N);
            WriteObject(name, aligned_obj.Data);
        }

        template <typename T, size_t N, std::enable_if_t<std::is_base_of_v<nemesis::HavokObject, T>, int> = 0>
        void WriteSerializeIgnoredObject(const std::string& name, const nemesis::hkAlign<N, T>& aligned_obj)
        {
            Pad(N);
            WriteSerializeIgnoredObject(name, aligned_obj.Data);
        }

        void WritePointerSize(unsigned char pointer_size);
        void WriteEndian(unsigned char endian);
        void WritePaddingOption(unsigned char padding_option);
        void WriteContentsVersionString(const std::string& version_string);
    };
}
