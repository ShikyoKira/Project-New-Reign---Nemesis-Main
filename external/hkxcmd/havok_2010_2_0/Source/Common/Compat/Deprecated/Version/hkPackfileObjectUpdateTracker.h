/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_PACKFILEOBJECTUPDATETRACKER_H
#define HK_SERIALIZE_PACKFILEOBJECTUPDATETRACKER_H

#include <Common/Compat/Deprecated/Version/hkObjectUpdateTracker.h>
#include <Common/Serialize/Util/hkSerializeMultiMap.h>
#include <Common/Serialize/Util/hkSerializeLog.h>
#include <Common/Serialize/Packfile/hkPackfileData.h>

/// A tracker which understands the layout of packfiles.
class hkPackfileObjectUpdateTracker : public hkObjectUpdateTracker
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkPackfileObjectUpdateTracker( hkPackfileData* data );
		~hkPackfileObjectUpdateTracker();

		void setTopLevelObject( void* topLevel, const char* topClass );
		const char* getTopLevelClassName();

			//
			// Memory
			//

		virtual void addAllocation(void* p);
		virtual void addChunk(void* p, int n, HK_MEMORY_CLASS c);

			//
			// Pointers
			//

		virtual void objectPointedBy( void* newObject, void* fromWhere );
		virtual void replaceObject( void* oldObject, void* newObject, const hkClass* newClass );

			//
			// Finish
			//

		virtual void addFinish( void* newObject, const char* className );
		virtual void removeFinish( void* oldObject );

		hkPackfileData* m_packfileData;
        hkSerializeMultiMap<void*, void*> m_pointers;
		hkPointerMap<void*, const char*> m_finish;
		void* m_topLevelObject;
};

#endif //HK_SERIALIZE_PACKFILEOBJECTUPDATETRACKER_H

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
