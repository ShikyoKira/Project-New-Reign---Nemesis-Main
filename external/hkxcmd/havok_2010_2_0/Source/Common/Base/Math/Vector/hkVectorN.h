/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_VECTOR_N_H
#define HK_MATH_VECTOR_N_H

/// A column vector of any size.
class hkVectorN
{
	friend class hkMatrixNm;
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkVectorN);

		enum
		{
			IS_ALLOCATED = 0x80000000,
		};

			/// Get elements (read only)
		const hkReal* getElements() const { return (const hkReal*)(m_elements); }
			/// Get elements (read/write)
		hkReal* getElements() { return (hkReal*)m_elements; }

			/// [] access
		hkReal& operator[] (int i) { HK_ASSERT(0x3244aa23, i >= 0 && i < m_size); return ((hkReal*)m_elements)[i]; }
			/// []
		const hkReal& operator[] (int i) const { HK_ASSERT(0x424234aa, i >= 0 && i < m_size); return ((const hkReal*)m_elements)[i]; }
			
			/// Set to a - b
		void setSub(const hkVectorN& a, const hkVectorN& b);
			/// Set to a + b
		void setAdd(const hkVectorN& a, const hkVectorN& b);

			/// Add rhs to this (must be same size)
		void add(const hkVectorN& rhs);
			/// Sub rhs from this (must be same size)
		void sub(const hkVectorN& rhs);

			/// Set to v * s
		void setMul(hkSimdRealParameter s, const hkVectorN& v);

			/// Multiply by a value
		void mul(hkSimdRealParameter v);

			/// Dot product between this and rhs (must be same size)
		hkSimdReal dot(const hkVectorN& rhs) const;

			/// Does equality test - all values must be less than threshold
		hkBool equals(const hkVectorN& rhs, hkReal epsilon = 1e-5f) const;

			/// Tests if equal based on strided input
		hkBool equalsStrided(const hkReal* ele, int size, int stride, hkReal epsilon = 1e-5f) const;

			/// Get the length squared
		hkSimdReal lengthSquared() const;
			/// Get the length
		hkSimdReal length() const;
			/// Normalize, returns the original length
			/// If the length is close to zero, sets to 1,0,0,0...
		hkSimdReal normalize();

			/// Do a horizontal add
		hkSimdReal horizontalAdd() const;

			/// Sets the contents
		void set(const hkReal* v, int size);

			/// Sets the size, the contents values are undefined
		HK_FORCE_INLINE void setSize(int size);
			/// Sets the size and zeros the content
		void setSizeAndZero(int size);

			/// Sets all of the content to zero
		void setZero();

			/// Negates in place
		void negate();

			/// Returns true if the vector is ok
		hkBool isOk() const;

			/// Ctor
		hkVectorN(int size);
			/// Ctor
		hkVectorN(const hkReal* v, int size);
			/// Default Ctor
		hkVectorN(): m_elements(HK_NULL), m_size(0), m_flags(0) {}

			/// Copy Ctor
		hkVectorN(const hkVectorN& rhs);
			/// Assignment
		void operator=(const hkVectorN& rhs);

			/// Gets the amount of elements
		HK_FORCE_INLINE int getSize() const { return m_size; }

			/// Get the number of vectors
		HK_FORCE_INLINE int getNumVectors() const { return (m_size + 3) >> 2; }
			/// Returns true if its allocated
		HK_FORCE_INLINE hkBool isAllocated() const { return (m_flags & IS_ALLOCATED) != 0; }

			/// Get the elements
		HK_FORCE_INLINE const hkVector4* getVectors() const { return m_elements; }
			/// Get the elements
		HK_FORCE_INLINE hkVector4* getVectors() { return m_elements; }

			/// Uses the vecs, as passed in. The memory pointed to by vecs, must stay in scope as log as the vector
			/// Assumes the end of vecs has been zeroed correctly
		void alias(hkVector4* vecs, int size);
			/// Stops aliasing (by making an internal copy)
		void unalias();

			/// For the plane defined by the points, a, b, c. This works out 2 unit length perpendicular axies in the plane.
			/// This can be used to work out the squared distance from the plane
		static void HK_CALL calcUnitAxes(const hkVectorN& a, const hkVectorN& b, const hkVectorN& c, hkVectorN& e1, hkVectorN& e2);

			/// Works out the squared distance of point p, from a plane described by 2 perpendicular axes in the plane and a point on the plane
		static hkSimdReal HK_CALL calcPlaneDistanceSquared(const hkVectorN& p, hkVectorN& e1, hkVectorN& e2, const hkVectorN& planePoint);

			/// Dtor
		HK_FORCE_INLINE ~hkVectorN();

#ifdef HK_DEBUG
		static void HK_CALL selfTest();
#endif

    protected:

		hkVectorN(hkVector4* vecs, int size):m_elements(vecs), m_size(size), m_flags(0) {}

		        /// Sets the values (zeros end values if necessary).
		static void _setValues(hkVector4* dst, const hkReal* v, int size);
			/// Set the size
		void _setSize(int size);
		void _setSizeAndZeroLast(int size);

		// For clearing out the end bits
		static const hkVector4Comparison::Mask s_zeroMask[4];


		hkVector4* m_elements;
		int m_size;
		int m_flags;
};

HK_FORCE_INLINE hkVectorN::~hkVectorN()
{
    if (m_flags & IS_ALLOCATED)
    {
        hkDeallocateChunk<hkVector4>( m_elements, getNumVectors(), HK_MEMORY_CLASS_MATH);
    }
}

HK_FORCE_INLINE void hkVectorN::setSize(int size)
{
    if (size != m_size)
    {
        _setSizeAndZeroLast(size);
    }
}


template <int SIZE>
class hkInplaceVectorN: public hkVectorN
{
	public:
		enum { NUM_VECS = (SIZE + 3) >> 2 };

		hkInplaceVectorN():
			hkVectorN(m_vecs, SIZE)
		{
			m_vecs[NUM_VECS - 1].setZero4();
		}
	protected:
		hkVector4 m_vecs[NUM_VECS];
};

#endif //HK_MATH_VECTOR_N_H

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
