#pragma once

#include "Havok/Base/hkVariant.h"

#include "Havok/Types/hkInternalClassEnumItem.h"

#include "Utilities/TypeTraits.h"

namespace nemesis
{
    struct hkEnumBase : nemesis::hkVariant
    {
    public:
        virtual ~hkEnumBase() = default;

        virtual nemesis::hkEnumBase& operator=(const std::string&) = 0;

        virtual void reset_to_zero()    = 0;
        virtual int to_int() const      = 0;
        virtual std::string str() const = 0;
        virtual char* bin()             = 0;
        virtual const char* bin() const = 0;
        virtual size_t size() const     = 0;
    };

	template <typename EnumType, typename StorageType>
	struct hkEnum : nemesis::hkEnumBase
	{
        static_assert(std::is_same_v<nemesis::hkInternalClassEnumItem<StorageType, true>, EnumType>
                          || std::is_same_v<nemesis::hkInternalClassEnumItem<StorageType, false>, EnumType>,
                      "EnumType for hkInternalClassEnum must be hkInternalClassEnumItem type");
        static_assert(std::is_integral_v<StorageType>,
                      "StorageType for hkInternalClassEnum must be integral type");

	private:
        EnumType Enum;
        StorageType Storage{};

	public:
        hkEnum(std::initializer_list<std::pair<std::string, StorageType>> values)
            : Enum(values)
        {
        }

        hkEnum(const std::unordered_map<std::string, StorageType>& values)
            : Enum(values)
        {
        }

        hkEnum(std::unordered_map<std::string, StorageType>&& values)
            : Enum(values)
        {
        }

        nemesis::hkEnum<EnumType, StorageType>&
        operator=(std::initializer_list<std::pair<std::string, StorageType>> values)
        {
            Enum = values;
            return *this;
        }

        nemesis::hkEnum<EnumType, StorageType>&
        operator=(const std::unordered_map<std::string, StorageType>& values)
        {
            Enum = values;
            return *this;
        }

        nemesis::hkEnum<EnumType, StorageType>&
        operator=(std::unordered_map<std::string, StorageType>&& values)
        {
            Enum = values;
            return *this;
        }

        template <typename U = StorageType, std::enable_if_t<!std::is_same_v<U, int>, int> = 0>
        nemesis::hkEnum<EnumType, StorageType>& operator=(int i)
        {
            StorageType val = static_cast<StorageType>(i);
            Enum[val];
            Storage = val;
            return *this;
        }
        
        nemesis::hkEnum<EnumType, StorageType>& operator=(StorageType i)
        {
            Enum[i];
            Storage = i;
            return *this;
        }

        nemesis::hkEnumBase& operator=(const std::string& str) override
        {
            Storage = Enum[str];
            return *this;
        }

        StorageType& val()
        {
            return Storage;
        }
        
        const StorageType& val() const
        {
            return Storage;
        }

        void reset_to_zero() override
        {
            Storage = static_cast<StorageType>(0);
        }

        int to_int() const override
        {
            return static_cast<int>(Storage);
        }

        std::string str() const override
        {
            return Enum[Storage];
        }

        char* bin() override
        {
            return reinterpret_cast<char*>(&Storage);
        }

        const char* bin() const override
        {
            return reinterpret_cast<const char*>(&Storage);
        }

        size_t size() const noexcept override
        {
            return sizeof(StorageType);
        }
    };
}
