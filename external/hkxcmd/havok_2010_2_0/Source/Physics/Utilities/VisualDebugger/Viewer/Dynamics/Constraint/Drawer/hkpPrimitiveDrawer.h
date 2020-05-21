/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PRIMITIVEDRAWER_H
#define HK_PRIMITIVEDRAWER_H

#include <Common/Base/hkBase.h>
//#include <hkutilities/hkHavok2Common.h>
//#include <hkdynamics/constraint/hkpConstraintInstance.h>
//#include <hkdynamics/constraint/hkRigidConstraint.h>
//#include <hkvisualize/type/hkColor.h>
//#include <hkvisualize/hkDebugDisplayHandler.h>

/// Base class for constraint viewers
class hkDebugDisplayHandler;

class hkpPrimitiveDrawer 
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VDB, hkpPrimitiveDrawer );
	protected:

		hkDebugDisplayHandler* m_displayHandler;

	public:

        hkpPrimitiveDrawer();
		
			/// Sets the display handler
		void setDisplayHandler(hkDebugDisplayHandler* displayHandler);

			/// Displays an oriented point.
		void displayOrientedPoint(const hkVector4& position,const hkRotation& rot,hkReal size, int color, int id, int tag);

			/// Displays an arrow.
		void displayArrow (const hkVector4& startPos, const hkVector4& arrowDirection, const hkVector4& perpDirection, int color, hkReal scale, int id, int tag);
        
			/// Displays a cone.
		void displayCone (hkReal cone_angle, const hkVector4& startPos, const hkVector4& coneAaxis, const hkVector4& perpVector, int numSegments, int color, hkReal coneSize, int id, int tag);

			/// Displays a plane.
		void displayPlane(const hkVector4& startPos, const hkVector4& planeNormal, const hkVector4& vectorOnPlane, int color, hkReal scale, int id, int tag);

			/// Draws a semi circle.
			/// \param center The position of the center in world space.
			/// \param normal The axis about which the circle is drawn.
			/// \param startPerp An orthogonal axis to the normal which defines the start of the sweep.
		void drawSemiCircle(const hkVector4& center, hkVector4& normal, 
						    hkVector4& startPerp, hkReal thetaMin, hkReal thetaMax,
							hkReal radius,int numSegments, int color, int id, int tag);
};

#endif // HK_PRIMITIVEDRAWER_H

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
