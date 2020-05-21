/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_WORLD_OPERATION_QUEUE_H
#define HK_DYNAMICS2_WORLD_OPERATION_QUEUE_H

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Motion/hkpMotion.h>

#include <Physics/Dynamics/World/Util/hkpBodyOperation.h>

#include <Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h>

class hkAabb;
class hkpRigidBody;
class hkpWorldObject;
class hkpConstraintInstance;
class hkpShape;
class hkpConstraintCollisionFilter;

namespace hkWorldOperation
{
	enum Type
	{
		OPERATION_ID_ANY     = 0,
		OPERATION_ID_INVALID = 0,

		ENTITY_ADD,
		ENTITY_REMOVE,

		ENTITY_UPDATE_BROAD_PHASE,

		RIGIDBODY_SET_MOTION_TYPE,

		WORLD_OBJECT_SET_SHAPE,
		WORLD_OBJECT_UPDATE_SHAPE,

		ENTITY_BATCH_ADD,
		ENTITY_BATCH_REMOVE,

		CONSTRAINT_ADD,
		CONSTRAINT_REMOVE,

		ACTION_ADD,
		ACTION_REMOVE,

		ISLAND_MERGE,       

		PHANTOM_ADD,
		PHANTOM_REMOVE,
		PHANTOM_BATCH_ADD,
		PHANTOM_BATCH_REMOVE,

		PHANTOM_UPDATE_BROAD_PHASE,

		UPDATE_FILTER_ENTITY,
		UPDATE_FILTER_ENTITY_PAIR,
		UPDATE_FILTER_PHANTOM,
		UPDATE_FILTER_WORLD,

		UPDATE_MOVED_BODY_INFO,
	
		ENTITY_BATCH_REINTEGRATE_AND_RECOLLIDE,

		RIGIDBODY_SET_POSITION_AND_ROTATION,
		RIGIDBODY_SET_LINEAR_VELOCITY,
		RIGIDBODY_SET_ANGULAR_VELOCITY,
		RIGIDBODY_APPLY_LINEAR_IMPULSE,
		RIGIDBODY_APPLY_POINT_IMPULSE,
		RIGIDBODY_APPLY_ANGULAR_IMPULSE,

		WORLD_OBJECT_ADD_REFERENCE,
		WORLD_OBJECT_REMOVE_REFERENCE,

		ACTIVATE_REGION,

		ACTIVATE_ENTITY,
		DEACTIVATE_ENTITY,

		CONSTRAINT_FILTER_BROKEN_EVENT,

		USER_CALLBACK

	};

	class UserCallback : public hkReferencedObject
	{
		public:
			virtual void worldOperationUserCallback(hkUlong userData) = 0;
	};


		// !! This is defined in hkpWorldObject.h
//	enum Result
//	{
//		DONE,
//		POSTPONED
//	};

	struct BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, BaseOperation );

		BaseOperation(Type type) : m_type(type) {}
		hkEnum<Type, hkUint8> m_type;

#		if defined (HK_ENABLE_DETERMINISM_CHECKS)
		hkCheckDeterminismUtil::Fuid m_jobFuid;
