
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_WORLDVIEWER_BASE_H
#define HK_UTILITIES2_WORLDVIEWER_BASE_H

#include <Physics/Dynamics/World/Listener/hkpWorldDeletionListener.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#include <Common/Visualize/hkProcess.h>

class hkpWorldViewerBase :  public hkReferencedObject, 
							public hkProcess,
							protected hkpPhysicsContextWorldListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

		// Ctor will register as all the listeners
		hkpWorldViewerBase( const hkArray<hkProcessContext*>& contexts );

		// Dtor will unregister as all the listeners, from the context if any
		virtual ~hkpWorldViewerBase();

		// Sub classes should implement what they require from the following:
		
		//Process:
		virtual void getConsumableCommands( hkUint8*& commands, int& numCommands ) { commands = HK_NULL; numCommands = 0; }
		virtual void consumeCommand( hkUint8 command ) { }
		virtual void step( hkReal frameTimeInMs ) { /* nothing to do, driven by the simulation callbacks usually */ }

		//World deletion listener:
		virtual void worldRemovedCallback( hkpWorld* world ) { }
		
		//World added listener. Should impl this in sub class, but call up to this one to get the listener reg'd.
		virtual void worldAddedCallback( hkpWorld* world ) { }

	protected:

		hkpPhysicsContext* m_context; 
};

#endif // HK_UTILITIES2_WORLDVIEWER_BASE_H

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
