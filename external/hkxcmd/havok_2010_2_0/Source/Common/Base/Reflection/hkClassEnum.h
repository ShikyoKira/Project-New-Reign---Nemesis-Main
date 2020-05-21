/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_CLASS_ENUM_H
#define HK_BASE_CLASS_ENUM_H

class hkClass;
class hkStreamWriter;
class hkCustomAttributes;

/// hkClassEnum meta information
extern const hkClass hkClassEnumClass;
extern const hkClass hkClassEnumItemClass;

/// Reflection object for enumerated types.
class hkClassEnum
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_HKCLASS, hkClassEnum);
		HK_DECLARE_REFLECTION();

			/// A single enumerated name and value pair.
		class Item
		{
			public:
				
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, Item);
				HK_DECLARE_REFLECTION();

					/// Create a new enumerated item.
				Item( int v, const char* n) : m_value(v), m_name(n) { }

					/// Get the name of this item.
				const char* getName() const { return m_name; }

					/// Get the integer value of this item.
				int getValue() const { return m_value; }

			private:

				int m_value;
				const char* m_name;
		};

		enum FlagValues
		{
			FLAGS_NONE = 0
		};
		typedef hkFlags<FlagValues, hkUint32> Flags;

			/// Create an enumerated type.
		hkClassEnum( const char* name, const Item* items, int numItems );

			/// Get the name of this type.
		const char* getName() const;

			/// Get the number of values of this enum.
		int getNumItems() const;

			/// Get the i'th item.
		const hkClassEnum::Item& getItem(int i) const;

			/// Get the name of the item with value val.
			/// Not to be confused with the i'th item.
		hkResult getNameOfValue( int val, const char** name ) const;

			/// Get the value of the item named name (note: case insensitive)
			/// Not to be confused with the index of the item.
		hkResult getValueOfName( const char* name, int* val ) const;

			///
		hkResult decomposeFlags( int flagValue, hkArray<const char*>& bitsOut, int& bitsOver ) const;

			/// Get the enum signature for versioning.
		hkUint32 getSignature() const;

			/// Write the enum signature for versioning.
		void writeSignature( hkStreamWriter* w ) const;

			///
		const hkVariant* getAttribute(const char* name) const;

			/// See the Flags enum for a description of flags.
		inline const Flags& getFlags() const;

			/// See the Flags enum for a description of flags.
		inline Flags& getFlags();
		
	private:

		const char* m_name;
		const class Item* m_items;
		int m_numItems;
		class hkCustomAttributes* m_attributes; //+serialized(false)
		Flags m_flags;
};

#include <Common/Base/Reflection/hkClassEnum.inl>

#endif // HK_BASE_CLASS_ENUM_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
