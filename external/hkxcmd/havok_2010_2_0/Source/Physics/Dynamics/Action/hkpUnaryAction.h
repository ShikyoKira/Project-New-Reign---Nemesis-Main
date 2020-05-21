/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_UNARY_ACTION_H
#define HK_DYNAMICS2_UNARY_ACTION_H

#include <Physics/Dynamics/Action/hkpAction.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

extern const hkClass hkpUnaryActionClass;

	/// You can use this as a base class for hkpActions that operate on a single hkpEntity.
	/// In addition to the hkpAction interface, this class provides
	/// some useful basic functionality for actions, such as a callback that
	/// removes the action from the simulation if its hkpEntity is removed.
class hkpUnaryAction : public hkpAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();
		
			/// Constructor creates a hkpUnaryAction that operates on the specified hkpEntity.
		hkpUnaryAction(hkpEntity* entity = HK_NULL, hkUlong userData = 0);

			/// Deconstructor.
		~hkpUnaryAction();

			/// hkpAction interface implementation.
		virtual void getEntities( hkArray<hkpEntity*>& entitiesOut );

			/// Remove self from the hkpWorld when the hkpEntity is removed.
		virtual void entityRemovedCallback(hkpEntity* entity);

			/// Sets m_entity, adds a reference to it and adds the action as a listener.
			/// NB: Only intended to be called pre-simulation i.e., before hkpUnaryAction
			/// is added to an hkpWorld.
		void setEntity(hkpEntity* entity);

			/// Returns m_entity.
		inline hkpEntity* getEntity();

			/// Returns m_entity cast to hkpRigidBody.
		inline hkpRigidBody* getRigidBody();

			/// The applyAction() method does the actual work of the action, and is called at every simulation step.
		virtual void applyAction( const hkStepInfo& stepInfo ) = 0;

		virtual const hkClass* getClassType( ) const { return &hkpUnaryActionClass; }

	protected:

			/// The hkpEntity.
		hkpEntity* m_entity;	//+hk.Link("PARENT_NAME")

	public:

		hkpUnaryAction( class hkFinishLoadedObjectFlag flag ) : hkpAction(flag) {}
};

#include <Physics/Dynamics/Action/hkpUnaryAction.inl>

#endif // HK_DYNAMICS2_UNARY_ACTION_H

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
