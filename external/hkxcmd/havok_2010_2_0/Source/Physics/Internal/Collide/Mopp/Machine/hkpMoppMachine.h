/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
//
//

#ifndef HK_COLLIDE2_MOPP_MACHINE_H
#define HK_COLLIDE2_MOPP_MACHINE_H

#include <Common/Base/hkBase.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Common/Base/Types/Color/hkColor.h>
#include <Physics/Internal/Collide/Mopp/Machine/hkp26Dop.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>

struct hkpMoppPlanesQueryInput
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MOPP, hkpMoppPlanesQueryInput );

		enum { HK_MAX_NUM_PLANES = 32 };
	
			/// The number of planes, a maximum of HK_MAX_NUM_PLANES
		int m_numPlanes;

			/// The planes. The distance to the plane is calculated using:
			/// \code dist = m_planes[x].dot3( position ) + m_planes[x](3) \endcode
			/// The planes are pointing away from the viewing frustum (they define a convex object)
			/// so they have the same direction as the planes in the hkpConvexVerticesShape
		const hkVector4 *m_planes;
};

/// Output object for hkpMoppKDopGeometriesVirtualMachine. One of these is
/// created for each KDOP that is found, according to the hkpMoppKDopQuery.
struct hkpMoppInfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MOPP, hkpMoppInfo );

	/// The 26-plane bounded shape at each node of the MOPP.
	hkp26Dop m_dop;

	/// Shapekey of a terminal, if m_isTerminal is true.
	hkpShapeKey m_shapeKey;

	/// The level this hkpMoppInfo represents.
	hkInt8 m_level;

	/// Specifies whether this info represents a terminal. If it doesn't,
	/// it represents an intermediate.
	hkBool m_isTerminal;
};


/// Query object for hkpMoppKDopGeometriesVirtualMachine
struct hkpMoppKDopQuery
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MOPP, hkpMoppKDopQuery );

	/// Set true to exit after the first hit
	hkBool	m_earlyExit;

	/// Depth of display KDOPs to display (-1 to just display nodes, 0 to display all)
	int	m_kdopDepth;

	/// Set true to only save KDOPs that lead to the specified ID.
	hkBool	m_useSpecifiedID;
	unsigned int m_specifiedId;

	hkpMoppKDopQuery()
	{
		m_earlyExit = false;

		m_kdopDepth = -1;
		m_useSpecifiedID = false;
		m_specifiedId = 0;
	}
};


class hkpMoppModifier;

extern "C"
{
		/// Returns true if the OBB hits a MOPP leave node
	int HK_CALL hkMoppEarlyExitObbVirtualMachine_queryObb(const hkpMoppCode* code, const hkTransform& BvToWorld, const hkVector4& extent, const float& radius);

		/// Return all the keys in a MOPP. Note: the order of keys in a MOPP is consistent.
		/// Please read hkpMoppCompilerInput
	void HK_CALL hkMoppFindAllVirtualMachine_getAllKeys(   const hkpMoppCode* code, hkArray<hkpShapeKey>* primitives_out);

		/// Returns at least keys in a MOPP which overlap with the given OBB.
	void HK_CALL hkMoppObbVirtualMachine_queryObb(const hkpMoppCode* code, const hkTransform& BvToWorld, const hkVector4& halfExtent, const float radius, hkArray<hkpShapeKey>* primitives_out);

		/// Returns at least keys in a MOPP which overlap with the given AABB.
	void HK_CALL hkMoppObbVirtualMachine_queryAabb(const hkpMoppCode* code, const hkAabb& aabb, hkArray<hkpShapeKey>* primitives_out);

			/// Returns at least keys in a MOPP which overlap with the given sphere
	void HK_CALL hkMoppSphereVirtualMachine_querySphere(const hkpMoppCode* code, const hkSphere &sphere, hkArray<hkpShapeKey>* primitives_out);

		/// Query optimized for frustum checks. It reports all hits intersecting the planes (partialHitsOut).
		/// and all hits completely inside the convex object defined by the planes (fullyIncludedHitsOut).
	void HK_CALL hkMoppUsingFloatAabbVirtualMachine_queryPlanes( const hkpMoppCode* code, const hkpMoppPlanesQueryInput &query, hkArray<hkpShapeKey>* partialHitsOut, hkArray<hkpShapeKey>* fullyIncludedHitsOut);

		/// Same as hkMoppUsingFloatAabbVirtualMachine_queryPlanes but instead of returning all hits which are fully included
		/// it returns ranges of hits in fullyIncludedHitsOut.
		/// You can use hkMoppFindAllVirtualMachine_getAllKeys to find about the ordering of keys and
		/// than either reorder your input or create two mapping arrays.
		/// Example:
		///
		/// If you call hkMoppFindAllVirtualMachine_getAllKeys you might get the following hits:
		/// 1, 3, 2, 7, 5, 4, 8, 6
		///
		/// If hkMoppUsingFloatAabbVirtualMachine_queryPlanesOptimized returns the range [3,4], it means all hits between 3 and 4 inclusive,
		/// which is 3,2,7,5,4
	void HK_CALL hkMoppUsingFloatAabbVirtualMachine_queryPlanesOptimized( const hkpMoppCode* code, const hkpMoppPlanesQueryInput &query, hkArray<hkpShapeKey>* partialHitsOut, hkArray<hkpShapeKey>* fullyIncludedHitsOut);


		/// Same as hkMoppUsingFloatAabbVirtualMachine_queryPlanes, but using a sphere instead of a convex object
	void HK_CALL hkMoppUsingFloatAabbVirtualMachine_querySphere( const hkpMoppCode* code, const hkSphere &query, hkArray<hkpShapeKey>* partialHitsOut, hkArray<hkpShapeKey>* fullyIncludedHitsOut);

		/// Same as hkMoppUsingFloatAabbVirtualMachine_queryPlanesOptimized, but using a sphere instead of a convex object
	void HK_CALL hkMoppUsingFloatAabbVirtualMachine_querySphereOptimized( const hkpMoppCode* code, const hkSphere &query, hkArray<hkpShapeKey>* partialHitsOut, hkArray<hkpShapeKey>* fullyIncludedHitsOut);

			/// Queries the MOPP and calls shouldTerminalBeRemoved on at least all nodes which overlap the input AABBs.
			/// modifierOut returns whether you want to remove a node or not.
			/// For every subtree which only has 'to-be-removed' nodes the address ADDR of the root node of this
			/// subtree is calculated and m_modifierOut->addTerminalRemoveInfo(ADDR) called. If you set the moppcode at
			/// this relative address to zero, you will effectively disable this subtree.
			///
			/// In short:
			///   - call queryAabb with an AABB containing all your nodes you want to remove
			///   - for every node you want to remove, return true in you implementation of hkpMoppModifier::shouldTerminalBeRemoved
			///   - remember all MOPP code addressed in hkpMoppModifier::addTerminalRemoveInfo
			///   - change the MOPP at all those addresses to zero to apply the MOPP changes.
			///   - optional: if you remember all the places you have changed, than you can undo your changes
	void HK_CALL hkMoppModifyVirtualMachine_queryAabb( const hkpMoppCode* code, const hkAabb& aabb, hkpMoppModifier* modifierOut );

		/// 
	void HK_CALL hkMoppKDopGeometriesVirtualMachine_query( const hkpMoppCode* code, const hkpMoppKDopQuery &query, hkpMoppInfo* kDopGeometries );
}



#endif // HK_COLLIDE2_MOPP_MACHINE_H

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
