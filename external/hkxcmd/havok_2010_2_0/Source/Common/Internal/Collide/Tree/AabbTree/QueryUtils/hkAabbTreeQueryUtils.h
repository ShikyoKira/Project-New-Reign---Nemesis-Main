/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_AABBTREE_AABBQUERY_UTIL_H
#define HK_AABBTREE_AABBQUERY_UTIL_H

#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTree.h>


typedef void** voidWriteIterator;

class hkpCollidable;

class hkAabbTreeQueryUtilsData
{
	public:
		static hkVector4Comparison::Mask s_numRaysToMask[5];

		static hkAabbTreeData::RayBundle HK_CALL bundleRays(const hkVector4& from0, const hkVector4& to0, 
			const hkVector4& from1, const hkVector4& to1,
			const hkVector4& from2, const hkVector4& to2,
			const hkVector4& from3, const hkVector4& to3,
			hkVector4Comparison::Mask mask = hkVector4Comparison::MASK_XYZW);
};

template<typename BOUNDING_VOLUME, typename hkIdxType = hkUint16>
class hkAabbTreeQueryUtils
{
	public:
		// TYPES

		typedef class hkAabbTree<BOUNDING_VOLUME, hkIdxType> hkDynamicAabbTree;
		typedef struct hkAabbTree<BOUNDING_VOLUME, hkIdxType>::Node TreeNode;

		struct NodePair
		{
			NodePair(){}
			NodePair(TreeNode* a, TreeNode* b){m_a = a; m_b = b;}

			TreeNode* m_a;
			TreeNode* m_b;
		};

		struct IdxPair
		{
			hkIdxType m_a;
			hkIdxType m_b;
			HK_FORCE_INLINE IdxPair() {}
			HK_FORCE_INLINE IdxPair(hkIdxType a, hkIdxType b) : m_a(a), m_b(b) {}
		};


		// COLLECTOR INTERFACES
		
		struct TreeTreeCollector
		{
			virtual ~TreeTreeCollector(){}
			virtual bool addHit(TreeNode* leafA, TreeNode* leafB) = 0;
		};

		struct Collector
		{
			virtual ~Collector(){}
			// returns the hit fraction. If 0 is returned, stop traversing
			// Traverse order of tree is not always from close to far. So even if it hits first time, there is a possibility that it will find much closer hit later
			virtual hkReal addLeaf( const TreeNode* leaf) = 0;
		};

		struct ClosestPointCollector : public Collector
		{
			ClosestPointCollector(const hkVector4& point, hkReal initDistanceSquared) : m_point( point ), m_distanceSquared(initDistanceSquared){}
			virtual ~ClosestPointCollector(){}
			// returns distance^2
			virtual hkReal addLeaf( const TreeNode* leaf) = 0;

			hkVector4 m_point;
			hkReal m_distanceSquared;
		};


		// COLLECTOR IMPLEMENTATIONS

		struct TreeTreeNodePairCollector : public TreeTreeCollector
		{
			TreeTreeNodePairCollector(hkArray<NodePair>* resultBuffer) : m_resultBuffer( resultBuffer ){}
			virtual ~TreeTreeNodePairCollector(){}
			virtual bool addHit(TreeNode* leafA, TreeNode* leafB){ m_resultBuffer->pushBack( NodePair( leafA, leafB )); return false;}

			hkArray<NodePair>* m_resultBuffer;
		};

		struct AllHitsCollector: public Collector
		{
			AllHitsCollector(){}
			virtual ~AllHitsCollector(){}
			virtual hkReal addLeaf( const TreeNode* leaf){ m_hits.pushBack( leaf->template getLeafData<const void*>() ); return 1.0f;}

			hkInplaceArray<const void*, 128> m_hits;
		};

		struct AllHitsNodeCollector: public Collector
		{
			AllHitsNodeCollector(hkArray<TreeNode*>* buffer): m_hits(buffer){}
			virtual ~AllHitsNodeCollector(){}
			virtual hkReal addLeaf( const TreeNode* leaf){ m_hits->pushBack( const_cast<TreeNode*>(leaf) ); return 1.0f;}

			hkArray<TreeNode*>* m_hits;
		};


		// QUERY FUNCTIONS

		/// Ray casting
		static void HK_CALL castRay( const hkDynamicAabbTree* tree, const hkVector4& rayFrom, const hkVector4& rayTo, Collector& collector, hkReal initialEarlyOutFraction = 1.0f );

		static void HK_CALL castRayBundle( const hkDynamicAabbTree* tree, const TreeNode* nodePpu, const TreeNode* escapeNode, 
			const hkAabbTreeData::RayBundle& rayBundle, Collector* collectors[4], hkReal earlyHitFractions[4]);

		static void HK_CALL castRayBundleSpu( const hkDynamicAabbTree* tree, const TreeNode* nodeSpu, const TreeNode* escapeNode, 
			const hkAabbTreeData::RayBundle& rayBundle, Collector* collectors[4], hkReal earlyHitFractions[4]);

		/// AABB query
		static int HK_CALL queryAabb( const hkDynamicAabbTree* tree, const hkAabb& aabb, Collector* collector );

		static int HK_CALL queryAabb( const hkDynamicAabbTree* tree, const hkAabb& aabb, voidWriteIterator* hitArray, int hitCapacity );

		static int HK_CALL queryAabb( const hkDynamicAabbTree* tree, const BOUNDING_VOLUME& aabb, Collector* collector );

		static int HK_CALL queryAabb( const hkDynamicAabbTree* tree, const BOUNDING_VOLUME& aabb, voidWriteIterator* hitArray, int hitCapacity );

		static int HK_CALL queryAabb( const hkDynamicAabbTree* tree, const hkAabb& aabb, hkArray<void*>& hitArray );

		/// AABB cast
		static int HK_CALL HK_AABB_TREE_FUNCTION( castAabb )( const hkDynamicAabbTree* tree, const hkAabb& queryVolume, typename hkAabbTreeData::CastVolumeInput& input, 
			Collector* collector, hkReal aabbCheckTolerance = 0.0f, void** hitArray = HK_NULL, int hitCapacity = 0 );


		/// Tree Tree collision
		static int HK_CALL queryTreeCollision( const hkDynamicAabbTree* treeA, const hkDynamicAabbTree* treeB, TreeTreeCollector* collector );

		static int HK_CALL queryTreeCollision( const hkDynamicAabbTree* treeA, const hkDynamicAabbTree* treeB, hkArray<IdxPair>* pairsOut );

		/// Closest point query
		static void HK_CALL queryClosestPoint( const hkDynamicAabbTree* tree, const hkVector4& point, Collector* collector );


	private:
		static int HK_CALL HK_AABB_TREE_FUNCTION( queryAabbInternal )( const hkDynamicAabbTree* tree, const BOUNDING_VOLUME& aabb, 
			Collector* collector, hkArray<void*>* ptrArray, voidWriteIterator* hitArray, int hitCapacity );
};

#endif

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