#		endif

	private:
		BaseOperation();
	};

	struct AddEntity : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, AddEntity );

		AddEntity() : BaseOperation(ENTITY_ADD) {}
		hkpEntity* m_entity;
		hkpEntityActivation m_activation;
	};

	struct RemoveEntity : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, RemoveEntity );

		RemoveEntity() : BaseOperation(ENTITY_REMOVE) {}
		hkpEntity* m_entity;
	};

	struct SetRigidBodyMotionType : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, SetRigidBodyMotionType );

		SetRigidBodyMotionType() : BaseOperation(RIGIDBODY_SET_MOTION_TYPE) {}
		hkpRigidBody* m_rigidBody;
		hkEnum<hkpMotion::MotionType, hkUint8>       m_motionType;
		hkEnum<hkpEntityActivation, hkUint8>         m_activation;
		hkEnum<hkpUpdateCollisionFilterOnEntityMode, hkUint8> m_collisionFilterUpdateMode;
		hkEnum<hkpUpdateCollectionFilterMode, hkUint8>	m_updateShapeCollections;
	};

	struct SetWorldObjectShape : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, SetWorldObjectShape );

		SetWorldObjectShape() : BaseOperation(WORLD_OBJECT_SET_SHAPE) {}
		hkpWorldObject* m_worldObject;
		const hkpShape* m_shape;
	};

	struct UpdateWorldObjectShape : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UpdateWorldObjectShape );

		UpdateWorldObjectShape()
		:	BaseOperation(WORLD_OBJECT_UPDATE_SHAPE)
		,	m_shapeModifier(HK_NULL)
		{}

		hkpWorldObject* m_worldObject;
		hkpShapeModifier* m_shapeModifier;
	};

	struct AddEntityBatch : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, AddEntityBatch );

		AddEntityBatch() : BaseOperation(ENTITY_BATCH_ADD) {}
		hkpEntity** m_entities;
		hkObjectIndex m_numEntities;
		hkEnum<hkpEntityActivation, hkUint8> m_activation;
	};

	struct RemoveEntityBatch : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, RemoveEntityBatch );

		RemoveEntityBatch() : BaseOperation(ENTITY_BATCH_REMOVE) {}
		hkpEntity** m_entities;
		hkObjectIndex m_numEntities;
	};

	struct AddConstraint : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, AddConstraint );

		AddConstraint() : BaseOperation(CONSTRAINT_ADD) {}
		hkpConstraintInstance* m_constraint;
	};

	struct RemoveConstraint : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, RemoveConstraint );

		RemoveConstraint() : BaseOperation(CONSTRAINT_REMOVE) {}
		hkpConstraintInstance* m_constraint;
	};

	struct AddAction : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, AddAction );

		AddAction() : BaseOperation(ACTION_ADD) {}
		hkpAction* m_action;
	};

	struct RemoveAction : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, RemoveAction );

		RemoveAction() : BaseOperation(ACTION_REMOVE) {}
		hkpAction* m_action;
	};


	struct MergeIslands : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, MergeIslands );

		MergeIslands() : BaseOperation(ISLAND_MERGE) {}
		hkpEntity* m_entities[2];
	};


	struct AddPhantom : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, AddPhantom );

		AddPhantom() : BaseOperation(PHANTOM_ADD) {}
		hkpPhantom* m_phantom;
	};

	struct RemovePhantom : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, RemovePhantom );

		RemovePhantom() : BaseOperation(PHANTOM_REMOVE) {}
		hkpPhantom* m_phantom;
	};

	struct AddPhantomBatch : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, AddPhantomBatch );

		AddPhantomBatch() : BaseOperation(PHANTOM_BATCH_ADD) {}
		hkpPhantom** m_phantoms;
		hkObjectIndex m_numPhantoms;
	};

	struct RemovePhantomBatch : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, RemovePhantomBatch );

		RemovePhantomBatch() : BaseOperation(PHANTOM_BATCH_REMOVE) {}
		hkpPhantom** m_phantoms;
		hkObjectIndex m_numPhantoms;
	};


	struct UpdateEntityBP : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UpdateEntityBP );

		UpdateEntityBP() : BaseOperation(ENTITY_UPDATE_BROAD_PHASE) {}
		hkpEntity* m_entity;
	};

	struct UpdatePhantomBP : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UpdatePhantomBP );

		UpdatePhantomBP() : BaseOperation(PHANTOM_UPDATE_BROAD_PHASE) {}
		hkpPhantom* m_phantom;
		hkAabb* m_aabb;
	};


	struct UpdateFilterOnEntity : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UpdateFilterOnEntity );

		UpdateFilterOnEntity() : BaseOperation(UPDATE_FILTER_ENTITY) {}
		hkpEntity* m_entity;
		hkEnum<hkpUpdateCollisionFilterOnEntityMode, hkUint8> m_collisionFilterUpdateMode;
		hkEnum<hkpUpdateCollectionFilterMode, hkUint8> 	m_updateShapeCollections;
	};

	struct UpdateFilterOnEntityPair : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UpdateFilterOnEntityPair );

		UpdateFilterOnEntityPair() : BaseOperation(UPDATE_FILTER_ENTITY_PAIR) {}
		hkpEntity* m_entityA;
		hkpEntity* m_entityB;
	};

	struct UpdateFilterOnPhantom : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UpdateFilterOnPhantom );

		UpdateFilterOnPhantom() : BaseOperation(UPDATE_FILTER_PHANTOM) {}
		hkpPhantom* m_phantom;
		hkEnum<hkpUpdateCollectionFilterMode, hkUint8> 	m_updateShapeCollections;
	};
	struct UpdateFilterOnWorld : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UpdateFilterOnWorld );

		UpdateFilterOnWorld() : BaseOperation(UPDATE_FILTER_WORLD) {}
		hkEnum<hkpUpdateCollisionFilterOnWorldMode, hkUint8>      m_collisionFilterUpdateMode;
		hkEnum<hkpUpdateCollectionFilterMode, hkUint8> 	m_updateShapeCollections;
	};

	struct ReintegrateAndRecollideEntityBatch : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, ReintegrateAndRecollideEntityBatch );

		ReintegrateAndRecollideEntityBatch() : BaseOperation(ENTITY_BATCH_REINTEGRATE_AND_RECOLLIDE) {}
		hkpEntity**	  m_entities;
		hkObjectIndex m_numEntities;
		hkUint8       m_mode;
		hkEnum<hkpEntityActivation, hkUint8> m_activation;
	};

	struct UpdateMovedBodyInfo : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UpdateMovedBodyInfo );

		UpdateMovedBodyInfo() : BaseOperation(UPDATE_MOVED_BODY_INFO) {}
		hkpEntity* m_entity;
	};


	struct SetRigidBodyPositionAndRotation : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, SetRigidBodyPositionAndRotation );

		SetRigidBodyPositionAndRotation() : BaseOperation(RIGIDBODY_SET_POSITION_AND_ROTATION) {}
		hkpRigidBody* m_rigidBody;
		hkVector4* m_positionAndRotation; // performs and external allocation
	};
	struct SetRigidBodyLinearVelocity : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, SetRigidBodyLinearVelocity );

		SetRigidBodyLinearVelocity() : BaseOperation(RIGIDBODY_SET_LINEAR_VELOCITY) {}
		hkpRigidBody* m_rigidBody;
		hkReal m_linearVelocity[3];
	};
	struct SetRigidBodyAngularVelocity : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, SetRigidBodyAngularVelocity );

		SetRigidBodyAngularVelocity() : BaseOperation(RIGIDBODY_SET_ANGULAR_VELOCITY) {}
		hkpRigidBody* m_rigidBody;
		hkReal m_angularVelocity[3];
	};
	struct ApplyRigidBodyLinearImpulse : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, ApplyRigidBodyLinearImpulse );

		ApplyRigidBodyLinearImpulse() : BaseOperation(RIGIDBODY_APPLY_LINEAR_IMPULSE) {}
		hkpRigidBody* m_rigidBody;
		hkReal m_linearImpulse[3];
	};
	struct ApplyRigidBodyPointImpulse : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, ApplyRigidBodyPointImpulse );

		ApplyRigidBodyPointImpulse() : BaseOperation(RIGIDBODY_APPLY_POINT_IMPULSE) {}
		hkpRigidBody* m_rigidBody;
		hkVector4* m_pointAndImpulse; // performs and external allocation
	};
	struct ApplyRigidBodyAngularImpulse : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, ApplyRigidBodyAngularImpulse );

		ApplyRigidBodyAngularImpulse() : BaseOperation(RIGIDBODY_APPLY_ANGULAR_IMPULSE) {}
		hkpRigidBody* m_rigidBody;
		hkReal m_angularImpulse[3];
	};

		// This is a 'special' operation which actually is not postponed, but merely uses operationQueue's critical section to guarantee inter-thread safety.
	struct AddReference : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, AddReference );

		AddReference() : BaseOperation(WORLD_OBJECT_ADD_REFERENCE) {}
		hkpWorldObject* m_worldObject;
	};

		// This is a 'special' operation which actually is not postponed, but merely uses operationQueue's critical section to guarantee inter-thread safety.
	struct RemoveReference : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, RemoveReference );

		RemoveReference() : BaseOperation(WORLD_OBJECT_REMOVE_REFERENCE) {}
		hkpWorldObject* m_worldObject;
	};

	struct ActivateRegion : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, ActivateRegion );

		ActivateRegion() : BaseOperation(ACTIVATE_REGION) {}
		hkAabb* m_aabb;
	};

	struct ActivateEntity : public BaseOperation 
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, ActivateEntity );

		ActivateEntity() : BaseOperation(ACTIVATE_ENTITY) {}
		hkpEntity* m_entity;
	};

	struct DeactivateEntity : public BaseOperation 
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, DeactivateEntity );

		DeactivateEntity() : BaseOperation(DEACTIVATE_ENTITY) {}
		hkpEntity* m_entity;
	};
	
	struct ConstraintCollisionFilterConstraintBroken: public BaseOperation
	{
		ConstraintCollisionFilterConstraintBroken() : BaseOperation( CONSTRAINT_FILTER_BROKEN_EVENT ){}
		hkpConstraintCollisionFilter* m_filter;
		hkpConstraintInstance* m_constraintInstance;
		hkBool				   m_constraintBroken;
	};



	struct UserCallbackOperation : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, UserCallbackOperation );

		UserCallbackOperation() : BaseOperation(USER_CALLBACK) {}
		UserCallback* m_userCallback;
		hkUlong m_userData; // +default(0)
	};

	struct BiggestOperation : public BaseOperation
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, BiggestOperation );

		BiggestOperation() : BaseOperation(OPERATION_ID_INVALID) {}
		hkUlong dummy[3 + 1];
	};
}

