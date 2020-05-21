/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_FILTERED_SURFACE_VELOCITY_UTIL
#define HK_UTILITIES2_FILTERED_SURFACE_VELOCITY_UTIL


#include <Physics/Utilities/Collide/ContactModifiers/SurfaceVelocity/hkpSurfaceVelocityUtil.h>

class hkpRigidBody;

	/// Adds a surface velocity to all enabled entities.
	/// Note: If you want to set a surface velocity once and forever and you do not keep a pointer to
	/// the instance, do not forget to call removeReference
class hkpFilteredSurfaceVelocityUtil: public hkpSurfaceVelocityUtil
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);

			/// Creates a handle to a filtered surface velocity.
			///
			/// Will call setSurfaceVelocity() in hkpResponseModifier, which gives the
			/// surface a conveyor-belt like behavior. The propertyId is used as the property key
			/// to store a flag within an enabled entity. This value should be unique.
		hkpFilteredSurfaceVelocityUtil(hkpRigidBody* body, const hkVector4& surfaceVelocityWorld, int propertyId);

			/// Enables surface velocity for the supplied entity.
			///
			/// Also stores a flag as property in the entity using the unique propertyId supplied in this
			/// class's constructor.
			/// To understand constraintOwner, please read the reference manual for hkpResponseModifier
		void enableEntity(hkpEntity *entity, class hkpConstraintOwner* constraintOwner = HK_NULL );

			/// Disables surface velocity for the supplied entity. Also removes the flag property from entity.
			/// To understand constraintOwner, please read the reference manual for hkpResponseModifier
		void disableEntity(hkpEntity *entity, hkpConstraintOwner* constraintOwner = HK_NULL);


	protected:

			// The hkpContactListener interface implementation
		virtual void contactPointCallback( const hkpContactPointEvent& event );

	protected:

			/// Custom property id used when marking an entity as 'enabled'
		int m_propertyId;

};

#endif		// HK_UTILITIES2_FILTERED_SURFACE_VELOCITY_UTIL

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
