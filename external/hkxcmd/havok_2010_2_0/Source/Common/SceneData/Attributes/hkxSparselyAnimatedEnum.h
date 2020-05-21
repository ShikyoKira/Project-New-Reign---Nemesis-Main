/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCENEDATA_HKX_ANIMATED_ENUM_H
#define HK_SCENEDATA_HKX_ANIMATED_ENUM_H

#include <Common/SceneData/Attributes/hkxSparselyAnimatedInt.h>

extern const class hkClass hkxEnumItemClass;

extern const class hkClass hkxEnumClass;

extern const hkClass hkxSparselyAnimatedEnumClass;

class hkxEnum : public hkReferencedObject
{
	//+vtable(true)
	//+version(0)
public: 

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);
	HK_DECLARE_REFLECTION();

	hkxEnum() { }
	hkxEnum(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_items(f) {}

	/// Get the name of the item with value val.
	/// Not to be confused with the i'th item.
	hkResult getNameOfValue( int val, const char** name ) const;

	/// Get the value of the item named name (note: case insensitive)
	/// Not to be confused with the index of the item.
	hkResult getValueOfName( const char* name, int* val ) const;

	struct Item
	{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA, Item);
		HK_DECLARE_REFLECTION();

		Item( ) { }
		Item( int v, const char* n) : m_value(v), m_name(n) { }
		Item( hkFinishLoadedObjectFlag f ) : m_name(f) {}

		int m_value;
		hkStringPtr m_name;
	};

	hkArray<struct hkxEnum::Item> m_items;

};

	/// An hkxSparselyAnimatedEnum stores the values associated with an hkxAttribute of type enum.
	/// This is the same as an hkxSparselyAnimatedInt with extra information about the conversion from int->string (enum), using
	/// an hkClassEnum object for that.
class hkxSparselyAnimatedEnum : public hkxSparselyAnimatedInt
{
	//+vtable(true)
	//+version(1)
public: 
	
	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
	HK_DECLARE_REFLECTION();

	hkxSparselyAnimatedEnum() { }
	hkxSparselyAnimatedEnum(hkFinishLoadedObjectFlag f) : hkxSparselyAnimatedInt(f), m_enum(f) {}
	
	hkRefPtr<hkxEnum> m_enum; // int value in parent class
};


#endif //HK_SCENEDATA_HKX_ANIMATED_ENUM_H

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
