/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CONSTRAINT_ATOM_UTIL_H
#define HK_DYNAMICS2_CONSTRAINT_ATOM_UTIL_H

#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

#	define HK_ON_PLATFORM_SPU(code)

class hkpSimpleContactConstraintAtomUtil
{
	public:
		  // Expands both the contactPoint and contactPointProperties arrays by one element. 
		  // The atom may get reallocated, and the updated pointer is returned.
		static hkpSimpleContactConstraintAtom* expandOne(hkpSimpleContactConstraintAtom* oldAtom_mightGetDeallocated, hkPadSpu<hkBool>& atomReallocated);

			// Analogical to hkArray::removeAtAndCopy() on both the contactPoint and contactPointProperties arrays. 
		inline static void removeAtAndCopy(hkpSimpleContactConstraintAtom* atom, int index);

		  // Analogical to hkArray::optimizeCapacity() on both the contactPoint and contactPointProperties arrays. 
		  // The atom may get reallocated, and the updated pointer is returned.
		static hkpSimpleContactConstraintAtom* optimizeCapacity(hkpSimpleContactConstraintAtom* oldAtom_mightGetDeallocated, int numFreeElemsLeft, hkPadSpu<hkBool>& atomReallocated);

		static hkpSimpleContactConstraintAtom* HK_CALL allocateAtom( int numReservedContactPoints , int numExtraUserDatasA, int numExtraUserDatasB
			, int maxNumContactPoints
			);

		static void copyContents(hkpSimpleContactConstraintAtom* dst, const hkpSimpleContactConstraintAtom* src);

		inline static void HK_CALL deallocateAtom(hkpSimpleContactConstraintAtom* atom);

		static inline int calculateAtomSize( int numReservedContactPoints, int numExtraUserDatas)
		{
			int size = HK_NEXT_MULTIPLE_OF(16,sizeof(hkpSimpleContactConstraintAtom)) + numReservedContactPoints * ( sizeof(hkContactPoint) + sizeof(hkpContactPointProperties) + numExtraUserDatas * sizeof(hkpContactPointProperties::UserData) );
			return HK_NEXT_MULTIPLE_OF(16, size);
		}

};

void hkpSimpleContactConstraintAtomUtil::deallocateAtom(hkpSimpleContactConstraintAtom* atom)
{
	hkpSimpleContactConstraintAtom* localAtom = HK_GET_LOCAL_CONTACT_ATOM(atom);

	extern int atomSizeRoundUp( int );
	int allocsize = atomSizeRoundUp( localAtom->m_sizeOfAllAtoms );
	hkDeallocateChunk( reinterpret_cast<char*>(atom), allocsize, HK_MEMORY_CLASS_DYNAMICS );
}



#endif // HK_DYNAMICS2_CONSTRAINT_ATOM_UTIL_H

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
