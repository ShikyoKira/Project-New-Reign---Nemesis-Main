/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_UTILITIES_BREAK_OFF_PARTS_UTIL_H
#define INC_UTILITIES_BREAK_OFF_PARTS_UTIL_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

#include <Physics/Dynamics/World/Listener/hkpWorldDeletionListener.h>
#include <Physics/Dynamics/Constraint/Contact/hkpContactImpulseLimitBreachedListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Collide/Deprecated/hkpCollisionListener.h>
#include <Physics/Dynamics/Constraint/hkpConstraintListener.h>
#include <Physics/Dynamics/World/Extensions/hkpWorldExtension.h>

class hkpPhysicsSystem;
class hkpRigidBody;
class hkBitField;

	/// This is an interface to the game/graphics engine and implements the user part of the hkpBreakOffPartsUtil
class hkpBreakOffPartsListener
{
public:	
HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpBreakOffPartsListener);
	virtual ~hkpBreakOffPartsListener(){}

		/// Event structure
	struct ContactImpulseLimitBreachedEvent
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpBreakOffPartsListener::ContactImpulseLimitBreachedEvent );

		struct PointInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpBreakOffPartsListener::ContactImpulseLimitBreachedEvent::PointInfo );
			hkpRigidBody* m_collidingBody;
			hkpShapeKey   m_brokenShapeKey;
			hkBool        m_isContact;				///< if a contact force limit is exceeded, this is set to true, otherwise a constraint impulse limit has been exceeded.
			hkReal		  m_breakingImpulse;		///< The impulse which caused the breaking.
			hkReal		  m_contactPointDirection; // either +1 or -1
			const hkContactPoint* m_contactPoint;
			const hkpContactPointProperties* m_properties;
			const hkpSimpleConstraintContactMgr* m_internalContactMgr;	///< The contact manager.
		};

		hkpRigidBody*						 m_breakingBody;
		hkInplaceArray<PointInfo,4> m_points;
	};


		/// breaks off a subpart and returns the newly created objects.
		/// The implementation can decide to break off several elements, it just has to return
		/// the removed shapekeys and the created bodies and constraints.
		/// This is called from deep within the engine, so the implementation has to be THREADSAFE!
		/// However there are never two calls to this listener in parallel thanks to a critical section in hkpBreakOffPartsUtil
	virtual hkResult breakOffSubPart( const ContactImpulseLimitBreachedEvent& event, hkArray<hkpShapeKey>& keysBrokenOffOut, hkpPhysicsSystem& bodiesOut ) = 0;
};



	/// This is a helper class, breaking off pieces from a compound body.
	/// The basic idea is that this class properly monitors and clips the impulse acting on a compound sub part.
	/// So it basically can be used instead of the usual hkContactPointCallback.
	///   - There should only be one instance of this class per hkpWorld.
	///   - This only works if you can identify a sub piece using a single hkpShapeKey, that means
	///     only one hkMoppShape is allowed per rigid body (e.g., hkpExtendedMeshShape)
	///   - If you use this class on the PlayStation(R)3 SPU, the default implementation on the SPU will put
	///     the shape key into the materials userData
