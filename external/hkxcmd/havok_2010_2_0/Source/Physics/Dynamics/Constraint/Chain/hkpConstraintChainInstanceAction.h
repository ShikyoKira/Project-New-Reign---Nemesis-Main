/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CONSTRAINT_CHAIN_INSTANCE_ACTION_H
#define HK_DYNAMICS2_CONSTRAINT_CHAIN_INSTANCE_ACTION_H

#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>

#include <Physics/Dynamics/Action/hkpArrayAction.h>

extern const class hkClass hkpConstraintChainInstanceActionClass;

          

class hkpEntity;


	/// Internal action only to be used by hkpConstraintChainInstance. It assures proper logical linking of all the entities connected by a hkpConstraintChainInstance.
class hkpConstraintChainInstanceAction : public hkpAction
{
	public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
	protected:

		HK_DECLARE_REFLECTION();

		friend class hkpConstraintChainInstance;

			/// Default ctor. Only to be run from the hkpConstraintChainInstance ctor.
		hkpConstraintChainInstanceAction(hkpConstraintChainInstance* chain) : hkpAction(0)
		{
			m_constraintInstance = chain;
			// we don't add reference. This object belongs to the chain instance.

			HK_ASSERT2(0xad78855d, m_constraintInstance->m_chainedEntities.getSize() == 0, "hkpConstraintChainInstanceAction can only be created in the constructor of hkpConstraintChainInstance.");
		}

	public:

			/// Dtor.
		virtual ~hkpConstraintChainInstanceAction()
		{
		}
			/// Interface implementation.
		virtual void applyAction( const hkStepInfo& stepInfo );

			/// Interface implementation. Cloning is not supported.
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const;

			/// Interface implementation.
		virtual void entityRemovedCallback(hkpEntity* entity);

			/// Returns the constraint chain instance that this action belongs to.
		inline hkpConstraintChainInstance* getConstraintInstance() { return m_constraintInstance; }

			/// Gets the action's entities. This function references the entity array stored in the constraint chain.
		virtual void getEntities( hkArray<hkpEntity*>& entitiesOut );

	public:

			/// The constraint chain instance that this action belongs to.
		class hkpConstraintChainInstance* m_constraintInstance; //+owned(false)
		
			/// Serialization ctor. Not to be used, as this class is not meant to be serialized.
		hkpConstraintChainInstanceAction( class hkFinishLoadedObjectFlag flag ) : hkpAction(flag) {}
	
};



#endif // HK_DYNAMICS2_CONSTRAINT_CHAIN_INSTANCE_ACTION_H

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
