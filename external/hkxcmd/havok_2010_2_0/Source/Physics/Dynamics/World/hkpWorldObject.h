/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_WORLD_OBJECT_H
#define HK_DYNAMICS2_WORLD_OBJECT_H

#include <Physics/Collide/Agent3/Machine/Nn/hkpLinkedCollidable.h>
#include <Common/Base/DebugUtil/MultiThreadCheck/hkMultiThreadCheck.h>
#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Dynamics/Common/hkpProperty.h>

class hkpWorld;
class hkpShape;
class hkpPhantom;
class hkpRigidBody;

extern const hkClass hkpWorldObjectClass;

	// This namespace refers to the operation postponing framework in hkpWorldOperationQueue.h
namespace hkWorldOperation
{
		// A result value informing whether a critical operation was performed immediately
		// or postponed ( -- e.g. when requested in a callback during collision detection phase, 
		// when the hkpWorld is locked).
	enum Result
	{
		POSTPONED,
		DONE
	};
}

	/// This is the base class for hkpEntity and hkpPhantom. It contains a list of hkpProperty key-value pairs
	/// and also a hkpCollidable object.
class hkpWorldObject : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Adds a property to the entity. Properties must be unique.
			/// You can use properties to add additional information to an entity - e.g.
			/// for using your own collision filters or backlinks into your game.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void addProperty( hkUint32 key, hkpPropertyValue value );

			/// Removes the property of the specified type. This function returns the property
			/// removed, or 0 if there is no property of the given type.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		hkpPropertyValue removeProperty( hkUint32 key );

			/// set a property, if property already exists -> override, else create
		void setProperty( hkUint32 key, hkpPropertyValue value);

			/// Should multithreading checks enabled or disabled
		enum MtChecks
		{
			MULTI_THREADING_CHECKS_ENABLE,	///< enable multithreading access checks (default)
			MULTI_THREADING_CHECKS_IGNORE	///< disable multithreading access checks. Only use this if you know what you are doing and don't care about determinism.
		};

			/// Updates the property with a new value. The property's key must already exist.
			/// Note: you can set \a mtCheck to MULTI_THREADING_CHECKS_IGNORE, if you are sure to only call getProperty(), hasProperty() and editProperty()
			/// when accessing the engine from multiple threads.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] [plock,HK_ACCESS_RW] [this,HK_ACCESS_RW] );
		void editProperty( hkUint32 key, hkpPropertyValue value, MtChecks mtCheck = MULTI_THREADING_CHECKS_ENABLE );


			/// Returns the property specified, or 0 if there is no property of the given type.
			/// Note: you can set \a mtCheck to MULTI_THREADING_CHECKS_IGNORE, if you are sure to only call getProperty(), hasProperty() and editProperty()
			/// when accessing the engine from multiple threads.
		inline hkpPropertyValue getProperty( hkUint32 key, MtChecks mtCheck = MULTI_THREADING_CHECKS_ENABLE ) const;

			/// Returns whether a property of this type has been set for the entity.
			/// Note: you can set \a mtCheck to MULTI_THREADING_CHECKS_IGNORE, if you are sure to only call getProperty(), hasProperty() and editProperty()
			/// when accessing the engine from multiple threads.
		inline bool hasProperty( hkUint32 key, MtChecks mtCheck = MULTI_THREADING_CHECKS_ENABLE ) const;

			/// locks all property field of all entities, allows for
			/// calling editProperty even if you only have 'read only' access for this entity.
			/// Warning: If you use this function in a multi threaded environment, you most likely
			/// end up with a nondeterministic behavior.
		void lockProperty( hkUint32 key );

			/// unlocks a given locked property
		void unlockProperty( hkUint32 key );

			/// Clear and deallocate the properties of this world object.
		inline void clearAndDeallocateProperties();



			/// Gets the collidable owned by this world object.
			/// If the object has no collision detection representation,
			/// the shape in the collidable will be HK_NULL.
		inline hkpCollidable* getCollidableRw();

			/// Gets the collidable owned by this world object.
			/// If the object has no collision detection representation, the shape in the collidable will be HK_NULL.
		inline const hkpCollidable* getCollidable() const;


		inline hkpLinkedCollidable* getLinkedCollidable();

		inline const hkpLinkedCollidable* getLinkedCollidable() const;

		inline const hkpLinkedCollidable* getCollidableMtUnchecked() const;

			/// Returns true if this object is added to a world
		inline hkBool isAddedToWorld() const;

			/// Get the world that owns this object. This is HK_NULL if the object has not been added to the world
		inline hkpWorld* getWorld() const;

			/// Get a user data pointer for the world object
		inline hkUlong getUserData() const;

			/// Set a user data pointer for the world object
		inline void setUserData( hkUlong data );

			/// Get the name of this world object.
		inline const char* getName() const;

			/// Set the name of this world object. The string is copied internally.
		inline void setName( const char* name );

		void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const;

			/// Set the shape of an hkpEntity or an hkpPhantom.
		virtual hkWorldOperation::Result setShape( const hkpShape* shape );

			/// Must be called if the internal data of the shape associated with an entity is to be changed.
			/// The function has a similar effect to setShape in that it tells the physics world to update its
			/// cached shape data, while not suffering from the overhead of deallocating / allocating a
			/// new shape. The changes to the shape should not be done before this function is called, but instead from
			/// a provided object, derived from hkpShapeModifier. This way, the shape modifications can be done while the world is locked.
		virtual hkWorldOperation::Result updateShape(hkpShapeModifier* shapeModifier);

			/// Lock this class and all child classes for read only access for this thread
			/// Note: This is only for debugging and does not wait to get exclusive access,
			/// but simply assert if another thread locked the hkpWorld. You must read the
			/// user guide about multithreading to use this.
		inline void markForRead( );

			/// Lock this class and all child classes for read write access for this thread
			/// Note: This is only for debugging and does not wait to get exclusive access,
			/// but simply assert if another thread locked the hkpWorld. You must read the
			/// user guide about multithreading to use this.
		inline void markForWrite( );
			
			// implementation of above function
		void markForWriteImpl( );

			/// Undo lockForRead
			/// Note: This is only for debugging and does not wait to get exclusive access,
			/// but simply assert if another thread locked the hkpWorld. You must read the
			/// user guide about multithreading to use this.
		inline void unmarkForRead( );

			/// Unlock For write
			/// Note: This is only for debugging and does not wait to get exclusive access,
			/// but simply assert if another thread locked the hkpWorld. You must read the
			/// user guide about multithreading to use this.
		inline void unmarkForWrite();

			/// Check for read write access
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void checkReadWrite();

			/// Check for read only access
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		void checkReadOnly() const;

	public:

		/// Broadphase types.
		enum BroadPhaseType
		{
			/// 
			BROAD_PHASE_INVALID,
			/// hkpEntity.
			BROAD_PHASE_ENTITY,
			/// hkpPhantom.
			BROAD_PHASE_PHANTOM,
			/// hkpBroadPhaseBorder's objects (AABB phantoms).
			BROAD_PHASE_BORDER,
			/// 
			BROAD_PHASE_MAX_ID
		};

	protected:

		friend class hkpWorld;
		friend class hkpWorldOperationUtil;
		inline void setWorld( hkpWorld* world );

		hkpWorldObject( const hkpShape* shape, BroadPhaseType type ); 

		inline virtual ~hkpWorldObject(); // virtual for the sake of serialization flags

	public:
		// gets the motion state 
		virtual class hkMotionState* getMotionState() = 0;

		inline hkMultiThreadCheck& getMultiThreadCheck();
		inline const hkMultiThreadCheck& getMultiThreadCheck() const;

		inline void copyProperties( const hkpWorldObject* otherObj );


		void setTreeData(void* data);

		void* getTreeData() const;

	protected:

		hkpWorld* m_world; //+nosave

		hkUlong m_userData; //+default(0)

	protected: 
			/// The collidable, which can only be accessed from hkpEntity / hkpPhantom
			/// Note: this member needs to be 16 byte aligned on PlayStation(R)3
		class hkpLinkedCollidable m_collidable;

	protected:
		class hkMultiThreadCheck m_multiThreadCheck;

		hkStringPtr m_name;

	public:
		hkArray<class hkpProperty> m_properties;

	private:

		void* m_treeData; // +nosave

	public:

		hkpWorldObject( class hkFinishLoadedObjectFlag flag );
};

	/// Helper function that returns the hkpWorldObject pointer for a hkpCollidable
inline hkpWorldObject* hkGetWorldObject(const hkpCollidable* collidable)
{
	return reinterpret_cast<hkpWorldObject*>( const_cast<void*>(collidable->getOwner()) );
}

#include <Physics/Dynamics/World/hkpWorldObject.inl>

#endif	//HK_DYNAMICS2_WORLD_OBJECT_H

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
