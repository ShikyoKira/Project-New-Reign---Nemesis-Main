/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLISION_INPUT_H
#define HK_COLLIDE2_COLLISION_INPUT_H

class hkpCollisionDispatcher;
class hkpCollisionFilter;
class hkpConvexListFilter;
struct hkpCollisionAgentConfig;
class hkpSpuCollisionQueryDispatcher;


	/// The basic structure needed for all hkpCollisionAgent queries.
struct hkpCollisionInput
{

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpCollisionInput);

	public:

			/// Get the collision tolerance
		hkReal getTolerance() const { return m_tolerance; }

			/// Set the collision tolerance
		void setTolerance(hkReal t) { m_tolerance = t; }

	public:

		hkpCollisionInput() : m_forceAcceptContactPoints(false) {}


			/// A pointer to the collision dispatcher, needed if more agents need to be created to complete the query
		hkPadSpu<hkpCollisionDispatcher*> m_dispatcher;


			/// Used to weld normals obtained from getClosestPoints functions
		hkPadSpu<hkBool32> m_weldClosestPoints;

			/// Internal -- if we are linear casting, don't allow welding to reject contact points
		hkPadSpu<hkBool32> m_forceAcceptContactPoints;

			/// The collision tolerance. Only points closer than this tolerance will be reported.
		hkPadSpu<hkReal> m_tolerance;

			/// The filter used if any shape collections are queried. This filter will be called before any child shapes
			/// are considered for the query.
		hkPadSpu<const hkpCollisionFilter*> m_filter;

			/// A filter used to decide how to treat convex list shapes in collisions.
		hkPadSpu<const hkpConvexListFilter*> m_convexListFilter;

			/// Only used when you create persistent agents
		mutable hkPadSpu<hkBool32> m_createPredictiveAgents;

		struct Aabb32Info
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCollisionInput::Aabb32Info );

			hkVector4 m_bitOffsetLow;
			hkVector4 m_bitOffsetHigh;
			hkVector4 m_bitScale;
		};

		Aabb32Info m_aabb32Info;
};

#endif // HK_COLLIDE2_COLLISION_INPUT_H

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
