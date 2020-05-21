/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_SERVER_PROCESS_SCHEDULER_H
#define HK_VISUALIZE_SERVER_PROCESS_SCHEDULER_H

#include <Common/Visualize/hkProcess.h>
#include <Common/Visualize/hkProcessFactory.h>
#include <Common/Visualize/hkProcessContext.h>
#include <Common/Visualize/hkCommandRouter.h>
#include <Common/Visualize/hkProcessHandler.h>
#include <Common/Visualize/hkServerDebugDisplayHandler.h>

class hkDisplaySerializeIStream;
class hkDisplaySerializeOStream;
class hkStreamReader;
class hkStreamWriter;

/// This holds the control over Processes and their life cycle.
/// It is in itself a master process (can send and recv process related commands too)
class hkServerProcessHandler : public hkReferencedObject,
			public hkProcessHandler, public hkProcess
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

		hkServerProcessHandler( hkArray<hkProcessContext*>& contexts, 
							hkStreamReader* inStream, 
							hkStreamWriter* outStream);

		~hkServerProcessHandler();
		
		virtual void getConsumableCommands( hkUint8*& commands, int& numCommands );
		virtual void consumeCommand( hkUint8 command );

		// Client Implemented
		virtual hkResult registerProcess(const char* name, int tag);
		virtual hkResult selectProcess(int tag);
		// Server Implemented
		virtual hkResult createProcess(int tag);
		virtual hkResult deleteProcess(int tag);

		// Util funcs
		virtual int getProcessId(const char* name) { return hkProcessFactory::getInstance().getProcessId( name ); }
		virtual const char* getProcessName( int tag ) { return hkProcessFactory::getInstance().getProcessName( tag ); }
		virtual hkProcess* getProcessByName( const char* name );
		virtual void getProcessesByType( int type, hkArray<hkProcess*>& processes );

			/// Will step all processes forward in time (if the impl step)
			/// and will then write a step chunk (0x0) to the out stream.
		void step( hkReal frameTimeInMs );

		virtual int getProcessTag() { return m_tag; } // always just 0;

		hkResult registerAllAvailableProcesss();

		const hkArray<hkProcess*>& getProcessList() const { return m_processList; }

	protected:

		int findProcessByTag(int tag);

		hkCommandRouter m_commandRouter; // maps incoming data to Processes that want to service that commnd
		hkArray<hkProcess*> m_processList; // owned
		hkArray<hkProcessContext*> m_contexts; // not owned.
		
		static int m_tag;
};

#endif // HK_VISUALIZE_SERVER_PROCESS_SCHEDULER_H

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
