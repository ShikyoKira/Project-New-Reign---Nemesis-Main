/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULT_CONTROL_LISTENER_H
#define HK_DEFAULT_CONTROL_LISTENER_H

#include <Common/Base/hkBase.h>

class hkaDefaultAnimationControl;

/// Override this class to get callbacks for default control events
/// These events are : overflow, underflow and end of ease in/out
/// You need to call hkaDefaultAnimationControl::addDefaultControlListener() to add a new listener to a default control
class hkaDefaultAnimationControlListener
{
	//+hk.MemoryTracker(ignore=True)
	public:

		/// Called whenever the hkaDefaultAnimationControl overflows (hkaDefaultAnimationControl::update passes the end of the animation)
		virtual void loopOverflowCallback(hkaDefaultAnimationControl* control, hkReal deltaTime, hkUint32 overflows) {}

		/// Called whenever the hkaDefaultAnimationControl underflows (hkaDefaultAnimationControl::update passes the beginning of the animation)
		virtual void loopUnderflowCallback(hkaDefaultAnimationControl* control, hkReal deltaTime, hkUint32 underflows) {}

		/// Called whenever the hkaDefaultAnimationControl finishes easing in
		virtual void easedInCallback(hkaDefaultAnimationControl* control, hkReal deltaTime) {}

		/// Called whenever the hkaDefaultAnimationControl finishes easing out
		virtual void easedOutCallback(hkaDefaultAnimationControl* control, hkReal deltaTime) {}

			/// Virtual destructor for derived objects
		virtual ~hkaDefaultAnimationControlListener() {}
};


#endif // HK_DEFAULT_CONTROL_LISTENER_H

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
