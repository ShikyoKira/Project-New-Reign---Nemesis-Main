/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_LOADED_OBJECT_REGISTRY_H
#define HK_SERIALIZE_LOADED_OBJECT_REGISTRY_H

#include <Common/Base/Container/StringMap/hkStringMap.h>
#include <Common/Base/Reflection/hkTypeInfo.h>

/// Helper class to initialize object vtables.
/// When objects are loaded we call placement new on them
/// (with the hkFinishLoadedObjectFlag) to "finish" the object
/// and make it ready to use. This class maintains a map of
/// class names to finishing functions.
class hkTypeInfoRegistry : public hkReferencedObject, public hkSingleton<hkTypeInfoRegistry>
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkTypeInfoRegistry(int finishFlag=1, int informMemoryTracker = 1) : m_finishFlag(finishFlag), m_informMemoryTracker(informMemoryTracker) {}

			/// Add a hook for loaded objects of type "className".
			/// The name is not copied and must be valid for the lifetime
			/// of this object.
		virtual void registerTypeInfo( const hkTypeInfo* info )
		{
			m_map.insert( info->getTypeName(), info );
		}

			/// Perform the final steps to finish loading an object.
			/// Generally this will involve initializing a vtable and/or
			/// setting up cached/computed values.
		virtual const hkTypeInfo* finishLoadedObject( void* obj, const char* className ) const
		{
			if( const hkTypeInfo* t = m_map.getWithDefault( className, HK_NULL ) )
			{
				t->finishLoadedObject(obj, m_finishFlag);
				return t;
			}
			return HK_NULL;
		}

			/// Perform the final step cleaning a loaded object before the memory is deallocated.
			/// This function should be used only to clean non-virtual objects. Use removeReference()
			/// to clean virtual objects.
			/// Generally this will involve calling the object destructor.
			/// 
		virtual const hkTypeInfo* cleanupLoadedObject( void* obj, const char* className ) const
		{
			const hkTypeInfo* t = m_map.getWithDefault( className, HK_NULL );
			if( t )
			{
				t->cleanupLoadedObject(obj);
			}
			else
			{
				HK_WARN(0x4e34ea5f, "Class " << className << " not found in hkCleanupLoadedObjectRegistry. Has it been registered?" );
			}
			return t;
		}

			/// Register a null terminated list of type infos.
		void registerList( const hkTypeInfo* const * infos)
		{
			const hkTypeInfo* const * ti = infos;
			while(*ti != HK_NULL)
			{
				registerTypeInfo( *ti );
				++ti;
			}
		}

			/// Merges all entries from "mergeFrom" (potentially overwriting current entries).
		virtual void merge(const hkTypeInfoRegistry& mergeFrom) 
		{
			hkStringMap<const hkClass*>::Iterator iter = mergeFrom.m_map.getIterator();
			while (mergeFrom.m_map.isValid(iter))
			{
				m_map.insert( mergeFrom.m_map.getKey(iter), mergeFrom.m_map.getValue(iter) );
				iter = mergeFrom.m_map.getNext(iter);
			}
		}

			/// Get registered type info by name.
		virtual const hkTypeInfo* getTypeInfo( const char* typeName ) const
		{
			return m_map.getWithDefault(typeName, HK_NULL);
		}

	protected:

		hkStringMap<const hkTypeInfo*> m_map;
		int m_finishFlag;
		int m_informMemoryTracker;
};

#endif // HK_SERIALIZE_LOADED_OBJECT_REGISTRY_H

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