struct hkpBodyOperationEntry
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpBodyOperationEntry);
		hkpEntity*                       m_entity;
		hkpBodyOperation*                m_operation;
		int                              m_priority;
		hkpBodyOperation::ExecutionState m_hint;
};

class hkpWorldOperationQueue
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_WORLD, hkpWorldOperationQueue ); 

		hkpWorldOperationQueue(hkpWorld* world);

		~hkpWorldOperationQueue();

		void queueOperation(const hkWorldOperation::BaseOperation& operation);

						void executeAllPending();

		HK_FORCE_INLINE void purgeAllPending() { m_pending.clear(); }

		void queueBodyOperation(hkpEntity* breakingBody, hkpBodyOperation* operation, int priority, hkpBodyOperation::ExecutionState hint);

		void executeAllPendingBodyOperations();

	public:
		hkArray<hkWorldOperation::BiggestOperation> m_pending;
		hkpWorld* m_world;

		hkArray<hkWorldOperation::BiggestOperation> m_islandMerges;

		hkArray<hkpBodyOperationEntry> m_pendingBodyOperations;

	private:

		hkpWorldOperationQueue(){}
};



struct hkpDebugInfoOnPendingOperationQueues
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_WORLD, hkpDebugInfoOnPendingOperationQueues ); 

	hkpDebugInfoOnPendingOperationQueues(hkArray<hkWorldOperation::BiggestOperation>* pending) : 
		m_pending(pending), 
		m_nextPendingOperationIndex(0),
		m_nextQueue(HK_NULL),
		m_prevQueue(HK_NULL)
	{
	}

	hkArray<hkWorldOperation::BiggestOperation>* m_pending;
	int m_nextPendingOperationIndex;
	hkpDebugInfoOnPendingOperationQueues* m_nextQueue;
	hkpDebugInfoOnPendingOperationQueues* m_prevQueue;

	// This struct is only used by the debug code.
	HK_ON_DEBUG 
	(
		static void HK_CALL init(hkpWorld* world);
		static void HK_CALL cleanup(hkpWorld* world);

		static void HK_CALL updateNextPendingOperationIndex(hkpWorld* world, int index);
		static void HK_CALL addNextPendingOperationQueue(hkpWorld* world, hkArray<hkWorldOperation::BiggestOperation>* tmpOldPending);
		static void HK_CALL removeLastPendingOperationQueue(hkpWorld* world, hkArray<hkWorldOperation::BiggestOperation>* tmpOldPending);
		static int  HK_CALL getNumPendingOperationQueues(hkpWorld* world);

		static hkpDebugInfoOnPendingOperationQueues*  HK_CALL getLastQueue(hkpDebugInfoOnPendingOperationQueues* queue)
		{
			while(queue->m_nextQueue)
			{
				queue = queue->m_nextQueue;
			}
			return queue;
		}

		// Searching through pending lists:
		//   gotoTheLastOne
		//   start from the nextPendingOperationIndex and process till end
		//     goto previousQueue and repeat
		//   while previousQueue != HK_NULL

		static const hkWorldOperation::BaseOperation* findFirstPendingIslandMerge(hkpWorld* world, hkpSimulationIsland* isleA, hkpSimulationIsland* isleB);

		static const hkBool areEmpty(hkpWorld* world);
	)
};

#include <Physics/Dynamics/World/Util/hkpWorldOperationQueue.inl>

#endif // HK_DYNAMICS2_WORLD_OPERATION_QUEUE_H

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
