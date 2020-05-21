/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_QUADRIC_METRIC_H
#define HK_QUADRIC_METRIC_H

#include <Common/Base/Math/Vector/hkVectorN.h>
#include <Common/Base/Math/Matrix/hkMatrixNm.h>


class hkQuadricMetric
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, hkQuadricMetric);

			/// Calc the a matrix from the e1, e2 unit length perp vectors in the plane
		static void HK_CALL calcA(const hkVectorN& e1, const hkVectorN& e2, hkMatrixNm& a);

			/// Set from a plane
		void setFromPlane(const hkVectorN& a, const hkVectorN& b, const hkVectorN& c);
			/// Vertices defined consecutively
		void setFromPlane(const hkReal* values, int size);

			/// Calc the distance squared from this
		hkSimdReal calcDistanceSquared(const hkVectorN& a) const;
			/// Calc the distance from
		hkSimdReal calcDistanceSquared(hkVector4Parameter a) const;
		
			/// For a given dimension works out the size in hkReals to store (using store method for example)
			/// size * (size + 1) / 2 = amount of elements in the symmetric matrix that need to be stored
		HK_FORCE_INLINE static int HK_CALL calcStoreSize(int dim) { return (dim * (dim + 1)) / 2 + dim + 1; }
			/// Get the size (in reals to store this)
		int getStoreSize() const;
		
		
			/// Gets the size
		HK_FORCE_INLINE int getSize() const { return m_b.getSize(); }

			/// Returns true if the metric appears valid
		hkBool isOk() const;

			/// Store
		void store(hkReal* data) const;
			/// Load
		void load(int size, const hkReal* load);

			/// Add to this quadric metric another one
		void add(const hkQuadricMetric& rhs) { m_a.add(rhs.m_a); m_b.add(rhs.m_b); m_c += rhs.m_c; }
			/// Set the result of adding a and b
		void setAdd(const hkQuadricMetric& a, const hkQuadricMetric& b) { m_a.setAdd(a.m_a, b.m_a); m_b.setAdd(a.m_b, b.m_b); m_c = a.m_c + b.m_c; }

			/// Returns true if they are equal
		hkBool equals(const hkQuadricMetric& rhs, hkReal threshold = 1e-5f);
		
#ifdef HK_DEBUG
		static void HK_CALL selfTest();
#endif

			/// Note the matrix is symmetric
		hkMatrixNm m_a;
		hkVectorN m_b;
		hkReal m_c;
};


#endif // HK_QUADRIC_METRIC_H

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
