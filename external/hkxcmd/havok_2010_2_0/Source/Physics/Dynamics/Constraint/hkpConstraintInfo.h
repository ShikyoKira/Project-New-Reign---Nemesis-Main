/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CONSTRAINT_INFO_H
#define HK_DYNAMICS2_CONSTRAINT_INFO_H
//! Keep the members of this synced with the generic constraint cinfo
//! as it has to save all these members.


#define hkpConstraintInfoSpu2 hkpConstraintInfo

struct hkpConstraintInfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintInfo );

		// This variable only has a meaning in an island (it is equal to sizeOfJacobians for constraints).
		// It relates to splitting buffers on playstation
	int m_maxSizeOfSchema;
	int m_sizeOfSchemas;
	int m_numSolverResults;
	int m_numSolverElemTemps;

	inline void clear( ) { m_maxSizeOfSchema = 0; m_sizeOfSchemas = 0; m_numSolverResults = 0; m_numSolverElemTemps = 0; }
	inline void addHeader( ) { m_sizeOfSchemas += HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA; }
	inline void add( int schemaSize, int numSolverResults, int numSolverTempElems ) { m_sizeOfSchemas += schemaSize; m_numSolverResults += numSolverResults; m_numSolverElemTemps += numSolverTempElems; }

	inline void add( const hkpConstraintInfo& other);
	inline void sub( const hkpConstraintInfo& other);

	inline void merge( const hkpConstraintInfo& other);

#if !defined(hkpConstraintInfoSpu2)
	inline hkpConstraintInfo& operator= (const struct hkpConstraintInfoSpu2& s);
#endif
};


inline void hkpConstraintInfo::merge( const hkpConstraintInfo& delta)
{
	m_maxSizeOfSchema = hkMath::max2( m_maxSizeOfSchema, delta.m_maxSizeOfSchema);
	m_sizeOfSchemas    += delta.m_sizeOfSchemas;
	m_numSolverResults += delta.m_numSolverResults;
	m_numSolverElemTemps += delta.m_numSolverElemTemps;
}


inline void hkpConstraintInfo::add( const hkpConstraintInfo& delta)
{
	//	HK_ASSERT2(0XAD7865543, m_maxSizeOfSchema < 10000 && delta.m_maxSizeOfSchema < 10000 && delta.m_sizeOfSchemas  < 10000, "Warning: Max schema size very (too?) large.");
	m_maxSizeOfSchema = hkMath::max2( m_maxSizeOfSchema, int(delta.m_maxSizeOfSchema));
	m_maxSizeOfSchema = hkMath::max2( m_maxSizeOfSchema, int(delta.m_sizeOfSchemas));
	m_sizeOfSchemas    += delta.m_sizeOfSchemas;
	m_numSolverResults += delta.m_numSolverResults;
	m_numSolverElemTemps += delta.m_numSolverElemTemps;
}

inline void hkpConstraintInfo::sub( const hkpConstraintInfo& delta)
{
	m_sizeOfSchemas    -= delta.m_sizeOfSchemas;
	m_numSolverResults -= delta.m_numSolverResults;
	m_numSolverElemTemps -= delta.m_numSolverElemTemps;
}


#if !defined(hkpConstraintInfoSpu2)
struct hkpConstraintInfoSpu2
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintInfo );

	// This variable only has a meaning in an island (it is equal to sizeOfJacobians for constraints).
	// It relates to splitting buffers on playstation
	hkPadSpu<int> m_maxSizeOfSchema;
	hkPadSpu<int> m_sizeOfSchemas;
	hkPadSpu<int> m_numSolverResults;
	hkPadSpu<int> m_numSolverElemTemps;

	inline void clear( ) { m_maxSizeOfSchema = 0; m_sizeOfSchemas = 0; m_numSolverResults = 0; m_numSolverElemTemps = 0; }
	inline void addHeader( ) { m_sizeOfSchemas = m_sizeOfSchemas + HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA; }
	inline void add( int schemaSize, int numSolverResults, int numSolverTempElems )
	{
		m_sizeOfSchemas = m_sizeOfSchemas + schemaSize;
		m_numSolverResults = m_numSolverResults + numSolverResults;
		m_numSolverElemTemps = m_numSolverElemTemps + numSolverTempElems;
	}

	inline void add( const hkpConstraintInfo& other);
	inline void sub( const hkpConstraintInfo& other);
};

inline void hkpConstraintInfoSpu2::add( const hkpConstraintInfo& delta)
{
	//	HK_ASSERT2(0XAD7865543, m_maxSizeOfSchema < 10000 && delta.m_maxSizeOfSchema < 10000 && delta.m_sizeOfSchemas  < 10000, "Warning: Max schema size very (too?) large.");
	m_maxSizeOfSchema = hkMath::max2( int(m_maxSizeOfSchema), int(delta.m_maxSizeOfSchema));
	m_maxSizeOfSchema = hkMath::max2( int(m_maxSizeOfSchema), int(delta.m_sizeOfSchemas));
	m_sizeOfSchemas      = m_sizeOfSchemas + delta.m_sizeOfSchemas;
	m_numSolverResults   = m_numSolverResults + delta.m_numSolverResults;
	m_numSolverElemTemps = m_numSolverElemTemps + delta.m_numSolverElemTemps;
}

inline void hkpConstraintInfoSpu2::sub( const hkpConstraintInfo& delta)
{
	m_sizeOfSchemas      = m_sizeOfSchemas - delta.m_sizeOfSchemas;
	m_numSolverResults   = m_numSolverResults - delta.m_numSolverResults;
	m_numSolverElemTemps = m_numSolverElemTemps - delta.m_numSolverElemTemps;
}

inline hkpConstraintInfo& hkpConstraintInfo::operator= (const struct hkpConstraintInfoSpu2& s)
{
	m_maxSizeOfSchema = s.m_maxSizeOfSchema;
	m_sizeOfSchemas = s.m_sizeOfSchemas;
	m_numSolverResults = s.m_numSolverResults;
	m_numSolverElemTemps = s.m_numSolverElemTemps;
	return *this;
}

#endif


#endif // HK_DYNAMICS2_CONSTRAINT_DATA_H

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
