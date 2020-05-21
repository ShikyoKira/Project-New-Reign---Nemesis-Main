/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_RIGID_MOTION_UTIL_H
#define HK_DYNAMICS2_RIGID_MOTION_UTIL_H

class hkpEntity;
class hkStepInfo;
class hkpVelocityAccumulator;

extern "C"
{
		/// Single step without gravity or damping, used by TOI backstepping.
		///
		/// This function is capable of handling batches:
		/// Using the motions parameter one usually passes in a pointer to an array of hkMotions and leaves the motionOffset parameter set to 0.
		/// Apart from that it is also allowed to pass in a pointer to an array of any other class, e.g., hkpEntity. In this case you have to manually
		/// supply the motion's offset within this different class (e.g., motionOffset = HK_OFFSET_OF(hkpEntity, m_motion) )
	void HK_CALL hkRigidMotionUtilStep( const hkStepInfo& info, hkpMotion*const* motions, int numMotions, int motionOffset );

		/// Single step with gravity + damping.
		///
		/// This function is capable of handling batches: see hkRigidMotionUtilStep() for details.
		/// Returns the number of frames since all motions have not left the reference deactivation sphere.
	int HK_CALL hkRigidMotionUtilApplyForcesAndStep( const struct hkpSolverInfo& solverInfo, const hkStepInfo& info, const hkVector4& deltaVel, hkpMotion*const* motions, int numMotions, int motionOffset );


		/// Build accumulators
		/// This function decouples building accumulators and applying damping to facilitate multithreaded execution
	hkpVelocityAccumulator* HK_CALL hkRigidMotionUtilBuildAccumulators(const hkStepInfo&				info,
																		hkpMotion*const*				motions,
																		int								numMotions,
																		int								motionOffset,
																		hkpVelocityAccumulator*			accumulatorsOut);

		/// Apply damping forces and build accumulator (gravity will be applied to the accumulators later)
		///
		/// This function is capable of handling batches: see hkRigidMotionUtilStep() for details.
	hkpVelocityAccumulator* HK_CALL hkRigidMotionUtilApplyForcesAndBuildAccumulators(const hkStepInfo&				info,
																			hkpMotion*const*					motions,
																			int								numMotions,
																			int								motionOffset,
																			hkpVelocityAccumulator*			accumulatorsOut);

		/// Apply accumulator onto motion
		///
		/// This function is capable of handling batches: see hkRigidMotionUtilStep() for details.
		/// Returns the number of frames since all motions have not left the reference deactivation sphere
		///
		/// If processDeactivation == false, then function returns -1.
	int HK_CALL hkRigidMotionUtilApplyAccumulators( const struct hkpSolverInfo& solverInfo,
											const hkStepInfo&				info,
											const hkpVelocityAccumulator*	accumulators,
											      hkpMotion*const*			motions,
												  int						numMotions,
												  int						motionOffset );

		/// Recheck the velocities of the object against the reference velocities.
		///
		/// This is done to make sure that no island gets wrongfully deactivated.
		/// E.g. when a single object suddenly changed its velocity (e.g., hit by a raycast gun).
	bool HK_CALL hkRigidMotionUtilCanDeactivateFinal( const hkStepInfo& info, hkpMotion*const* motions, int numMotions, int motionOffset );
}



#endif // HK_DYNAMICS2_RIGID_MOTION_UTIL_H

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
