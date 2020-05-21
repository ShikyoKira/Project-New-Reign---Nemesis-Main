/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_CONTACT_POINT_MATERIAL_H
#define HK_MATH_CONTACT_POINT_MATERIAL_H

#include <Common/Base/hkBase.h>

extern const class hkClass hkContactPointMaterialClass;


	/// This class is used to get and set the friction for a contact point. You can also use it to attach your own user data
	/// to a contact point. This can be used for example to set a friction map value in when a contact point is added
	/// so that the same data can be used when the contact point is being updated (from a processContactCallback() for example)
class hkContactPointMaterial
{
	public:

		HK_DECLARE_REFLECTION();

			/// Get the friction for the contact point, see setFriction().
		inline hkReal getFriction( ) const;


			/// Set the friction for the contact point.
			/// Usually the value will be between 0 and 1, but may be set be greater than 1.
			/// Internally this is stored as an 8.8 fixed point value, so
			/// values must lie in the range 0 (no friction) to 255 (max friction).
		inline void setFriction( hkReal r );

			/// Set the restitution for the contact point, see setRestitution().
		inline hkReal getRestitution( ) const;

			/// Set the restitution for the contact point.
			/// Usually the value will be between 0 (all energy lost in collision) and 1 (max restitution), but may be set greater than 1.
			/// Internally this is stored as an 1.7 fixed point value, so values must lie in the range 0 to 1.98.
			/// Note: for a contact point that has been around for several frames this has virtually no effect. You need to set the
			/// restitution for a contact point on creation of the contact point (when the approaching velocities are non-zero).
		inline void setRestitution( hkReal r );

			/// Get the user data
		inline hkUlong getUserData() const;

			/// Set the user data. This allows you to store any info, or a pointer with a contact point.
		inline void setUserData( hkUlong data );

			/// returns !=0 if the contact point might still be a potential contact point.
		inline hkBool32 isPotential();

			/// Sets the maximum impulse applied per solver substep. This only works well if you set the restitution between the bodies to <0.3f otherwise the system might apply a up to twice the impulse.
			/// If set to zero, an unlimited force is allowed (default).
		inline void setMaxImpulsePerStep( hkUFloat8 impulse );

			/// Gets the maximum impulse per solver step.
		inline hkReal getMaxImpulsePerStep();


			// 
			//	Internal section
			//

	protected:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkContactPointMaterial );
		// Need this for now as the memory tracker definition inserts a public:
	protected:


			/// Not used by Havok, feel free to use it.
		hkUlong m_userData; // +default(0)
			
			/// The friction stored as 8 unsigned floating point .
		hkUFloat8 m_friction;	

			/// the restitution stored as 1.7 fixed point -> range [0...1.98].
		hkUint8 m_restitution;

	public:
			/// The maximum impulse. Note: if you set this data to !=0, you have to clear CONTACT_USES_SOLVER_PATH2 in the flags field.
		hkUFloat8 m_maxImpulse;

	public:
		void reset()
		{
			m_friction.m_value = 0;
			m_maxImpulse.m_value = 0;
			m_restitution = 0;
			m_flags = CONTACT_IS_NEW;
		}


		enum FlagEnum
		{
				/// True until the contact point is passed to the solver for the first time.
			CONTACT_IS_NEW = 1,
			
				/// Internal solver optimization, do not change (engine can crash if you do).
			CONTACT_USES_SOLVER_PATH2 = 2,

				/// User data used by the breakoffpartutil.
			CONTACT_BREAKOFF_OBJECT_ID = 4,
				
				/// The contact will not be converted into a constraint if this flag is set.
			CONTACT_IS_DISABLED = 8
		};

		/// See FlagEnum for how it is used.
		hkUint8 m_flags;

		friend class hkpSaveContactPointsEndianUtil;
};

#include <Common/Base/Types/Physics/ContactPoint/hkContactPointMaterial.inl>

#endif // HK_MATH_CONTACT_POINT_MATERIAL_H

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
