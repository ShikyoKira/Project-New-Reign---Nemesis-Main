/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_VISCOSE_SURFACE_UTIL
#define HK_UTILITIES2_VISCOSE_SURFACE_UTIL


#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

#include <Physics/Dynamics/Entity/hkpEntity.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>


class hkpRigidBody;

	/// Makes a surface a little slippery (like thick oil or tar).
	/// It allows for removing objects which got stuck in a narrow gap.
	/// Typical applications include cars getting stuck between two buildings
	/// Note: you have to call makeSurfaceViscose before you add the entity to the world
	/// Note: this class only works if the default hkpSimpleConstraintContactMgr is used
class hkpViscoseSurfaceUtil: public hkReferencedObject, private hkpContactListener, private hkpEntityListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);

			/// Creates a handle to a surface velocity
		static inline void HK_CALL makeSurfaceViscose(hkpRigidBody* entity);	
		
	protected:
		hkpViscoseSurfaceUtil( hkpRigidBody* entity );

		// The contact listener interface.

		void contactPointCallback( const hkpContactPointEvent& event );

		// The hkpEntityListener interface implementation

		void entityDeletedCallback( hkpEntity* entity );

		//
		// For internal use
		//	
	public:
		hkpEntity*   m_entity;
};

void HK_CALL hkpViscoseSurfaceUtil::makeSurfaceViscose(hkpRigidBody* entity)
{
	new hkpViscoseSurfaceUtil( entity );
}

#endif		// HK_UTILITIES2_VISCOSE_SURFACE_UTIL

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
