/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MOTION_ANALYZER_UTILITY_H
#define HK_MOTION_ANALYZER_UTILITY_H

#include <Animation/Animation/MotionAnalysis/hkaFootStepTiming.h>
#include <Animation/Animation/MotionAnalysis/hkaExpandAnimationUtility.h>


class hkaMotionAnalyzerUtility
{
public:


	/// Tests if a bone is considered "down". Results are OR'd into the result vector
	/// bone Bone index
	/// up Up vector
	/// positionTolerance Position tolerance in model units
	/// velocityTolerance Velocity tolerance in model units (distance units, per frame)
	/// animation Animation to analyze
	/// isDownInOut Frames in which foot is considered down will be set true, other values not changed
	static void HK_CALL analyzeBoneDowns( hkInt16 bone, const hkVector4& up, hkReal positionTolerance, hkReal velocityTolerance, const hkaExpandAnimationUtility& animation, bool logicalOR, hkArray< hkBool >& isDownInOut );

	static void HK_CALL getPosition( hkInt16 bone, const hkVector4& up, const hkaExpandAnimationUtility& animation, hkArray< hkReal >& positionOut );

	static void HK_CALL getVelocity( hkInt16 bone, const hkVector4& up, const hkaExpandAnimationUtility& animation, hkArray< hkReal >& velocityOut );

	static void HK_CALL arrayMaximum( const hkArrayBase< hkReal >& a, const hkArrayBase< hkReal >& b, hkArray< hkReal >& out );

	static void HK_CALL arrayMinimum( const hkArrayBase< hkReal >& a, const hkArrayBase< hkReal >& b, hkArray< hkReal >& out );


	/// Tests if a bone is in front of another bone
	/// boneA Bone index of front bone
	/// boneB Bone index of back bone
	/// forward Forward direction
	/// animation Animation to analyze
	/// isInFrontInOut Frames in which boneA is in front will be set true, other values not changed
	static void HK_CALL isInFrontOf( hkInt16 boneA, hkInt16 boneB, const hkVector4& forward, const hkaExpandAnimationUtility& animation, hkArray< hkBool >& isInFrontInOut );


	/// \return The minimum of the forward and backward difference of a bone, projected into a plane
	/// bone Bone to analyze
	/// frame Frame to analyze at
	/// up Normal to the plane of projection
	/// animation Input animation
	static hkReal HK_CALL minimumPlanarDifference( hkInt16 bone, int frame, const hkVector4& up, const hkaExpandAnimationUtility& animation );


	/// Finds the maximum component of a bone's motion in a specified direction
	/// \param bone Bone to consider
	/// \param dir Direction to
	static hkReal HK_CALL maxDotPerBone( hkInt16 bone, const hkVector4& dir, const hkaExpandAnimationUtility& animation );
	static hkReal HK_CALL minDotPerBone( hkInt16 bone, const hkVector4& dir, const hkaExpandAnimationUtility& animation );

	/// Computes the cyclic backward difference between positions at a given frame
	/// \param bone Bone to analyze
	/// \param aniamtion World space animation to analyze
	static hkVector4 HK_CALL backwardDifference( hkInt16 bone, int frame, const hkaExpandAnimationUtility& animation );

	/// Computes the cyclic forward difference between positions at a given frame
	/// \param bone Bone to analyze
	/// \param aniamtion World space animation to analyze
	static hkVector4 HK_CALL  forwardDifference( hkInt16 bone, int frame, const hkaExpandAnimationUtility& animation );


	/// \return The index of the first frame in which the value changed to val
	/// \param aniamtion World space animation to analyze
	/// \param val Value
	static int getCyclicEventFrame( const hkArrayBase< hkBool >& values, hkBool val );

	/// \return The index of the first frame with value == val and cyclic previous != val
	static int findFirst( hkBool val, const hkArrayBase< hkBool >& values );

	/// Find frames with value == val and cyclic previous != val
	static void findTransitions( hkBool val, const hkArrayBase< hkBool >& values, hkArray< int >& framesOut );
};

#endif // HK_MOTION_ANALYZER_UTILITY_H

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
