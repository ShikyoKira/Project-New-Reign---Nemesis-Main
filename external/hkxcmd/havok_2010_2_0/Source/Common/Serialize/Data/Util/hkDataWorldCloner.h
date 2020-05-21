/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DATA_WORLD_CLONER_H
#define HK_DATA_WORLD_CLONER_H

#include <Common/Serialize/Data/hkDataObject.h>

	// Private utility class used by hkDataObjectUtil
class hkDataWorldCloner
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkDataWorldCloner);
			/// Create a cloner with two worlds.
		hkDataWorldCloner( hkDataWorld& dstWorld, const hkDataWorld& srcWorld );

			/// Perform the clone.
		hkResult clone();

	protected:

		template<typename Value>
		void copySimpleValue(Value dst, const Value& src );
		void copyArray( hkDataArray& dst, const hkDataArray& src, int arraySize);
		void copyValue( hkDataObject& dstObj, const hkDataObject& srcObj, const hkDataClass::MemberInfo& minfo );
		hkDataClassImpl* findDestClass(const char* classname);
		void getClassMemberInfos( hkArray<hkDataClass::MemberInfo>::Temp& dstInfos, const hkDataClass& dstClass, const hkDataClass& srcClass );
		void copyObjectMembers( hkDataObject& dstObject, const hkDataObject& srcObject );
		hkDataObject copyObject( const hkDataObject& srcObject );

	protected:

		typedef hkMap<hkDataObject::Handle, hkDataObject::Handle> Map;

		hkDataWorld& m_dstWorld;
		const hkDataWorld& m_srcWorld;
		Map m_copied;
};

#endif // HK_DATA_WORLD_CLONER_H

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
