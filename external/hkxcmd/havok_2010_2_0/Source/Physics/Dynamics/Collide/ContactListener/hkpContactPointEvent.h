/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONTACT_POINT_EVENT_H
#define HK_CONTACT_POINT_EVENT_H

#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpCollisionEvent.h>

class hkpContactPoint;
class hkpContactPointProperties;
class hkpSimulationIsland;

	/// This is the contact event received by the contactListener.
	/// It is a hkpCollisionEvent extended with information pertaining to a specific contact point.
	/// Objects of this class are always temporary, as are some of the target objects of the pointers.
class hkpContactPointEvent : public hkpCollisionEvent
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpContactPointEvent );

		enum Type 
		{
			// The three standard types of contactPointEvent fired by the physics engine.

				/// The event is the TOI responsible for a phase of continuous physics.
				/// Events of this type are fired single-threaded and deterministically.
			TYPE_TOI,
				/// The event describes a collision occurring when entities are re-collided
				/// during continuous physics. Events of this type are fired single-threaded
				/// and deterministically.
			TYPE_EXPAND_MANIFOLD,
				/// The event describes a collision occurring at a PSI, the most common type of
				/// contact point event. Events of this type can be fired multi-threaded and
				/// non-deterministically.
			TYPE_MANIFOLD,

			// Two types which are fired under special circumstances.

				/// The event is a copy of a TYPE_MANIFOLD event optionally fired at the end of
				/// the physics step by the hkpEndOfStepCallbackUtil. Events of this type are
				/// fired single-threaded and can be fired deterministically (although that
				/// is not the default).
			TYPE_MANIFOLD_AT_END_OF_STEP,

				/// The event is being fired by the hkpSaveContactPointUtil as it loads a saved contact point.
				/// Events of this type are not fired for new contact points because TYPE_MANIFOLD events will
				/// be fired for them in the following frame. Events of this type are fired single-threaded
				/// and deterministically.
			TYPE_MANIFOLD_FROM_SAVED_CONTACT_POINT
		};

	public:
			/// Constructor.
		inline hkpContactPointEvent( CallbackSource source, hkpRigidBody* a, hkpRigidBody* b, hkpSimpleConstraintContactMgr* mgr,
			Type type, hkContactPoint* cp, hkpContactPointProperties* cpp,
			hkReal* sepVel, hkReal* rotNorm, 
			hkBool fullManifold, hkBool firstCallback, hkBool lastCallback, 
			hkpShapeKey* keyStorage,
			hkpVelocityAccumulator* accA, hkpVelocityAccumulator* accB );
		
			/// Returns whether the event is a TOI.
		inline hkBool isToi() const;

			/// Get the branch of shapes, from the root shape to the colliding leaf subshape, for the given
			/// body.
			/// NOTE: The behavior of this method is undefined if the array of shape keys stored in the
			/// contactPointProperties is truncated (this can happen when a body's
			/// m_numShapeKeysInContactPointProperties is less than the depth of the shape hierarchy).
			/// \param maxShapes the maximum number of shapes for which temporary storage has been made
			///   available. The method asserts if this isn't big enough.
			/// \param buffers a pointer to an array of size (maxShapes - 1) used for temporary shape storage.
			/// \param shapesOut a pointer to an array of size maxShapes used to store the shape pointers.
			///   All but the first pointer may point into the buffers.
			/// \param numShapesOut the number of shapes.
		void getShapes( int bodyIdx, int maxShapes, hkpShapeBuffer* buffers, const hkpShape** shapesOut, int& numShapesOut ) const;

			/// Returns a pointer to the shape key storage for the first or second body.
			/// The keys in each block are stored in leaf-to-root order. If the branch is shorter than
			/// the body's m_numShapeKeysInContactPointProperties, then the last key is followed by
			/// an HK_INVALID_SHAPE_KEY.
			/// The sequence of shape keys may lead to a hkpConvexTranslateShape or a
			/// hkpConvexTransformShape, in which case any remaining shapes can be found by
			/// assuming the missing keys are 0 (the remaining shapes must be
			/// hkpConvexTranslateShapes, hkpConvexTransformShapes or the convex leaf shape).
			/// \returnHK_NULL if there is no shape key storage for the body.
		inline hkpShapeKey* getShapeKeys( int bodyIdx ) const;

			/// Returns the contact point ID (except when the event has type TOI).
		hkContactPointId getContactPointId() const;

			/// Returns the separating velocity of the two bodies at this contact point.
			/// This is the relative velocity of the objects at the point of contact projected onto the
			/// collision normal.
		inline hkReal getSeparatingVelocity() const;
			
			/// Sets the separating velocity (not in events of TYPE_MANIFOLD_AT_END_OF_STEP or old contact
			/// points). Changing this value influences the restitution. If this event is a TOI event, you
			/// should only decrease its value, otherwise objects can sink in at the expense of tons of
			/// extra CPU.
		inline void setSeparatingVelocity( hkReal sepVel ) const;

			/// Specifies how much the contact normal should be rotated in TOI events.
		inline hkReal getRotateNormal() const;

			/// When set to a non-zero value, this parameter reduces number of TOI collisions,
			/// artificially adding separating velocity to colliding bodies. The higher the relative
			/// velocity of the bodies in the contact's tangent plane, the higher the extra impulse.
			/// Note: This causes serious artifacts for collisions of e.g., a car riding along a wall and
			/// then crashing/sliding into the wall. Similarly, it may cause artifacts, when a car's
			/// chassis collides with landscape. It is advised to zero this parameter in a callback in
			/// such cases.
		inline void setRotateNormal( hkReal rotNorm ) const;

			/// Ensures that the velocities of the body are up-to-date.
			/// You must call this before applying impulses or forces to the body.
		inline void accessVelocities( int bodyIdx ) const;

			/// Ensures that any changes made to the velocities of the body are preserved.
			/// You must call this after applying impulses or forces to the body.
		inline void updateVelocities( int bodyIdx ) const;

	public:
			/// The type of contact this event describes.
		Type m_type;

			/// The contact point.
		hkContactPoint* m_contactPoint;

			/// The contact point properties (e.g., newness, enabledness, friction, restitution).
		hkpContactPointProperties* m_contactPointProperties;

			/// Are we firing callbacks for the full contact manifold, or just firing callbacks for
			/// new contacts?
		hkBool m_firingCallbacksForFullManifold;

			/// When contactPointCallbacks are being fired for the full contact manifold, this is set
			/// for the first callback fired.
		hkBool m_firstCallbackForFullManifold;

			/// When contactPointCallbacks are being fired for the full contact manifold, this is set
			/// for the last callback fired.
		hkBool m_lastCallbackForFullManifold;

	protected:
			/// This pointer will be null for events of TYPE_MANIFOLD_AT_END_OF_STEP.
		hkReal* m_separatingVelocity;

			/// This pointer is null for all events except those of TYPE_TOI.
		hkReal* m_rotateNormal;

		//
		// Internal
		//

			/// A pointer to the base of the array of shape keys.
		hkpShapeKey* m_shapeKeyStorage;

			/// Needed for updateVelocities(). These pointers are null except for TYPE_MANIFOLD events.
		hkpVelocityAccumulator* m_accumulators[2];
};

#include <Physics/Dynamics/Collide/ContactListener/hkpContactPointEvent.inl>

#endif // HK_CONTACT_POINT_EVENT_H

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
