/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_PROJECTILE_GUN_H
#define HKP_PROJECTILE_GUN_H

#include <Physics/Utilities/Weapons/hkpFirstPersonGun.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

class hkpProjectileGun;

/// Little helper for the projectile guns.
/// This projectile can act on collisions occurring in the world.
class hkpGunProjectile : public hkReferencedObject, public hkpContactListener
{
 	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		enum Flags { HIT_LISTENING = 0x4, HIT = 0x2, DESTROYED = 0x1 };

		/// Ctor
		hkpGunProjectile(hkpProjectileGun* gun, hkpRigidBody* body);

		/// Dtor
		~hkpGunProjectile();

	public:

			/// Returns true if the projectile is destroyed
		HK_FORCE_INLINE bool isDestroyed() const { return (m_flags.get() & DESTROYED) != 0; }
			/// Destroy the projectile
		void destroy();

			/// Returns true if the projectile was hit in the last simulation
		HK_FORCE_INLINE bool wasHit() const { return (m_flags.get() & HIT) != 0; }
			/// Clear the hit flag
		HK_FORCE_INLINE void clearHit() { m_flags.andWith(hkUint8(~HIT)); }

			/// Get the rigid body
		HK_FORCE_INLINE hkpRigidBody* getRigidBody() const { return m_body; }

			/// Returns true if the projectile is in the world
		HK_FORCE_INLINE bool isInWorld() const { return m_body && m_body->getWorld() != HK_NULL; }

			/// Get the t
		HK_FORCE_INLINE hkReal getExistanceTime() const { return m_existanceTime; }

			/// Returns true if it is hit listening for hits
		HK_FORCE_INLINE bool isHitListening() { return (m_flags.get() & HIT_LISTENING) != 0; }

			/// Enable/disable hit listening
		void setHitListening(bool enable);

			/// Add it to the world
		void addToWorld(hkpWorld* world);
			/// Remove from the world
		void removeFromWorld();

			/// Update the projectile
		void update(hkReal timeStep);

			/// Get the gun the projectile belongs to
		HK_FORCE_INLINE hkpProjectileGun* getGun() const { return m_gun; }

			/// If the velocity of the body is greater than minVelocity, turn the body such that
			/// it's x axis is facing along the direction of motion
		static void HK_CALL flyTrue(hkpRigidBody* body, hkReal minVelocity, hkReal timeStep);

	public:

			/// Called when destroyed (you can only destroy a projectile once)
		virtual void onDestroy();
			/// Called when a hit occurs
		virtual void onHit( const hkpContactPointEvent& event ) { m_flags.orWith(HIT); }
			/// Called on an update
		virtual void onUpdate(hkReal timeStep) {}

	private:

		void _destroyBody();
		// hkpContactListener
		virtual void contactPointCallback( const hkpContactPointEvent& event ) { onHit(event); }

	protected:

		// State flags
		hkFlags<Flags, hkUint8> m_flags;
		// The body
		hkpRigidBody* m_body;

		// Holds existance time
		hkReal m_existanceTime;
		// Gun
		hkpProjectileGun* m_gun;
};

extern const hkClass hkpProjectileGunClass;

/// Base class for guns that fire bullets with physics representation which are reacting on impact.
class hkpProjectileGun : public hkpFirstPersonGun
{
    public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		HK_DECLARE_REFLECTION();

		/// Ctor
		hkpProjectileGun(hkpWorld* world, hkdWorld* destructionWorld);
		hkpProjectileGun(class hkFinishLoadedObjectFlag flag);

		/// Dtor
		virtual ~hkpProjectileGun();

	public:

		// FirstPersonGun
		virtual void stepGun( hkReal timeStep, hkpWorld* world, const hkpRigidBody* characterBody, const hkTransform& viewTransform, bool fire, bool fireRmb );

		virtual void fireGun( hkpWorld* world, const hkTransform& viewTransform );

		virtual void reset( hkpWorld* world );

		// own methods
		virtual void onGunFired(hkpWorld* world, const hkTransform& viewTransform) {}

		virtual void onUpdate( hkReal timeStep, hkpWorld* world, const hkpRigidBody* characterBody, const hkTransform& viewTransform, bool fire, bool fireRmb );

	protected:

			/// Destroys all of the projectiles
		void clearProjectiles();
			/// Destroys and projectiles older than specified age (they are still in the projectile list tho)
		void destroyAgedProjectiles(hkReal time);
			/// Removes all projectiles from the list which are destroyed
		void removeDestroyedProjectiles();
			/// Update all of the projectiles by timestep time
		void updateProjectiles(hkReal timeStep);
			/// Clear the hit flag
		void clearHitProjectiles();

			/// Returns the first (first added), active (not destroyed) projectile. If none found returns NULL.
		hkpGunProjectile* getFirstActiveProjectile() const;

			/// Adds a projectile
		void addProjectile(hkpGunProjectile* proj);

	public:

		int m_maxProjectiles;                  //+default(5) +hk.RangeInt32(absmin=0,absmax=1000) +hk.Ui(visible=true, label="Max Projectiles in World")

		hkReal m_reloadTime;                   //+default(0.3f) +hk.RangeReal(absmin=0.0,absmax=100.0) +hk.Ui(visible=true, label="Reload delay")

			/// Counts down m_reload time. If <=0 can fire.
		hkReal m_reload;                       //+nosave

		hkArray<hkpGunProjectile*> m_projectiles; //+nosave

		hkpWorld* m_world;                     //+nosave
		hkdWorld* m_destructionWorld;          //+nosave
};

#endif // HKP_PROJECTILE_GUN_H

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
