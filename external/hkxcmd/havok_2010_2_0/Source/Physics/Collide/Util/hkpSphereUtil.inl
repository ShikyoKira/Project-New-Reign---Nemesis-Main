/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Physics/Collide/hkpCollide.h>
#include <Common/Base/hkBase.h>
#include <Physics/Collide/Util/hkpSphereUtil.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>

inline hkBool HK_CALL hkpSphereUtil::castRayUtil(hkReal radius, const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results)
{
	//
	//	This functions is a modified version of
	//  http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm
	//  Modifications include changing the order of if statements to prevent
	//  any division which can produce a number greater than 1
	//

	{
	    hkReal	radius2 = radius * radius;
    
	    // 
	    // solve quadratic function: ax*x + bx + c = 0
	    //
	    hkVector4 dir; dir.setSub4( input.m_to, input.m_from);
    
	    hkReal B = dir.dot3( input.m_from ).getReal();
	    if ( B >= 0 )
	    {
		    // ray points away from sphere center
		    goto returnFalse;
	    }

		const hkReal A = dir.lengthSquared3().getReal();

		//
		//	Check for long rays (check for startpoints being 10 times outside the radius
		//
		hkReal offset;
		hkVector4 midPoint;
		if ( B * B > A * radius2 * 100.0f)
		{
			// no hit if length is smaller than the distance of the startpoint to the center
			if ( A < radius2 )
			{
				goto returnFalse;
			}
			offset = -B;
			midPoint.setInterpolate4(input.m_from, input.m_to, hkSimdReal(offset/A));
			B = 0.0f;
		}
		else
		{
			offset = 0.0f;
			midPoint = input.m_from;
		}

		const hkReal C = midPoint.lengthSquared3().getReal() - radius2;
		const hkReal det = B*B - A*C;

		if ( det <= 0 )
		{
			//
			//	Infinite ray does not hit
			//
			goto returnFalse;
		}

		const hkReal sqDet = hkMath::sqrt( det );

		const hkReal t2 = -B - sqDet;
		hkReal t = t2 + offset;

		if ( t >= (A * results.m_hitFraction))
		{
			//
			//	hits behind endpoint or is greater than previous hit fraction
			//
			goto returnFalse;
		}

		if ( t < 0 )
		{
			//
			// start point inside
			//
			goto returnFalse;
		}

		//  Note: we know that t > 0
		//  Also that A > t 
		//  So this division is safe and results in a point between 0 and 1

		t = t/A;

		results.m_hitFraction = t;
		results.m_normal.setInterpolate4( input.m_from, input.m_to, hkSimdReal(t) );
		results.m_normal.mul4( hkSimdReal(1.0f / radius) );
		results.setKey(HK_INVALID_SHAPE_KEY);
		return true;
	}

returnFalse:
	return false;
}

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
