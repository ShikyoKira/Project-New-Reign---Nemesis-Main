/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_WORLD_EXTENSION_H
#define HK_WORLD_EXTENSION_H

#include <Physics/Dynamics/World/hkpWorld.h>

/// Ids of known world extensions.
enum hkpKnownWorldExtensionIds
{
		/// Id for an extension which doesn't need to found using
		/// hkpWorld::findWorldExtension.
	HK_WORLD_EXTENSION_ANONYMOUS = -1,

		/// Id for the hkpBreakOffPartsUtil.
	HK_WORLD_EXTENSION_BREAK_OFF_PARTS = 1000,
		/// Id for the hkpCollisionCallbackUtil.
	HK_WORLD_EXTENSION_COLLISION_CALLBACK = 1001

};

/// hkpWorldExtension is an abstract class for objects whose lifetime is
/// partly controlled by the world. When an extension is added to the world,
/// the world keeps a reference to it. This reference is removed when the
/// world is destroyed.
///
/// hkpWorldExtensions usually modify the world in some way, usually by
/// implementing listener interfaces.
///
/// This abstract class is intended to support two different usage patterns.
///
/// 1. For complex extensions with non-default constructors, you should create
/// the extension and add it to the world using addWorldExtension. You can
/// optionally release your reference. An example of this use case is the
/// hkpBreakOffPartsUtil.
///
/// 2. You want a single instance of an extension to be present in the world.
/// For this, the extension must have a default constructor and a unique id.
/// You should use the static requireExtension and releaseExtension methods
/// in this class. An example of this use case is the hkpCollisionCallbackUtil.
class hkpWorldExtension : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Implement this to perform the operations that should occur
			/// when this extension is attached to the world. (E.g. add
			/// listeners)
		virtual void performAttachments( hkpWorld* world ) = 0;

			/// Implement this to perform the operations that should occur
			/// when the extension is detached from the world. (E.g. remove
			/// listeners)
		virtual void performDetachments( hkpWorld* world ) = 0;

			/// Return the id of this extension.
		inline int getId() const;

			/// Return the world to which this extension has been added, or
			/// HK_NULL if it is not currently added to the world.
		inline hkpWorld* getWorld() const;

	public:
		//
		// The following methods make use of the attachment counting mechanism.
		// These methods require T to be a subclass of hkpWorldExtension with a
		// default constructor and a static int s_id member.
		//

			/// Add a new instance of the extension to the world if there isn't
			/// one, or increment its attachment count otherwise.
		template <typename T>
		static void requireExtension( hkpWorld* world );

			/// Decrement the extension's attachment count and remove it if it
			/// has reached zero.
		template <typename T>
		static void releaseExtension( hkpWorld* world );

	protected:

			/// Construct an extension with a given id.
			/// You can use the default id for extensions which aren't
			/// intended to be unique in the world, and don't need to
			/// be searched for in the world's extension array.
		hkpWorldExtension( int id = HK_WORLD_EXTENSION_ANONYMOUS );

	public:
		// Internal

			/// Called by the world when an extension is added.
		inline void addedToWorld( hkpWorld* world );

			/// Called by the world when an extension is removed.
		inline void removedFromWorld( hkpWorld* world );

	protected:
			/// The world this extension has been attached to, or NULL.
		hkpWorld* m_world;

			/// The id of this extension.
		int m_id;

			/// The number of times it has been requested that this extension
			/// be attached to the world.
		hkUint16 m_attachmentCount;
};

#include <Physics/Dynamics/World/Extensions/hkpWorldExtension.inl>

#endif // HK_WORLD_EXTENSION_H

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
