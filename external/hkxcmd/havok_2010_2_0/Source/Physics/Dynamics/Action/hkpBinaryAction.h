/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_BINARY_ACTION_H
#define HK_DYNAMICS2_BINARY_ACTION_H

#include <Physics/Dynamics/Action/hkpAction.h>
#include <Common/Base/Reflection/Attributes/hkAttributes.h>
class hkpRigidBody;

extern const hkClass hkpBinaryActionClass;

	/// You can use this as a base class for hkActions that operate on a pair of
	/// hkRigidBodies. In addition to the hkpAction interface, this class provides
	/// some useful basic functionality for actions, such as a callback that removes
	/// the action from the simulation if either of its hkRigidBodies is removed.
class hkpBinaryAction : public hkpAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Constructor creates a new hkpBinaryAction that operates on the specified entities.
		hkpBinaryAction( hkpEntity* entityA, hkpEntity* entityB, hkUlong userData = 0 );

			/// hkpAction interface implementation.
		virtual void getEntities( hkArray<hkpEntity*>& entitiesOut );

			/// Removes the action from the hkpWorld if one of its hkRigidBodies is removed.
		virtual void entityRemovedCallback(hkpEntity* entity);

			/// Sets m_bodyA, adds a reference to it and adds the action as a listener.
			/// NB: Only intended to be called pre-simulation i.e., before hkpBinaryAction
			/// is added to an hkpWorld.
		void setEntityA( hkpEntity* entityA);

			/// Gets m_bodyA.
		inline hkpEntity* getEntityA();

			/// Sets m_bodyB, adds a reference to it and adds the action as a listener.
			/// NB: Only intended to be called pre-simulation i.e., before hkpBinaryAction
			/// is added to an hkpWorld.
		void setEntityB( hkpEntity* entityB);

			/// Gets m_bodyB.
		inline hkpEntity* getEntityB();

			/// The applyAction() method does the actual work of the action, and is called at every simulation step.
		virtual void applyAction( const hkStepInfo& stepInfo ) = 0;

		virtual const hkClass* getClassType( ) const { return &hkpBinaryActionClass; }

	protected:
			/// Destructor.
		virtual ~hkpBinaryAction();

			/// The first entity.
		hkpEntity* m_entityA;//+hk.Ui(group="Attach to", visible=false) +hk.Link("PARENT_NAME")

			/// The second entity.
		hkpEntity* m_entityB;//+hk.Ui(visible=true, label="Other Body", endGroup=true) +hk.Link("MESH")

		void _referenceBodies(); 

	public:

		hkpBinaryAction( class hkFinishLoadedObjectFlag flag ) : hkpAction(flag) {}
};

#include <Physics/Dynamics/Action/hkpBinaryAction.inl>


#endif // HK_DYNAMICS2_BINARY_ACTION_H

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
