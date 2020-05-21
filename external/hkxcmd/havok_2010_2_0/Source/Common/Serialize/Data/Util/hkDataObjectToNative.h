/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_DATAOBJECT_TO_NATIVE_H
#define HK_SERIALIZE_DATAOBJECT_TO_NATIVE_H

#include <Common/Serialize/Data/hkDataObject.h>

class hkClassNameRegistry;

	// Private utility class used by hkDataObjectUtil
	// This class copies data objects to their native counterparts.
class hkDataObjectToNative
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkDataObjectToNative);

		hkDataObjectToNative(const hkClassNameRegistry* classReg, hkBool32 allocatedOnHeap)
			: m_classReg(classReg), m_allocatedOnHeap(allocatedOnHeap)
		{
		}

		struct PointerInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkDataObjectToNative::PointerInfo);
			hkDataObject::Handle m_handle;
			void* m_addr;
			hkBool m_isVariant;
			hkBool m_isOwning;
		};
		struct Alloc
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkDataObjectToNative::Alloc);
			void set(void* addr, int size) { m_addr=addr; m_size=size; }
			void* m_addr;
			int m_size;
		};
		struct DummyArray
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkDataObjectToNative::DummyArray);
			void* data;
			int size; 
			int capacity; 
		};
		struct DummyRelArray
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkDataObjectToNative::DummyRelArray);
			hkUint16 size;
			hkUint16 offset;
		};
		
		struct CopyInfoOut
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkDataObjectToNative::CopyInfoOut);
			CopyInfoOut()
			{
				pointersOut.reserve(32);
			}
			void addPointer( const hkDataObject& obj, void* addr, hkBool32 isOwning)
			{
				hkDataObject::Handle handle = obj.getHandle();
				if( handle.p0 || handle.p1 )
				{
					PointerInfo& info = pointersOut.expandOne();
					info.m_handle = handle;
					info.m_addr = addr;
					info.m_isOwning = isOwning!=0;
					info.m_isVariant = false;
				}
			}
			void addVariant( const hkDataObject& obj, hkVariant* addr, hkBool32 isOwning)
			{
				hkDataObject::Handle handle = obj.getHandle();
				if( handle.p0 || handle.p1 )
				{
					PointerInfo& info = pointersOut.expandOne();
					info.m_handle = handle;
					info.m_addr = addr;
					info.m_isOwning = isOwning!=0;
					info.m_isVariant = true;
				}				
			}
			void addAlloc(void* p, int size)
			{
				allocs.expandOne().set(p,size);
			}

			hkArray<PointerInfo>::Temp pointersOut;
			hkArray<Alloc>::Temp allocs;
		};

			/// Create a new copy from the given object.
			/// Return any allocations in infoOut.
		hkVariant allocateObject( const hkDataObject& obj, CopyInfoOut& infoOut );
			/// Copy the members from a data object into a zero buffer.
			/// The buffer must be of the correct size (allocateObject does this).
			/// Any pointers encountered and any new allocations will be added to the infoOut.
			/// This method may fail if an m_allocatedOnHeap is true and a circular reference
			/// is encountered (because there is no way to clean up correctly).
		hkResult fillNativeMembers( void* dst, const hkDataObject& src, CopyInfoOut& infoOut );

	protected:

			/// Class reg of the copy target.
		const hkClassNameRegistry* m_classReg;
			/// True means copied objects are intended to be cleaned up with regular destructors.
			/// Some types only work for packfiles (e.g., simple arrays). The flattened memory of
			/// packfiles allows these to work without leaks.
		hkBool32 m_allocatedOnHeap;

			/// Find the native class for a data object.
		const hkClass* findClassOf( const hkDataObject& obj );
			/// 
		template<typename Value>
		hkResult fillNativeSimpleMember( void* address, hkClassMember::Type mtype, const Value& srcValue, const hkClass* klass, hkBool32 owningReference, CopyInfoOut& infoOut );
			///
		hkResult copyIntoNativeArray( void* address, const hkClassMember& member, const hkDataArray& garray, CopyInfoOut& infoOut);
			///
		hkResult copyIntoRelArray( void* address, const hkClassMember& member, const hkDataArray& srcArray, CopyInfoOut& infoOut, void*& relArrayAddress, void* objectStart );

			///
		hkResult fillNativeEnumMember( void* address, const hkClassMember& member, const hkDataObject::Value& value, const hkClass& klass );
			// Check that a data object obj may be safely assigned to a native member
			// Return the class which should be used for copying into the native object
		const hkClass* getMemberClassAndCheck(const hkClassMember& member, const hkDataObject& obj );
};

#endif // HK_SERIALIZE_DATAOBJECT_TO_NATIVE_H

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
