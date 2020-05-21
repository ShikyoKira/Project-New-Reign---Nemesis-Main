/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_MATRIX_NM_H
#define HK_MATH_MATRIX_NM_H

class hkVectorN;

/// A 2d matrix of any size
/// Data is stored in column order in memory
class hkMatrixNm
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkMatrixNm);

			/// Returns true if the matrix is square
		hkBool isSquare() const { return m_numCols > 0 && m_numCols == m_numRows; }

			/// Set to the identity (only possible on square matrix)
		void setIdentity();

			/// Returns true if is the identity
		hkBool isIdentity(hkReal epsilon = 1e-5f) const;

			/// Set the size
		void setSize(int numRows, int numCols);

			/// Returns if the matrix is ok
		hkBool isOk() const;

			/// Returns true if the matrix is symmetric (must be square, and its transpose equal itself)
		hkBool isSymmetric(hkReal threshold = 1e-5f) const;

			/// Set from a column ordered array
		void set( int numRows, int numCols, const hkReal* v);

			/// Assignment
		void operator=(const hkMatrixNm& rhs) { m_numCols = rhs.m_numCols; m_numRows = rhs.m_numRows; m_elements = rhs.m_elements; }

			/// Set the transpose
		void setTranspose(const hkMatrixNm& rhs);

			/// Accumulate another matrix. Must be the same size
		void add(const hkMatrixNm& rhs);
			/// Set to a and b added
		void setAdd(const hkMatrixNm& a, const hkMatrixNm& b);

			/// Subtract rhs from this. Must be the same size
		void sub(const hkMatrixNm& rhs);
			/// Set to a - b
		void setSub(const hkMatrixNm& a, const hkMatrixNm& b);

			/// Returns true if they are equal
		hkBool equals(const hkMatrixNm& rhs, const hkReal threshold = 1e-5f) const;


			/// Multiply all components by v
		void mul(hkSimdRealParameter v);

			/// Get the column
		void getColumn(int col, hkVectorN& colOut) const;
			/// Get the row
		void getRow(int row, hkVectorN& rowOut) const;
			/// Get an alias to the actual columnin the matrix - altering the vector will alter the matrix, and the vector
			/// will only be valid whilst the matrix is in scope, and its size does not change
		void getColumnAlias(int row, hkVectorN& rowOut);

			/// Set as the outer product of a and b
		void setOuterProduct(const hkVectorN& a, const hkVectorN& b);

			/// Get the number of rows
		HK_FORCE_INLINE int getNumRows() const { return m_numRows; }
			/// Get the number of columns
		HK_FORCE_INLINE int getNumColumns() const { return m_numCols; }

			/// Get the vector stride between columns
		HK_FORCE_INLINE int getVectorStride() const { return (m_numRows + 3) >> 2; }
			/// Get the float stride between columns
		HK_FORCE_INLINE int getRealStride() const { return ((m_numRows + 3) >> 2) * int(sizeof(hkReal)); }

			/// Get the elements as reals. They are stored in column order, of realStride reals between columns
		HK_FORCE_INLINE hkReal* getRealElements() { return (hkReal*)m_elements.begin(); }
			/// Get the elements as reals. They are stored in column order, of realStride reals between columns
		HK_FORCE_INLINE const hkReal* getRealElements() const { return (const hkReal*)m_elements.begin(); }

			/// Element access
		hkReal& operator() ( int row, int col) { HK_ASSERT(0x23423424, col >= 0 && col < m_numCols && row >= 0 && row < m_numRows); return ((hkReal*)m_elements.begin())[col * getRealStride() + row]; }
			/// Element access
		const hkReal& operator() (int row, int col) const { HK_ASSERT(0x2423a432, col >= 0 && col < m_numCols && row >= 0 && row < m_numRows); return ((const hkReal*)m_elements.begin())[col * getRealStride() + row]; }

			/// Get the vectors
		HK_FORCE_INLINE hkVector4* getVectors() { return m_elements.begin(); }
		HK_FORCE_INLINE const hkVector4* getVectors() const { return m_elements.begin(); }

			/// Multiply the matrix with the column vector 'in'. Store in the column vector 'out'.
		void multiply(const hkVectorN& in, hkVectorN& out) const;

			/// Invert the matrix in-place using Gaussian elimination. Searches for maximum pivots to
			/// improve accuracy.
			/// If matrix is singular (not invertible) will return HK_FAILURE.
		hkResult invert();

			/// Set to a multiplied by b
		void setMul(const hkMatrixNm& a, const hkMatrixNm& b);
			/// Multiply on FPU
		void setFpuMul(const hkMatrixNm& a, const hkMatrixNm& b);

			/// Ctor
		hkMatrixNm(int numRows, int numCols) { _setSize(numRows, numCols); }
			/// Copy ctor
		hkMatrixNm(const hkMatrixNm& rhs);
			/// Default Ctor
		hkMatrixNm() { m_numCols = 0; m_numRows = 0;}
			/// Construct from data values. Values are presented in column order
		hkMatrixNm(int numRows, int numCols, const hkReal* values);

#ifdef HK_DEBUG
		static void HK_CALL selfTest();
#endif

	protected:
		
		static const hkVector4Comparison::Mask s_zeroMask[4];

		void _setSize(int numRows, int numCols);
		static void _setValues( int numRows, int numCols, const hkReal* v, hkVector4* dst);

		// Data is held in column order. Each row is rounded up to 4 elements in size
		hkArray<hkVector4> m_elements;
		int m_numRows;
		int m_numCols;


};

#endif //HK_MATH_MATRIX_N_M_H

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