class hkpBreakOffPartsUtil: public hkpWorldExtension, public hkpContactImpulseLimitBreachedListener, hkpEntityListener, hkpConstraintListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_WORLD );

			/// Creates an instance of this class and add itself to the world.
			/// You only need one per world. After construction, you should
			/// add this to the world using hkpWorld::addWorldExtension. The
			/// hkpBreakOffPartsListener is used to decide on the destruction
			/// details and needs to be implemented by the user
		hkpBreakOffPartsUtil( hkpBreakOffPartsListener* listenerInterface );

			/// Flag a subpart of an object to break off above a given impulse limit
		void markPieceBreakable( hkpEntity* entity, hkpShapeKey key, hkReal maxImpulse );

			/// Flag a whole entity to be breakable
		void markEntityBreakable( hkpEntity* entity, hkReal maxImpulse );

			/// Set factor used to multiply maximum impulse for attached constraints.
			/// This function must be used when setting max impulses for individual hkpShapeKeys,
			/// and not for the entire entity with markEntityBreakable().
		void setMaxConstraintImpulse( hkpEntity* entity, hkReal maxConstraintImpulse );

			/// Flag a whole entity to be unbreakable
		void unmarkEntityBreakable( hkpEntity* entity );

			/// Flag a subpart of an object not to break off
		void unmarkPieceBreakable( hkpEntity* entity, hkpShapeKey key );

			/// Safely removes a piece from a listshape, or a listshape wrapped in a moppshape
			/// Both the MOPP and listshapes are updated.
		static void HK_CALL removeKeysFromListShape( hkpEntity* entity, hkpShapeKey* keysToRemove, int numKeys);


			/// Safely removes a piece from a listshape, or a listshape wrapped in a moppshape
			/// In this function you pass in only the enabled keys, all other keys are removed.
		static void HK_CALL removeKeysFromListShapeByEnabledList( hkpEntity* entity, const hkBitField& enabledKeys );


		//
		//	internal section
		//
	public:
		virtual void performAttachments( hkpWorld* world );
		virtual void performDetachments( hkpWorld* world );

	protected:
		~hkpBreakOffPartsUtil();

			// this removes pieces which are to be broken off
		virtual void contactImpulseLimitBreachedCallback( const hkpContactImpulseLimitBreachedListenerInfo* breachedContacts, int numBreachedContacts );

		        void contactImpulseLimitBreachedCallback_forToi( const hkpContactImpulseLimitBreachedListenerInfo* breachedContact );

			// hkWorldListenerImplementation
		virtual void worldDeletedCallback( hkpWorld* world);

			// hkEntityListenerImplementation
		virtual void entityRemovedCallback( hkpEntity* entity );

			// hkpConstraintListener interface implementation
		virtual void constraintAddedCallback( hkpConstraintInstance* constraint );
		virtual void constraintRemovedCallback( hkpConstraintInstance* constraint ) {}


	public:

			/// A small helper class, which gets attached to an hkpEntity.
			/// We provide two implementations of this, see below.
		class LimitContactImpulseUtil: public hkReferencedObject, public hkpCollisionListener
		{
			public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				/// Associate the given max impulse with the given key.
				inline void setMaxImpulseForShapeKey ( const hkpShapeKey key, hkUint8 max_impulse );

				/// Get the max impulse associated with the given key.
				/// If no impulse has been associated with this key, zero is returned.
				inline hkUint8 getMaxImpulseForKey ( const hkpShapeKey key ) const; 

				/// Stop storing the associated impulse value for this key.
				inline void removeKey ( const hkpShapeKey key );

				/// Checks if there are any hkpShapeKey-specific max impulse values defined.
				inline hkBool32 hasShapeKeySpecificMaxImpulses() { return m_shapeKeyToMaxImpulse.getSize(); }

				/// Return the weakest point on the object.
				void findWeakestPoint( hkpShapeKey& keyOut, hkReal& weakestImpulseOut );

				virtual ~LimitContactImpulseUtil();
			protected:
					/// Protected constructor.
				LimitContactImpulseUtil( hkpBreakOffPartsUtil* breakUtil, hkpEntity* entity );

					/// Used by the CPU callbacks (the contactPointCallback and the
					/// contactPointAddedCallback), but not used by the SPU version.
				virtual void limitContactImpulse( const hkUFloat8& maxImpulse, bool isEntityA, hkpContactPointProperties* properties );
		
					/// Calculate a maxImpulse value given that m_entity is colliding with other
					/// at key.
					/// \return0 if no maxImpulse should be applied in this case.
				inline hkUFloat8 getMaxImpulseForContactPoint( hkpShapeKey key, hkpRigidBody* other ) const;

			public:
				HK_ALIGN16(hkpEntity*       m_entity);

				hkUFloat8                   m_maxImpulse;		// set to 255 = (hkUFloat8::MAX_VALUE-1) if not breakable

				hkReal						m_maxConstraintImpulse;

			protected:

				/// Stores the max impulse for given shape key.
				/// The first value is a shape key
				/// The second value is the int value field of hkUfloat8.
				hkPointerMap<hkpShapeKey, hkUint8> m_shapeKeyToMaxImpulse;

				hkpBreakOffPartsUtil* m_breakOffUtil;
		};

		enum BreakOffGameControlResult
		{
			PROCESSING_ERROR,
			BREAK_OFF,
			DO_NOT_BREAK_OFF,
			USE_LIMIT
		};

		/// The functor for user interception of breaking events
		class GameControlFunctor : public hkReferencedObject
		{
		public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DESTRUCTION);
			GameControlFunctor() : hkReferencedObject() {}
			virtual ~GameControlFunctor() {}

			/// The decision function that says whether the \a limitedBody should be broken by impact of \a otherBody.
			/// The \a key is the shape key in contact in the \a otherBody and the currently set \a maxImpulse for breaking the \a limitedBody.
			/// This implementation simply returns USE_LIMIT.
			virtual hkpBreakOffPartsUtil::BreakOffGameControlResult test(const hkpRigidBody* limitedBody, const hkpRigidBody* otherBody, const hkpShapeKey key, const hkUFloat8& maxImpulse);
		};

	public:
			/// Get the pointer to the entity's LimitContactImpulseUtil or HK_NULL if there isn't one.
		HK_FORCE_INLINE static LimitContactImpulseUtil* HK_CALL getLimitContactImpulseUtilPtr( const hkpEntity* entity );

	protected:
			/// By default this implementation creates a LimitContactImpulseUtilDefault which uses
			/// deprecated callbacks in-order to benefit from pre-existing SPU support. If you
			/// want different behavior, you should override this method to create a subclass of
			/// LimitContactImpulseUtilCpuOnly.
		virtual LimitContactImpulseUtil* createLimitContactImpulseUtil( hkpEntity* entity );

	private:
			/// return the entity util if one exists or create one
		HK_FORCE_INLINE LimitContactImpulseUtil* getOrCreateUtil( hkpEntity* entity );

	public:
		//
		// Two implementations of LimitContactImpulseUtil
		//

			/// The default implementation uses deprecated callbacks in-order to benefit from
			/// pre-existing SPU support.
		class LimitContactImpulseUtilDefault : public LimitContactImpulseUtil
		{
			public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				inline LimitContactImpulseUtilDefault( hkpBreakOffPartsUtil* breakUtil, hkpEntity* entity );

			protected:
				// hkpCollisionListener interface implementation
				virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event);
		};

			/// This implementation uses the cross-platform (CPU only) contactPointCallbacks,
			/// allowing its behavior to be overridden easily. You can override the callback
			/// itself or override the limitContactImpulse method.
		class LimitContactImpulseUtilCpuOnly : public LimitContactImpulseUtil
		{
			public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				inline LimitContactImpulseUtilCpuOnly( hkpBreakOffPartsUtil* breakUtil, hkpEntity* entity ) : LimitContactImpulseUtil( breakUtil, entity ) {}

				virtual void contactPointCallback( const hkpContactPointEvent& event );
		};

	public:
			// an optional critical section to serialize access to this utility
		hkCriticalSection* m_criticalSection;

		hkpBreakOffPartsListener* m_breakOffPartsListener;

		hkRefPtr<GameControlFunctor> m_breakOffGameControlFunctor;
};

#include <Physics/Utilities/Destruction/BreakOffParts/hkpBreakOffPartsUtil.inl>

#endif //INC_UTILITIES_BREAK_OFF_PARTS_UTIL_H

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
