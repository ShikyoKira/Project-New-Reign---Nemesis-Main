/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_HINGE_LIMITS_DRAWER_H
#define HK_HINGE_LIMITS_DRAWER_H

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpConstraintDrawer.h>
#include <Common/Visualize/Shape/hkDisplaySemiCircle.h>
#include <Common/Base/hkBase.h>

/// Displays information about the ballSocket constraint.
class hkpHingeLimitsDrawer : public hkpConstraintDrawer
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VDB, hkpHingeLimitsDrawer );

		void drawConstraint(hkpConstraintInstance* constraint, hkDebugDisplayHandler* displayHandler, int id, int tag);

	protected:
		hkDisplaySemiCircle m_angularLimit;

};

#endif //HK_HINGE_LIMITS_DRAWER_H

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
