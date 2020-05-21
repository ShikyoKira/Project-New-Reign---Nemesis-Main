/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONSTRAINTSOLVER2_CONTACT_POINT_PROPERTIES_H
#define HK_CONSTRAINTSOLVER2_CONTACT_POINT_PROPERTIES_H

#include <Common/Base/hkBase.h>
#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>
#include <Common/Base/Types/Physics/ContactPoint/hkContactPointMaterial.h>

#define HK_NUM_EXTENDED_USER_DATAS_IN_TOI_EVENT 7

struct hkpSimpleContactConstraintAtom; 

	/// This class is used to get and set the friction for a contact point. You can also use it to attach your own user data
	/// to a contact point. This can be used for example to set a friction map value in when a contact point is added
	/// so that the same data can be used when the contact point is being updated (from a processContactCallback() for example)
class hkpContactPointProperties : public hkpSolverResults, public hkContactPointMaterial
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpContactPointProperties );

			/// returns true if this contact point was really used by the solver
			/// and an impulse was applied
		inline hkBool wasUsed()
		{
			return !isPotential() && ( m_impulseApplied > 0.f );
		}

	public:
			// internal data, not for client use
		hkReal m_internalDataA;

		typedef hkUint32 UserData;

		// The use of ExtendedUserData for anything other than shape keys is deprecated.
		static inline int getNumExtendedUserDatas(const hkpSimpleContactConstraintAtom* atom, int bodyIdx);
		inline UserData* getStartOfExtendedUserData( const hkpSimpleContactConstraintAtom* atom );
		inline UserData& getExtendedUserData(const hkpSimpleContactConstraintAtom* atom, int bodyIdx, int i);
};

HK_CLASSALIGN16(class) hkContactPointPropertiesWithExtendedUserData16 : public hkpContactPointProperties
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkContactPointPropertiesWithExtendedUserData16 );

		void set(const hkContactPointPropertiesWithExtendedUserData16& other)
		{
			*this = other;
		}

		UserData m_extendedUserDatas[HK_NUM_EXTENDED_USER_DATAS_IN_TOI_EVENT];
};

class hkpContactPointPropertiesStream
{
public:
	HK_FORCE_INLINE hkpContactPointProperties* asProperties() { return reinterpret_cast<hkpContactPointProperties*>(this); }
	HK_FORCE_INLINE const hkpContactPointProperties* asProperties() const { return reinterpret_cast<const hkpContactPointProperties*>(this); }

#if defined HK_DEBUG
	HK_FORCE_INLINE void operator[](int i) { HK_ASSERT2(0xad7655aa, false, "Array-element access operator not allowed. Offset the pointer by proper striding and then conver to (non-Ex) hkpContactPointProperties."); }
#endif	
};

// Only include that after the hkpContactPointProperties are defined
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>	


inline int hkpContactPointProperties::getNumExtendedUserDatas(const hkpSimpleContactConstraintAtom* atom, int bodyIdx)
{
	HK_ASSERT2(0xad7854aa, (&atom->m_numUserDatasForBodyA)+1 == (&atom->m_numUserDatasForBodyB), "Member variables incorrectly ordered.");
	return (&atom->m_numUserDatasForBodyA)[bodyIdx];
}


inline hkpContactPointProperties::UserData* hkpContactPointProperties::getStartOfExtendedUserData( const hkpSimpleContactConstraintAtom* atom )
{
	// Assert this is the right atom.. This doesn't work for TOIs...
	// This assert is disabled for TOIs
	HK_ASSERT2(0xad765512, m_internalDataA == 321.0f || ((hkUlong) atom < (hkUlong) this && (hkUlong) this < (hkUlong) hkAddByteOffset( (void*) atom, atom->m_sizeOfAllAtoms ) ), "The properties doesn't belong to that atom." );
	return (hkpContactPointProperties::UserData*) ( this + 1 );
}


inline hkpContactPointProperties::UserData& hkpContactPointProperties::getExtendedUserData(const hkpSimpleContactConstraintAtom* atom, int bodyIdx, int i) 
{
	// Assert this is the right atom.. This doesn't work for TOIs...
	// This assert is disabled for TOIs
	HK_ASSERT2(0xad765512, m_internalDataA == 321.0f || ((hkUlong)atom < (hkUlong)this && (hkUlong)this < (hkUlong)hkAddByteOffset((void*)atom, atom->m_sizeOfAllAtoms)), "The properties doesn't belong to that atom.");
	HK_ASSERT2(0xad8755aa, i < getNumExtendedUserDatas(atom, bodyIdx) && 0 <= bodyIdx && bodyIdx <= 1, "Index out of range.");
	return *(hkpContactPointProperties::UserData*)hkAddByteOffset((this+1), sizeof(hkpContactPointProperties::UserData) * (bodyIdx * getNumExtendedUserDatas(atom, 0) + i));
}


#endif // HK_CONSTRAINTSOLVER2_CONTACT_POINT_PROPERTIES_H

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
