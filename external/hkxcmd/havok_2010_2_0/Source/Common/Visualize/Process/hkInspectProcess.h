/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_INSPECT_PROCESS_H
#define HK_INSPECT_PROCESS_H

#include <Common/Visualize/hkProcess.h>
#include <Common/Visualize/hkProcessContext.h>
#include <Common/Serialize/Serialize/Platform/hkPlatformObjectWriter.h>

/// Given a set of tracked objects (as given by the list in hkVisualDebugger)
/// give the attached client the list, and upon request drill down and supply the
/// actual data.
#define HK_INSPECT_FLAG_AUTOUPDATE 0x01
#define HK_INSPECT_FLAG_RECURSE    0x02

class hkInspectProcess : public hkReferencedObject, public hkProcess
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);
			
		static hkProcess* HK_CALL create(const hkArray<hkProcessContext*>& contexts);
			
		static void HK_CALL registerProcess();

		virtual void getConsumableCommands( hkUint8*& commands, int& numCommands );

		virtual void consumeCommand( hkUint8 command );

			/// Gets the tag associated with this viewer type
		virtual int getProcessTag() { return m_tag; }

		static inline const char* HK_CALL getName() { return "ObjectInspection"; }

			// step only preforms work if auto update is turned on for any objects
			// ie: m_autoUpdateList is not empyt
		virtual void step(hkReal frameTimeInMs);

		virtual void init();

		static const class hkClass* s_specialClassClasses[];
		static const int s_numSpecialClassClasses;

	// internal stuff (public for the callbacks)
	public:

		struct ObjectPair
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkInspectProcess::ObjectPair );

			const void* obj;
			const hkClass* klass;
		};

		int writeObject( const void* ptr, const hkClass& klass, hkBool recur, hkPointerMap<const void*, const hkClass*>& writtenObjects );
		int addTopLevelObject( void* ptr, const hkClass& klass, hkUlong tagId );
		int removeTopLevelObject( void* ptr );
				
		hkArray<ObjectPair> m_autoUpdateList;		
		class hkVisualDebugger* m_vdb;

		hkInspectProcess(const hkArray<hkProcessContext*>& contexts);
		virtual ~hkInspectProcess();

	protected:

		static int m_tag;
		hkPlatformObjectWriter::Cache* m_cache;

};

#endif	// HK_INSPECT_PROCESS_H

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
