/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_INTERNAL_CONSTRAINT_UTILS_H
#define HK_DYNAMICS2_INTERNAL_CONSTRAINT_UTILS_H

extern "C"
{
	/// Calculates the relative angle between twist axes given plane axis
	void HK_CALL hkInternalConstraintUtils_calcRelativeAngle( 
		const hkVector4& twistAxisAinWorld, const hkVector4& twistAxisBinWorld, 
		const hkVector4& planeAxisAinWorld, const hkVector4& planeAxisBinWorld,
		hkVector4& axisOut, hkPadSpu<hkReal>& angleOut );
}


HK_FORCE_INLINE void HK_CALL hkInternalConstraintUtils_inlineCalcRelativeAngle( 
	const hkVector4& twistAxisAinWorld, const hkVector4& twistAxisBinWorld, 
	const hkVector4& planeAxisAinWorld, const hkVector4& planeAxisBinWorld,
	hkVector4& axisOut, hkPadSpu<hkReal>& angleOut, hkPadSpu<hkReal>& tauMaxOut ) 
{
	// Note : This algorithm is replicated inside the physics tools (max/maya)
	//		  Any changes should be ported there also.

	hkVector4 twist_axis_ws;
	twist_axis_ws.setAdd4( twistAxisBinWorld , twistAxisAinWorld );
	const hkReal twist_axis_ws_length3 = twist_axis_ws.length3().getReal();
	if (twist_axis_ws_length3>1e-16f)
	{
		tauMaxOut = twist_axis_ws_length3 * 0.5f;
		twist_axis_ws.mul4(hkSimdReal(1.0f / twist_axis_ws_length3));
	}
	else
	{
		tauMaxOut = 0.0f;
		twist_axis_ws = twistAxisBinWorld;
	}

	hkRotation m_ws_us;
	{
		hkVector4& c0 = m_ws_us.getColumn(0);
		hkVector4& c1 = m_ws_us.getColumn(1);
		hkVector4& c2 = m_ws_us.getColumn(2);
		c0 = twist_axis_ws;
		c1.setCross(twist_axis_ws, planeAxisBinWorld);
		c2.setCross(c1, c0);
	}
		
	hkReal d1 = m_ws_us.getColumn(1).dot3( planeAxisAinWorld ).getReal();
	hkReal d2 = m_ws_us.getColumn(2).dot3( planeAxisAinWorld ).getReal();
	hkReal currentTwistAngle  = hkMath_atan2fApproximation( d1, d2);
		// Explicitly specify angle calculated above. 
	angleOut = currentTwistAngle;
	axisOut = twist_axis_ws;
}

//
//	Computes the inertia scale factors for a linear constraint. These are used to make the constraint more stable
//	by scaling down the angular parts of the virtual mass.

HK_FORCE_INLINE void HK_CALL hkInternalConstraintUtils_computeInertiaScaleFactors(	const hkVector4& invInertiaA, const hkVector4& invInertiaB, 
																					const hkSimdReal& armLenA, const hkSimdReal& armLenB,
																					const hkSimdReal& scaleFactor,
																					hkSimdReal& fOutA, hkSimdReal& fOutB)
{
	const hkSimdReal maxInvInertiaA = invInertiaA.getHorizontalMax3();
	const hkSimdReal maxInvInertiaB = invInertiaB.getHorizontalMax3();

	hkSimdReal fA = maxInvInertiaA * armLenA * scaleFactor;	// For a given impulse, the angular velocity of body A is proportional to this term
	hkSimdReal fB = maxInvInertiaB * armLenB * scaleFactor;	// For a given impulse, the angular velocity of body B is proportional to this term

	const hkSimdReal eps = hkSimdReal::getConstant(HK_QUADREAL_EPS);
	const hkVector4Comparison cmpA = fA.compareGreaterThan(eps);
	const hkVector4Comparison cmpB = fB.compareGreaterThan(eps);
	fA.setReciprocal(fA);
	fB.setReciprocal(fB);
	fA = fA * invInertiaA.getSimdAt(3);
	fB = fB * invInertiaB.getSimdAt(3);

	const hkSimdReal one = hkSimdReal::getConstant(HK_QUADREAL_1);
	fA.setSelect(cmpA, fA, one);
	fB.setSelect(cmpB, fB, one);

	// Choose sub-unitary factors. If > 1 we are not modifying the problem
	// Artifacts appear if we use the same factor for both bodies!!
	fA.setMin(fA, one);
	fB.setMin(fB, one);
	fOutA = fA * armLenA;
	fOutB = fB * armLenB;
}

#endif // HK_DYNAMICS2_INTERNAL_CONSTRAINT_UTILS_H

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
