/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLISION_CALLBACK_UTIL
#define HK_COLLISION_CALLBACK_UTIL

#include <Physics/Dynamics/Constraint/hkpConstraintListener.h>
#include <Physics/Dynamics/World/Extensions/hkpWorldExtension.h>
#include <Physics/Dynamics/Collide/ContactListener/Util/hkpEndOfStepCallbackUtil.h>

/// This is a constraint listener which will fire hkpCollisionAdded/RemovedCallbacks as a
/// contact constraint is added or removed from the world.
class hkpCollisionCallbackUtil : public hkpWorldExtension, public hkpConstraintListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS );

			/// Constructor.
		hkpCollisionCallbackUtil() : hkpWorldExtension( s_id ) {}

			/// Require that a hkpCollisionCallbackUtil be added to the world
			/// if one has not been added already.
			/// (Wraps hkpWorldExtension::requireExtension.)
		inline static void HK_CALL requireCollisionCallbackUtil( hkpWorld* world );

			/// Release the requirement that a hkpCollisionCallbackUtil be
			/// in the world.
			/// (Wraps hkpWorldExtension::releaseExtension.)
		inline static void HK_CALL releaseCollisionCallbackUtil( hkpWorld* world );

			/// Get a pointer to the world's collision callback util.
			/// (Wraps hkpWorldExtension::findWorldExtension.)
		inline static hkpCollisionCallbackUtil* HK_CALL findCollisionCallbackUtil( const hkpWorld* world );

	public:
		// hkpWorldExtension interface.
		virtual void performAttachments( hkpWorld* world );
		virtual void performDetachments( hkpWorld* world );

		// hkpConstraintListener interface.
		virtual void constraintAddedCallback( hkpConstraintInstance* constraint );
		virtual void constraintRemovedCallback( hkpConstraintInstance* constraint );

	public:
			/// For convenience, this class contains the default hkpEndOfStepCallbackUtil.
		hkpEndOfStepCallbackUtil m_endOfStepCallbackUtil;

			/// The util's extension id.
		static const int s_id = HK_WORLD_EXTENSION_COLLISION_CALLBACK;
};

#include <Physics/Dynamics/Collide/ContactListener/Util/hkpCollisionCallbackUtil.inl>

#endif // HK_COLLISION_CALLBACK_UTIL

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
