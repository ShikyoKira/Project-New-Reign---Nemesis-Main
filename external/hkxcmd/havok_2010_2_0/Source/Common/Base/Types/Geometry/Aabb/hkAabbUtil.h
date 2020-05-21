/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_COLLIDE2_AABB_UTIL_H
#define HK_COLLIDE2_AABB_UTIL_H

#include <Common/Base/Types/Physics/MotionState/hkMotionState.h>	// needed for .inl file

class hkAabb;
class hkTransform;
class hkVector4;


/// A utility class for creating AABBs that contain various geometric objects.
class hkAabbUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkAabbUtil);
		//
		// Non-continuous AABB calculations
		//

			/// Calculates an AABB from an array of vertices.
		static void HK_CALL				calcAabb( const float* vertexArray, int numVertices, int striding, hkAabb& aabbOut );
		
			/// Calculates an AABB from an array of hkVector4.
		static void HK_CALL				calcAabb( const hkVector4* vertexArray, int numVertices, hkAabb& aabbOut );

			/// Calculates an AABB from an array of hkVector4 pointers.
		static void HK_CALL				calcAabb( const hkVector4*const* vertexArray, int numVertices, hkAabb& aabbOut );

			/// Calculates an AABB from an OBB specified by a transform, a center, and an halfExtents vector and an extra radius
		static inline void HK_CALL		calcAabb( const hkTransform& BvToWorld, const hkVector4& halfExtents, const hkVector4& center, hkSimdRealParameter extraRadius, hkAabb& aabbOut);

			/// Calculates an AABB from an OBB specified by a transform, and an halfExtents vector.
		static inline void HK_CALL		calcAabb( const hkTransform& BvToWorld, const hkVector4& halfExtents, hkSimdRealParameter extraRadius, hkAabb& aabbOut);

			/// Calculates an AABB from an OBB specified by a transform, and an AABB.
		static void HK_CALL				calcAabb(const hkTransform& BvToWorld, const hkAabb& aabb, hkSimdRealParameter extraRadius, hkAabb& aabbOut);

			/// Calculates an AABB from center and inner radius.
		static inline void HK_CALL		calcAabb( const hkVector4& center, hkSimdRealParameter innerRadius, hkAabb& aabbOut);

			/// Get the outer radius of an AABB.
		static inline hkReal HK_CALL	getOuterRadius(const hkAabb& aabb);

			/// Get the distance squared from point to an AABB, with point inside AABB distance == 0.
		static inline hkReal			distanceSquared(const hkAabb& a,const hkVector4& b);

			/// Get a vertex of an AABB. (with index bits set for positive as follow: Z | Y | X)
		static inline void				getVertex(const hkAabb& aabb, int index, hkVector4& vertexOut);

			/// Scale an AABB.
		static HK_FORCE_INLINE void		scaleAabb(const hkVector4& scale, const hkAabb& aabbIn, hkAabb& aabbOut);

			/// Expand an AABB by a combination of linear and angular motion.
			/// The resulting AABB is a follow:
			/// newMin = MIN(aabb.min + motionXYZ , aabb.min) - {motionW,motionW,motionW}
			/// newMax = MAX(aabb.max + motionXYZ , aabb.max) + {motionW,motionW,motionW}
		static HK_FORCE_INLINE void		expandAabbByMotion(const hkAabb& aabbIn, const hkVector4& motion, hkAabb& aabbOut);

			/// Project an AABB on an axis.
		static HK_FORCE_INLINE void		projectAabbOnAxis(const hkVector4& axis, const hkAabb& aabb, hkSimdReal& minOut, hkSimdReal& maxOut);

			/// Project the minimum extend of an AABB on an axis.
		static HK_FORCE_INLINE void		projectAabbMinOnAxis(const hkVector4& axis, const hkAabb& aabb, hkSimdReal& prjOut);

			/// Project the maximum extend of an AABB on an axis.
		static HK_FORCE_INLINE void		projectAabbMaxOnAxis(const hkVector4& axis, const hkAabb& aabb, hkSimdReal& prjOut);

			/// Compoute the widths span of an AABB wrt. a plane.
		static HK_FORCE_INLINE void		computeAabbPlaneSpan(const hkVector4& plane, const hkAabb& aabb, hkSimdReal& minOut, hkSimdReal& maxOut);

	public:

		//
		// Continuous AABB calculations
		//

		struct OffsetAabbInput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkAabbUtil::OffsetAabbInput );

			hkPadSpu<const hkMotionState*> m_motionState;

			// intermediate transforms
			hkTransform m_transforms[4];
			hkPadSpu<int> m_numTransforms;

			// start-end transforms
			hkTransform m_startTransform;
			hkTransform m_endTransformInv;
		};

			/// Sweep a discrete AABB and convert it into a 'continuous' swept AABB.
		HK_FORCE_INLINE static void HK_CALL sweepAabb(const hkMotionState* motionstate, hkReal tolerance, const hkAabb& aabbIn, hkAabb& aabbOut);

			/// Calculate OffsetAabbInput.
		static void HK_CALL initOffsetAabbInput(const hkMotionState* motionstate, OffsetAabbInput& input);

			/// Sweep a discrete AABB which has a huge offset from the mass center and convert it into a 'continuous' swept AABB.
		static void HK_CALL sweepOffsetAabb(const OffsetAabbInput& input, const hkAabb& aabbIn, hkAabb& aabbOut);


};

#include <Common/Base/Types/Geometry/Aabb/hkAabbUtil.inl>

#endif // HK_COLLIDE2_AABB_UTIL_H

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
