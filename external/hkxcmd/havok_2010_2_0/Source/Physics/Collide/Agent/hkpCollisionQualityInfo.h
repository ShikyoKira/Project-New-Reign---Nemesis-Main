/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLIDE_QUALITY_INFO_H
#define HK_COLLIDE2_COLLIDE_QUALITY_INFO_H


	/// Hold information for fine tuning the collision detection system
struct hkpCollisionQualityInfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCollisionQualityInfo );

		/// The distance until the created contact points will be kept (e.g. 5 cm),
		/// should be identical to hkpCollisionInput.m_collisionTolerance
	HK_ALIGN16(hkReal m_keepContact);

		/// the distance for creating 4 dimensional contacts of type point-face or edge-edge
		/// Typically this is a small negative number, but still bigger than the combined radii
		/// of the object
	hkReal m_create4dContact;

		/// the range for creating contacts for all other contact points (PP or PE)
	hkReal m_createContact;

		/// The distance, for which the tim optimizations for manifold will be used.
		/// Set to HK_REAL_MAX if you want to disable this optimization (improved quality)
		/// Set to <0.0 (typically ~m_create4dContact) to enable
	hkReal m_manifoldTimDistance;

	hkPadSpu<hkBool32>		m_useContinuousPhysics;

		// Tells if the TOI should use simple handling. Simple handling is only used when debris-quality objects
		// are involved in a collision. It is handled by simply backstepping the debris objects till the initial time
		// of impact, and not reintegrating them at all.
	hkBool m_useSimpleToiHandling;

	//
	//	For continuous physics
	//

		/// The negative "total" maximum penetration depth. This parameter is further scaled by hkpCollidable::m_allowedPenetrationDepth.
	hkReal m_minSeparation;			// total

		/// The negative maximum penetration depth relative to the current distance. This parameter is further scaled by hkpCollidable::m_allowedPenetrationDepth.
	hkReal m_minExtraSeparation;	// incremental

		/// The internal time step to detect penetrations.
		/// This parameter somehow allows to step the physics at a higher rate to
		/// detect bullet through paper problems
	hkReal m_minSafeDeltaTime;

		/// Absolute min delta time used for internal continuous collision detection algorithm.
		/// When the distance traveled by a body in that time approaches the body's size,
		/// then continuous collision detection may fail / not pick up contacts.
	hkReal m_minAbsoluteSafeDeltaTime;

		/// The separation distance at which a TOI will be generated.
	hkReal m_toiSeparation;

		/// The separation distance relative to the current distance
		/// at which a TOI will be generated.
	hkReal m_toiExtraSeparation;

		/// The distance accuracy of TOI calculations.
	hkReal m_toiAccuracy;

		/// The relative maximum allowed penetration.
	hkReal m_maxContraintViolation;

		/// The minimum delta time between two TOIs.
	hkReal m_minToiDeltaTime;

		/// If you use hkDynamics, this field is the priority of constraints,
		/// else it is not used: hkpConstraintInstance::ConstraintPriority
	hkUint16	m_constraintPriority;

		/// Used to flag whether to drop TOI contacts that will cause welding artifacts.
	hkBool m_enableToiWeldRejection;
	
	hkReal calcMinSeparation( hkReal currentDistance ) const
	{
		return m_minSeparation;
	}
};


#endif // HK_COLLIDE2_COLLIDE_QUALITY_INFO_H

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
