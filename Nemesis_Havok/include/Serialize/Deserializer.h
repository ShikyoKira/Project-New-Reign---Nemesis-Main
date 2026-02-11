#pragma once

#include <filesystem>
#include <functional>
#include <fstream>
#include <cassert>
#include <iomanip>

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

    template <typename T, bool Fixed = false, bool Proxy = false>
    struct hkRefPtr;

    struct HavokObject;
    struct hkSmallArrayBase;
    struct hkArrayBase;
    struct hkRefVariant;
    struct hkClass;
    struct hkPackfile;
    struct hkPackfileHeader;
    struct hkPackfileSection;
    struct hkPackfileSectionHeader;
    struct hkRootLevelContainer;

    struct Deserializer
    {
    protected:
        UPtr<std::istream> Stream;

        unsigned char PointerSize;
        unsigned char Endian = 1;
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

        virtual bool CanAssert() const = 0;

    public:
        virtual nemesis::hkPackfile Deserialize()                          = 0;
        virtual void Deserialize(nemesis::hkPackfileSection& section_data) = 0;
        virtual void Deserialize(nemesis::hkRootLevelContainer& root)      = 0;

        void Close() noexcept;

        virtual std::streampos GetCurrentPosition() const;
        virtual void GoToPosition(std::streampos address);

        virtual void Pad(unsigned int size);
        virtual void PadNode();
        virtual void Skip(unsigned int step);

        virtual bool& ReadValue(const std::string& name, bool& val)                             = 0;
        virtual char& ReadValue(const std::string& name, char& ch)                              = 0;
        virtual unsigned char& ReadValue(const std::string& name, unsigned char& ch)            = 0;
        virtual short& ReadValue(const std::string& name, short& byte)                          = 0;
        virtual unsigned short& ReadValue(const std::string& name, unsigned short& byte)        = 0;
        virtual int& ReadValue(const std::string& name, int& val)                               = 0;
        virtual unsigned int& ReadValue(const std::string& name, unsigned int& val)             = 0;
        virtual long& ReadValue(const std::string& name, long& val)                             = 0;
        virtual unsigned long& ReadValue(const std::string& name, unsigned long& val)           = 0;
        virtual long long& ReadValue(const std::string& name, long long& val)                   = 0;
        virtual unsigned long long& ReadValue(const std::string& name, unsigned long long& val) = 0;
        virtual Float16& ReadValue(const std::string& name, Float16& val)                       = 0;
        virtual float& ReadValue(const std::string& name, float& val)                           = 0;
        virtual double& ReadValue(const std::string& name, double& val)                         = 0;

        virtual std::string& ReadValue(const std::string& name, std::string& ref_str) = 0;
        virtual std::string&
        ReadValue(const std::string& name, std::string& ref_str, unsigned int size, char pad = 0x00)
            = 0;

        virtual nemesis::hkEventId& ReadValue(const std::string& name, nemesis::hkEventId& event_id) = 0;
        virtual nemesis::hkAttributeId& ReadValue(const std::string& name, nemesis::hkAttributeId& attr_id)
            = 0;
        virtual nemesis::hkVariableId& ReadValue(const std::string& name, nemesis::hkVariableId& var_id) = 0;
        virtual nemesis::hkCharacterPropertyId& ReadValue(const std::string& name,
                                                          nemesis::hkCharacterPropertyId& prop_id)
            = 0;

        virtual nemesis::hkCString& ReadValue(const std::string& name, nemesis::hkCString& cstring) = 0;
        virtual nemesis::hkCString&
        ReadValue(const std::string& name, nemesis::hkCString& cstring, unsigned int padding_size);
        virtual nemesis::hkStringPtr& ReadValue(const std::string& name, nemesis::hkStringPtr& string_ptr)
            = 0;
        virtual nemesis::hkStringPtr&
        ReadValue(const std::string& name, nemesis::hkStringPtr& string_ptr, unsigned int padding_size);
        virtual nemesis::hkEnumBase& ReadValue(const std::string& name, nemesis::hkEnumBase& val) = 0;
        virtual nemesis::hkVector4& ReadValue(const std::string& name, nemesis::hkVector4& vec4)  = 0;
        virtual nemesis::hkVector8& ReadValue(const std::string& name, nemesis::hkVector8& vec8)  = 0;
        virtual nemesis::hkQuaternion& ReadValue(const std::string& name, nemesis::hkQuaternion& quaternion)
            = 0;
        virtual nemesis::hkQsTransform& ReadValue(const std::string& name,
                                                  nemesis::hkQsTransform& qs_transform)
            = 0;
        virtual nemesis::hkUFloat8& ReadValue(const std::string& name, nemesis::hkUFloat8& ufloat8)       = 0;
        virtual nemesis::hkLong& ReadValue(const std::string& name, nemesis::hkLong& _long)               = 0;
        virtual nemesis::hkUlong& ReadValue(const std::string& name, nemesis::hkUlong& ulong)             = 0;
        virtual nemesis::hkHalf& ReadValue(const std::string& name, nemesis::hkHalf& half)                = 0;
        virtual nemesis::hkTransform& ReadValue(const std::string& name, nemesis::hkTransform& transform) = 0;
        virtual nemesis::hkMatrix3& ReadValue(const std::string& name, nemesis::hkMatrix3& matrix3)       = 0;
        virtual nemesis::hkMatrix4& ReadValue(const std::string& name, nemesis::hkMatrix4& matrix4)       = 0;

        template <typename T,
                  std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<nemesis::hkCString, T>
                                       || std::is_same_v<nemesis::hkStringPtr, T>,
                                   int>
                  = 0>
        inline T Read(const std::string& name)
        {
            T rtn_val;
            ReadValue(name, rtn_val);
            return rtn_val;
        }

        template <typename T,
                  std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<nemesis::hkCString, T>
                                       || std::is_same_v<nemesis::hkStringPtr, T>,
                                   int>
                  = 0>
        inline T Read()
        {
            T rtn_val;
            ReadValue("", rtn_val);
            return rtn_val;
        }

    protected:
        virtual void** ReadArrayValue(const std::string& name, void* list[], size_t size, size_t type_size)
            = 0;
        virtual bool** ReadArrayValue(const std::string& name, bool* (&list)[], size_t size) = 0;
        virtual char** ReadArrayValue(const std::string& name, char* (&list)[], size_t size) = 0;
        virtual unsigned char** ReadArrayValue(const std::string& name, unsigned char* (&list)[], size_t size)
            = 0;
        virtual short** ReadArrayValue(const std::string& name, short* (&list)[], size_t size) = 0;
        virtual unsigned short**
        ReadArrayValue(const std::string& name, unsigned short* (&list)[], size_t size)
            = 0;
        virtual int** ReadArrayValue(const std::string& name, int* (&list)[], size_t size) = 0;
        virtual unsigned int** ReadArrayValue(const std::string& name, unsigned int* (&list)[], size_t size)
            = 0;
        virtual long** ReadArrayValue(const std::string& name, long* (&list)[], size_t size) = 0;
        virtual unsigned long** ReadArrayValue(const std::string& name, unsigned long* (&list)[], size_t size)
            = 0;
        virtual long long** ReadArrayValue(const std::string& name, long long* (&list)[], size_t size) = 0;
        virtual unsigned long long**
        ReadArrayValue(const std::string& name, unsigned long long* (&list)[], size_t size)
            = 0;
        virtual Float16** ReadArrayValue(const std::string& name, Float16* (&list)[], size_t size) = 0;
        virtual float** ReadArrayValue(const std::string& name, float* (&list)[], size_t size)     = 0;
        virtual double** ReadArrayValue(const std::string& name, double* (&list)[], size_t size)   = 0;
        virtual nemesis::hkCString**
        ReadArrayValue(const std::string& name, nemesis::hkCString* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkStringPtr**
        ReadArrayValue(const std::string& name, nemesis::hkStringPtr* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkEnumBase**
        ReadArrayValue(const std::string& name, nemesis::hkEnumBase* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkVector4**
        ReadArrayValue(const std::string& name, nemesis::hkVector4* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkVector8**
        ReadArrayValue(const std::string& name, nemesis::hkVector8* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkQuaternion**
        ReadArrayValue(const std::string& name, nemesis::hkQuaternion* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkQsTransform**
        ReadArrayValue(const std::string& name, nemesis::hkQsTransform* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkUFloat8**
        ReadArrayValue(const std::string& name, nemesis::hkUFloat8* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkLong**
        ReadArrayValue(const std::string& name, nemesis::hkLong* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkUlong**
        ReadArrayValue(const std::string& name, nemesis::hkUlong* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkHalf**
        ReadArrayValue(const std::string& name, nemesis::hkHalf* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkTransform**
        ReadArrayValue(const std::string& name, nemesis::hkTransform* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkMatrix3**
        ReadArrayValue(const std::string& name, nemesis::hkMatrix3* (&list)[], size_t size)
            = 0;
        virtual nemesis::hkMatrix4**
        ReadArrayValue(const std::string& name, nemesis::hkMatrix4* (&list)[], size_t size)
            = 0;

        virtual nemesis::HavokObject**
        ReadArrayObject(const std::string& name, nemesis::HavokObject* (&list)[], size_t size)
            = 0;

        virtual nemesis::hkRefVariant**
        ReadArrayRefObject(const std::string& name, nemesis::hkRefVariant* (&list)[], size_t size)
            = 0;

        virtual nemesis::hkSmallArrayBase& ReadArrayObject(const std::string& name,
                                                           nemesis::hkSmallArrayBase& array)
            = 0;
        virtual nemesis::hkArrayBase& ReadArrayObject(const std::string& name, nemesis::hkArrayBase& array)
            = 0;
        virtual nemesis::hkRefVariant& ReadRefObject(const std::string& name, nemesis::hkRefVariant& ref_obj)
            = 0;
        virtual nemesis::HavokObject** ReadRefObject(const std::string& name,
                                                     nemesis::HavokObject** ptr_ptr_obj)
            = 0;

    public:
        template <typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
        inline void ReadValue(const std::string& name, T& enum_val)
        {
            using UnderlyingType = typename std::underlying_type_t<T>;
            UnderlyingType val;
            ReadValue(name, val);
            enum_val = static_cast<T>(val);
        }

        template <typename T, size_t N>
        void ReadValue(const std::string& name, std::array<T, N>& array)
        {
            static_assert(std::is_arithmetic_v<T> || std::is_same_v<Float16, T>
                              || std::is_base_of_v<nemesis::hkVariant, T>,
                          "Only primitive numeric types are allowed");

            T* p_array[N];

            for (size_t i = 0; i < N; ++i)
            {
                p_array[i] = &array[i];
            }

            ReadArrayValue(name, p_array, N);
        }

        template <typename T,
                  size_t N,
                  std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<Float16, T>
                                       || std::is_base_of_v<nemesis::hkVariant, T>,
                                   int>
                  = 0>
        void ReadValue(const std::string& name, nemesis::hkAlign<N, T>& aligned_obj)
        {
            Pad(N);
            ReadValue(name, aligned_obj.Data);
        }

        template <typename T, size_t N, std::enable_if_t<std::is_base_of_v<nemesis::HavokObject, T>, int> = 0>
        void ReadObject(const std::string& name, nemesis::hkAlign<N, T>& aligned_obj)
        {
            Pad(N);
            ReadValue(name, aligned_obj.Data);
        }

        template <typename T, std::enable_if_t<is_complete_v<T>, int> = 0>
        void ReadObject(const std::string& name, T*& obj_ptr)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>, "Only havok objects are allowed");

            nemesis::HavokObject* hkx_ptr = static_cast<nemesis::HavokObject*>(obj_ptr);
            ReadRefObject(name, &hkx_ptr);
        }
        
        void ReadObject(const std::string& name, void* obj_ptr);

        template <typename T, size_t N>
        void ReadObject(const std::string& name, std::array<T*, N>& array)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>, "Only havok objects are allowed");

            if constexpr (std::is_base_of_v<nemesis::hkRefVariant, T>)
            {
                nemesis::hkRefVariant* p_array[N];

                for (size_t i = 0; i < N; ++i)
                {
                    p_array[i] = array[i];
                }

                ReadArrayRefObject(name, p_array, N);
            }
            else
            {
                nemesis::HavokObject* p_array[N];

                for (size_t i = 0; i < N; ++i)
                {
                    p_array[i] = array[i];
                }

                ReadArrayObject(name, p_array, N);
            }
        }
        
        template <typename T, size_t N>
        void ReadObject(const std::string& name, std::array<T, N>& array)
        {
            static_assert(std::is_base_of_v<nemesis::HavokObject, T>, "Only havok objects are allowed");

            if constexpr (std::is_base_of_v<nemesis::hkRefVariant, T>)
            {
                nemesis::hkRefVariant* p_array[N];

                for (size_t i = 0; i < N; ++i)
                {
                    p_array[i] = &array[i];
                }

                ReadArrayRefObject(name, p_array, N);
            }
            else
            {
                nemesis::HavokObject* p_array[N];

                for (size_t i = 0; i < N; ++i)
                {
                    p_array[i] = &array[i];
                }

                ReadArrayObject(name, p_array, N);
            }
        }

        virtual nemesis::hkClass& ReadClass(nemesis::hkClass& cls);

        nemesis::hkSmallArray<nemesis::HavokObject>&
        ReadObject(const std::string& name, nemesis::hkSmallArray<nemesis::HavokObject>& array);

        template <typename T>
        nemesis::hkSmallArray<T>& ReadObject(const std::string& name, nemesis::hkSmallArray<T>& array);
        
        nemesis::hkArray<nemesis::HavokObject>& ReadObject(const std::string& name,
                                                           nemesis::hkArray<nemesis::HavokObject>& array);

        template <typename T>
        nemesis::hkArray<T>& ReadObject(const std::string& name, nemesis::hkArray<T>& array);

        nemesis::hkRefPtr<nemesis::HavokObject>&
        ReadObject(const std::string& name, nemesis::hkRefPtr<nemesis::HavokObject>& ref_obj);

        template <typename T, bool Fixed>
        nemesis::hkRefPtr<T, Fixed>& ReadObject(const std::string& name,
                                                nemesis::hkRefPtr<T, Fixed>& ref_obj);

        virtual nemesis::HavokObject& ReadObject(const std::string& name, nemesis::HavokObject& hkx_obj) = 0;

        virtual std::string ReadString(const std::string& name)                                     = 0;
        virtual std::string ReadString(const std::string& name, unsigned int size, char pad = 0x00) = 0;
        virtual nemesis::hkCString ReadCString(const std::string& name)                             = 0;
        virtual nemesis::hkStringPtr ReadStringPointer(const std::string& name)                     = 0;

        virtual uint64_t ReadUSize();
        virtual void ReadFileHeader(nemesis::hkPackfileHeader& header);

        template <typename T, std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>, int> = 0>
        T AssertValue(T& ref_val, std::initializer_list<T> options)
        {
            assert(options.size() > 0 && "Empty options is not supported");

            if (!CanAssert()) return *options.begin();

            size_t pos = Stream->tellg();
            T val      = Read<T>();
            std::string optional_val;

            for (const T& option : options)
            {
                if (val == option) return ref_val = val;

                optional_val.append(std::to_string(option) + "(" + NumberToHex(option, true) + ")|");
            }

            optional_val    = optional_val.substr(0, optional_val.size() - 1);
            std::string msg = "Detected value: " + std::to_string(val) + "(" + NumberToHex(val, true)
                              + "), Expected value: " + optional_val + ", Position: " + std::to_string(pos)
                              + "(" + NumberToHex(pos, true) + ")";
            throw std::runtime_error(msg);
        }

        template <typename T, std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>, int> = 0>
        T AssertValue(std::initializer_list<T> options)
        {
            assert(options.size() > 0 && "Empty options is not supported");

            if (!CanAssert()) return *options.begin();

            size_t pos = Stream->tellg();
            T val      = Read<T>();
            std::string optional_val;

            for (const T& option : options)
            {
                if (val == option) return val;

                optional_val.append(std::to_string(option) + "(" + NumberToHex(option, true) + ")|");
            }

            optional_val    = optional_val.substr(0, optional_val.size() - 1);
            std::string msg = "Detected value: " + std::to_string(val) + "(" + NumberToHex(val, true)
                              + "), Expected value: " + optional_val + ", Position: " + std::to_string(pos)
                              + "(" + NumberToHex(pos, true) + ")";
            throw std::runtime_error(msg);
        }

        template <typename T,
                  std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>, int> = 0,
                  size_t N>
        T AssertValue(T& ref_val, const T (&options)[N])
        {
            static_assert(N > 0 && "Empty options is not supported");

            if (!CanAssert()) return options[0];

            size_t pos = Stream->tellg();
            T val      = Read<T>();
            std::string optional_val;

            for (const T& option : options)
            {
                if (val == option) return ref_val = val;

                optional_val.append(std::to_string(option) + "(" + NumberToHex(option, true) + ")|");
            }

            optional_val    = optional_val.substr(0, optional_val.size() - 1);
            std::string msg = "Detected value: " + std::to_string(val) + "(" + NumberToHex(val, true)
                              + "), Expected value: " + optional_val + ", Position: " + std::to_string(pos)
                              + "(" + NumberToHex(pos, true) + ")";
            throw std::runtime_error(msg);
        }

        template <typename T,
                  std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>, int> = 0,
                  size_t N>
        T AssertValue(const T (&options)[N])
        {
            static_assert(N > 0 && "Empty options is not supported");

            if (!CanAssert()) return options[0];

            size_t pos = Stream->tellg();
            T val      = Read<T>();
            std::string optional_val;

            for (const T& option : options)
            {
                if (val == option) return val;

                optional_val.append(std::to_string(option) + "(" + NumberToHex(option, true) + ")|");
            }

            optional_val    = optional_val.substr(0, optional_val.size() - 1);
            std::string msg = "Detected value: " + std::to_string(val) + "(" + NumberToHex(val, true)
                              + "), Expected value: " + optional_val + ", Position: " + std::to_string(pos)
                              + "(" + NumberToHex(pos, true) + ")";
            throw std::runtime_error(msg);
        }

        virtual std::string AssertString(std::initializer_list<std::string> options);
        virtual std::string AssertString(unsigned int size, std::initializer_list<std::string> options);
        virtual uint64_t AssertUSize(std::initializer_list<uint64_t> options);

        unsigned int GetPointerSize() const noexcept;
        unsigned int GetEndian() const noexcept;
        unsigned int GetPaddingOption() const noexcept;
        const std::string& GetContentsVersionString() const noexcept;
        nemesis::HavokVersion GetContentsVersion() const noexcept;
    };
}

#include "Serialize/Deserializer.hpp"
