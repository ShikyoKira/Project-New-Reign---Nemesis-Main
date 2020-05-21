/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_TREE_WORLD_MANAGER_H
#define HKP_TREE_WORLD_MANAGER_H

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>

class hkJobThreadPool;
class hkJobQueue;

struct hkpWorldRayCastInput;
struct hkpWorldRayCastOutput;
class hkpRayHitCollector;
struct hkpLinearCastInput;
class hkpCdPointCollector;
class hkpCollisionFilter;

class hkKdTree;
template<typename BOUNDING_VOLUME, typename IDXTYPE>
class hkAabbTree;

class hkpTreeWorldManager : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		/// When an object (either hkpEntity or hkpPhantom) is removed from the world, there are two options for what to do to the tree:
		///     -# Mark the tree is dirty, so that it must be rebuilt before any new raycasting is done.
		///        This doesn't take any time, but means that you may need to rebuild the tree later
		///     -# Scan the tree for the entry corresponding to the object, and invalidate that entry
		///        This can take a bit of time ( it requires linear search through an array), but means the tree doesn't need to be rebuilt.
		enum ObjectRemovedBehavior
		{
			MARK_TREE_AS_DIRTY,
			REMOVE_OBJECT_FROM_TREE
		};
		hkpTreeWorldManager(){m_inactiveTree = HK_NULL; m_kdTree = HK_NULL;}
		virtual void updateFromWorld(hkJobQueue* jobQueue = HK_NULL, hkJobThreadPool* jobThreadPool = HK_NULL) = 0;

		virtual bool isUpToDate() const = 0;
		virtual void setUpToDate(bool upToDate) = 0;

		/// Register method to create aabbTree s.th. code can be stripped when not in use
		typedef hkpTreeWorldManager* (HK_CALL *CreateAabbTreeWorldManagerFunction)( hkpWorld*, hkpWorldCinfo::TreeUpdateType, hkReal );
		static CreateAabbTreeWorldManagerFunction createAabbTreeWorldManager;

		/// Register method to create kdTree s.th. code can be stripped when not in use
		typedef hkpTreeWorldManager* (HK_CALL *CreateKdTreeWorldManagerFunction)( hkpWorld*, hkpTreeWorldManager::ObjectRemovedBehavior );
		static CreateKdTreeWorldManagerFunction createKdTreeWorldManager;

		/// Register method to query AABB in AABB tree s.th. code can be stripped when not in use
		typedef int (HK_CALL *AabbTreeQueryUtilsQueryAabbFunction)( const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*, const hkAabb&, hkArray<void*>& );
		static AabbTreeQueryUtilsQueryAabbFunction aabbTreeQueryUtilsQueryAabb;

		/// Register method to query aabb in KdTree
		typedef void (HK_CALL *KdTreeAabbQueryUtilsQueryAabbFunction)( const hkKdTree*, const hkAabb&, hkArray<hkPrimitiveId>::Temp& );
		static KdTreeAabbQueryUtilsQueryAabbFunction kdTreeAabbQueryUtilsQueryAabb;

		//
		// Raycasting and linear casting interface.
		// The methods are analogical to those in hkpWorld.
		//

		/// Cast a ray into the world and get the closest hit.
		virtual void castRay(const hkpWorldRayCastInput& input, const hkpCollisionFilter* filter, hkpWorldRayCastOutput& output ) const = 0;

		/// Cast a ray into the world and do a callback for every hit.
		virtual void castRay(const hkpWorldRayCastInput& input, const hkpCollisionFilter* filter, hkpRayHitCollector& collector ) const = 0;

		/// Cast a shape within the world.
		/// The castCollector collects all potential hits.
		virtual void linearCast(const hkpCollidable* collA, 
			const struct hkpLinearCastInput& input, const class hkpCollidableCollidableFilter* filter,
			const struct hkpCollisionInput& collInput, struct hkpCollisionAgentConfig* config,
			class hkpCdPointCollector& castCollector, class hkpCdPointCollector* startPointCollector ) = 0;

		/// Calculate required working buffer size.
		/// The buffer have to be passed by set WorkingBuffer
		virtual int calcWorkingBufferSize(int maxObjects){return 0;}

		/// If aabbTreeWorldManager is used, this buffer has to be set before tree build.
		virtual void setWorkingBuffer(void* ptr){}

		/// Get KdTree.
		const hkKdTree* getKdTree() const{return m_kdTree;}

		/// Get AabbTree, which is used for inactive objects if aabbTreeWorldManager is used. Otherwise returns HK_NULL.
		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>* getAabbTree() const{return m_inactiveTree;}

		enum Type
		{
			MANAGER_KDTREE,
			MANAGER_AABBTREE,
		};

		Type m_type;

//		hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16>* m_inactiveTree;
		hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>* m_inactiveTree;
		hkKdTree* m_kdTree;

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
