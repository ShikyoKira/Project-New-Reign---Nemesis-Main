/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_PHANTOM_LISTENER_H
#define HK_DYNAMICS2_PHANTOM_LISTENER_H


#include <Physics/Dynamics/Phantom/hkpPhantom.h>

 /// Implement this class and add it to hkpWorld or hkpPhantom
 /// to receive a callback if any of the following events occur
class hkpPhantomListener
{
	//+hk.MemoryTracker(ignore=True)
	public:
			/// Virtual destructor for derived objects
		virtual ~hkpPhantomListener() {}

			/// Called when an phantom is added to the hkpWorld.
		virtual void phantomAddedCallback( hkpPhantom*  ) {}

			/// Called when an phantom is removed from the hkpWorld.
		virtual void phantomRemovedCallback( hkpPhantom*  ) {}

			/// Called when a phantom changes its shape.
		virtual void phantomShapeSetCallback( hkpPhantom* phantom )
		{
			// Most of Havok's phantom listeners (VDB viewers) are only concerned about phantoms that are in the world.
			if (phantom->getWorld())
			{
				// Since Havok's VDB viewers have do destroy old and create new display information, we can simply call:
				phantomRemovedCallback(phantom);
				phantomAddedCallback(phantom);
			}
		}

			/// Called when an phantom is deleted.
		virtual void phantomDeletedCallback( hkpPhantom*  ) {}
};


#endif	// HK_DYNAMICS2_PHANTOM_LISTENER_H

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
