/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_INTVECTOR_H
#define HK_MATH_INTVECTOR_H

#ifndef HK_MATH_MATH_H
#	error Please include Common/Base/hkBase.h instead of this file.
#endif

class hkIntVector;

// Defined only if the processor has a native vperm instruction (i.e. Xbox 360, PlayStation(R)3, SSE3)

// <ce.todo> Move to hkXxxMathTypes.inl
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED) && defined(HK_COMPILER_HAS_INTRINSICS_IA32)
	// SSE2 header
#	include <emmintrin.h>	
	typedef __m128i hkQuadUint;
	typedef const hkIntVector& hkIntVectorParameter;
	typedef int hkPermuteMask;
//XXTODO
//#elif (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED) && defined(HK_COMPILER_HAS_INTRINSICS_NEON)
//	// ARM Neon
//	#	include <arm_neon.h>	
//	typedef int32x4_t hkQuadUint;
//	typedef const hkIntVector& hkIntVectorParameter;
//	typedef int hkPermuteMask;
#else
	struct hkQuadUint
	{
		//+hk.MemoryTracker(ignore=True)
		HK_ALIGN16(hkUint32 u32[4]);
	};
	typedef const hkIntVector& hkIntVectorParameter;
	typedef int hkPermuteMask;
#endif

// handy for fallback implementations
union hkQuadUintUnion
{
	hkUint32 u[4];
	hkQuadUint q;
};

union hkQuadIntUnion
{
	hkInt32 u[4];
	hkQuadUint q;
};

union hkQuadShortUnion
{
	hkInt16 u[8];
	hkQuadUint q;
};

union hkQuadUshortUnion
{
	hkUint16 u[8];
	hkQuadUint q;
};

union hkQuadCharUnion
{
	hkUint8 u[16];
	hkQuadUint q;
};

union hkQuadUcharUnion
{
	hkUint8 u[16];
	hkQuadUint q;
};

/// Stores four hkUint32 in a SIMD friendly format.
class hkIntVector
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkIntVector);

			/// Clear
		HK_FORCE_INLINE void setZero();

		HK_FORCE_INLINE void setAll(int i);

		HK_FORCE_INLINE void set(int i);

		HK_FORCE_INLINE int get() const;

			/// Load from 16 byte aligned address.
		HK_FORCE_INLINE void loadAligned(const hkUint32* p);

			/// Load up to 16 bytes till the next 16 bytes alignment. The address must be 4 bytes aligned.
		HK_FORCE_INLINE void loadAligned1x(const hkUint32* p);

			/// Load from an unaligned address.
			
		HK_FORCE_INLINE void loadUnaligned4(const hkUint32* p);

			/// Store to an aligned address
		HK_FORCE_INLINE void store(void* p) const; // not yet implemented

			/// Store to an unaligned address
		HK_FORCE_INLINE void storeUnaligned4(void* p);

		template<int idx> HK_FORCE_INLINE void setBroadcast(hkIntVectorParameter v);

		const hkQuadUint& getQuad() const;
		hkQuadUint& getQuad();
		
		
			// Logical Ops
		HK_FORCE_INLINE void setNot( hkIntVectorParameter a );
		HK_FORCE_INLINE void setOr( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setAnd( hkIntVectorParameter a, hkIntVectorParameter b ); 
		HK_FORCE_INLINE void setXor( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setAndC( hkIntVectorParameter a, hkIntVectorParameter b );

			// Comparison.

			/// Returns != 0 if the value is negative. This function assumes that x=y=z=w.
		HK_FORCE_INLINE hkBool32 isNegativeAssumingAllValuesEqual(  ) const;

		HK_FORCE_INLINE hkVector4Comparison compareLessThanS32( hkIntVectorParameter a, hkIntVectorParameter b );

		// Will implement these as needed. There are potentially a LOT of functions to implement here
		//HK_FORCE_INLINE void setCompareEqual(32/16/8)( hkIntVectorParameter a, hkIntVectorParameter b );
		//HK_FORCE_INLINE void setCompareLessThan(S/U)(32/16/8)( hkIntVectorParameter a, hkIntVectorParameter b );
		//HK_FORCE_INLINE void setCompareLessThanEqual(S/U)(32/16/8)( hkIntVectorParameter a, hkIntVectorParameter b );
		//HK_FORCE_INLINE void setCompareGreaterThan(S/U)(32/16/8)( hkIntVectorParameter a, hkIntVectorParameter b );
		//HK_FORCE_INLINE void setCompareGreaterThanEqual(S/U)(32/16/8)( hkIntVectorParameter a, hkIntVectorParameter b );
			// Need to convert between comparison results and hkVector4Comparison masks easily

			// Integer arithmetic
		HK_FORCE_INLINE void setSubU16( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setAddU16( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setSubU32( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setAddU32( hkIntVectorParameter a, hkIntVectorParameter b );
		// Will implement these as needed. 
		//HK_FORCE_INLINE void setAddSaturate(S/U)(32/16/8)( hkIntVectorParameter a, hkIntVectorParameter b ) 
		//HK_FORCE_INLINE void setSubSaturate(S/U)(32/16/8)( hkIntVectorParameter a, hkIntVectorParameter b ) 
		HK_FORCE_INLINE void setAddSaturateU32( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setAddSaturateS16( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setAddSaturateU16( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setSubSaturateU32( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setSubSaturateS16( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setSubSaturateU16( hkIntVectorParameter a, hkIntVectorParameter b );
		//HK_FORCE_INLINE void setAddS32( hkIntVectorParameter a, hkIntVectorParameter b );
		//HK_FORCE_INLINE void setSubS32( hkIntVectorParameter a, hkIntVectorParameter b );



			// Min / Max
		//HK_FORCE_INLINE void setMin(S/U)(32/16/8)( hkIntVectorParameter a, hkIntVectorParameter b ) 
		HK_FORCE_INLINE void setMinU8( hkIntVectorParameter a, hkIntVectorParameter b ) ;
		//HK_FORCE_INLINE void setMinU16( hkIntVectorParameter a, hkIntVectorParameter b );	// requires sse4
		//HK_FORCE_INLINE void setMaxU16( hkIntVectorParameter a, hkIntVectorParameter b );

			// Logical Rotate / Shift
		//HK_FORCE_INLINE void setShiftLeft(32/16/8)( hkIntVectorParameter a, int shift );
		//HK_FORCE_INLINE void setShiftRight(32/16/8)( hkIntVectorParameter a, int shift );
		//HK_FORCE_INLINE void setRotateLeft(32/16/8)( hkIntVectorParameter a, int shift );
		//HK_FORCE_INLINE void setRotateRight(32/16/8)( hkIntVectorParameter a, int shift );

		template<int shift> HK_FORCE_INLINE void setShiftLeft16( hkIntVectorParameter a );
		template<int shift> HK_FORCE_INLINE void setShiftRight16( hkIntVectorParameter a );
		template<int shift> HK_FORCE_INLINE void setShiftLeft32( hkIntVectorParameter a );
		template<int shift> HK_FORCE_INLINE void setShiftRight32( hkIntVectorParameter a );
		template<int shift> HK_FORCE_INLINE void setShiftLeft128( hkIntVectorParameter a );
		template<int shift> HK_FORCE_INLINE void setShiftRight128( hkIntVectorParameter a );

		HK_FORCE_INLINE void setShiftLeft16( hkIntVectorParameter a, int shift );
		HK_FORCE_INLINE void setShiftRight16( hkIntVectorParameter a, int shift );
		
		HK_FORCE_INLINE void setShiftLeft32( hkIntVectorParameter a, hkIntVectorParameter shift );
		HK_FORCE_INLINE void setShiftRight32( hkIntVectorParameter a, hkIntVectorParameter shift );


		// Merge 

		/// interleave XY components of a and b. Result = { a.X, b.X, a.Y, b.Y }
		HK_FORCE_INLINE void setMergeHead32( hkIntVectorParameter a, hkIntVectorParameter b );

		/// interleave ZW components of a and b. Result = { a.Z, b.Z, a.W, b.W }
		HK_FORCE_INLINE void setMergeTail32( hkIntVectorParameter a, hkIntVectorParameter b );

		/// 16bit interleave the front part of a with the front part of b. Second half of a and b is ignored. Result = { a.u16[0], b.u16[0], a.u16[1], b.u16[1], ... a.u16[3], b.u16[3] }
		HK_FORCE_INLINE void setMergeHead16( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setMergeTail16( hkIntVectorParameter a, hkIntVectorParameter b );
		HK_FORCE_INLINE void setMergeHead8( hkIntVectorParameter a, hkIntVectorParameter b ); 
		HK_FORCE_INLINE void setMergeTail8( hkIntVectorParameter a, hkIntVectorParameter b ); 


		/// result.u32[i] = highshorts.u16[i]<<16 + lowShorts.u16[i]. highShorts.u16/lowShorts.u16[4..7] are ignored
		HK_FORCE_INLINE void setCombineHead16To32( hkIntVectorParameter highShorts, hkIntVectorParameter lowShorts );
		
		/// result.u32[i] = highshorts.u16[4+i]<<16 + lowShorts.u16[4+i]. highShorts.u16/lowShorts.u16[0..3] are ignored
		HK_FORCE_INLINE void setCombineTail16To32( hkIntVectorParameter highShorts, hkIntVectorParameter lowShorts );

		/// result.u16[i] = highshorts.u8[i]<<16 + lowShorts.u8[i]. highShorts.u8/lowShorts.u8[8..15] are ignored
		HK_FORCE_INLINE void setCombineHead8To16( hkIntVectorParameter highShorts, hkIntVectorParameter lowShorts );

		/// result.u16[i] = highshorts.u8[8+i]<<16 + lowShorts.u8[8+i]. highShorts.u8/lowShorts.u8[0..7] are ignored
		HK_FORCE_INLINE void setCombineTail8To16( hkIntVectorParameter highShorts, hkIntVectorParameter lowShorts );


		/// Uncombine (pack). Extract the low shorts from a.u32[] and b.u32[]. result.u16[] = { a.u32[0]&0xffff, a.u32[1]&0xffff, ... b.u32[3]&0xffff }
		HK_FORCE_INLINE void setConvertU32ToU16( hkIntVectorParameter a, hkIntVectorParameter b );

		/// saturate and convert 8 signed 32bit value to 8 unsigned signed shorts.
		HK_FORCE_INLINE void setConvertSaturateS32ToU16( hkIntVectorParameter a, hkIntVectorParameter b );

		/// saturate and convert 16 signed 16bit value to 16 unsigned bytes.
		HK_FORCE_INLINE void setConvertSaturateS16ToU8( hkIntVectorParameter a, hkIntVectorParameter b );


			/// Select, Shuffle
		HK_FORCE_INLINE void setShuffle( hkPermuteMask perm, hkIntVectorParameter b ); // not yet implemented

		template<int A, int B, int C, int D> HK_FORCE_INLINE void setShuffle( hkIntVectorParameter a );

			// XX if this interface is changed, should be consistent to hkVector4::select32. The name is confusing and the order of parameters are not the same. 
			/// (select & trueValue) | (~select & falseValue)
		HK_FORCE_INLINE void setSelect( hkVector4ComparisonParameter select, hkIntVectorParameter trueValue, hkIntVectorParameter falseValue ); 

		HK_FORCE_INLINE void setPermuteU8(hkIntVectorParameter a, hkIntVectorParameter mask);
		HK_FORCE_INLINE void setPermuteU8(hkIntVectorParameter a, hkIntVectorParameter b, hkIntVectorParameter mask);

			// Splat Immediate
		template<int val> HK_FORCE_INLINE void splatImmediate32();
		template<int val> HK_FORCE_INLINE void splatImmediate16();
		template<int val> HK_FORCE_INLINE void splatImmediate8(); 

			// Convert to from fixed point
		HK_FORCE_INLINE void convertU32ToF32( hkVector4& vOut ) const;
		HK_FORCE_INLINE void convertS32ToF32( hkVector4& vOut ) const; 

		HK_FORCE_INLINE void setConvertF32toU32( hkVector4Parameter vIn );
		HK_FORCE_INLINE void setConvertF32toS32( hkVector4Parameter vIn );		

		HK_FORCE_INLINE hkUint8  getU8 (int ielem) const {hkUint8*  ptr = (hkUint8*)&m_quad;  return ptr[ielem];}
		HK_FORCE_INLINE hkUint16 getU16(int ielem) const {hkUint16* ptr = (hkUint16*)&m_quad; return ptr[ielem];}
		HK_FORCE_INLINE hkUint32 getU32(int ielem) const {hkUint32* ptr = (hkUint32*)&m_quad; return ptr[ielem];}
			// Add versions with scaling if needed?

	private:

		hkQuadUint m_quad;

	public:
		operator hkQuadUint() const
		{
			return m_quad;
		}

};

#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
#	if defined(HK_COMPILER_HAS_INTRINSICS_IA32)
#		include <Common/Base/Math/Vector/Sse/hkSseIntVector.inl>
//#	elif defined(HK_COMPILER_HAS_INTRINSICS_NEON)
//#		include <Common/Base/Math/Vector/Neon/hkNeonIntVector.inl> //XXTODO
#	elif defined(HK_PLATFORM_PS3_PPU) || defined(HK_PLATFORM_PS3_SPU)
#		include <Common/Base/Math/Vector/Ps3/hkPs3IntVector.inl>
#	elif defined(HK_PLATFORM_XBOX360)
#		include <Common/Base/Math/Vector/Xbox360/hkXbox360IntVector.inl>
#	else
#		include <Common/Base/Math/Vector/hkIntVector.inl>
#	endif
#else
#	include <Common/Base/Math/Vector/hkIntVector.inl>
#endif

HK_FORCE_INLINE void hkIntVector::setSelect(hkVector4ComparisonParameter selectMask, hkIntVectorParameter trueValue, hkIntVectorParameter falseValue)
{
	((hkVector4*)(this))->select32( *(hkVector4*)(&falseValue), *(hkVector4*)(&trueValue), selectMask  );
}


/// result.u32[i] = highshorts.u16[i]<<16 + lowShorts.u16[i]. highShorts.u16/lowShorts.u16[4..7] are ignored
HK_FORCE_INLINE void hkIntVector::setCombineHead16To32( hkIntVectorParameter highShorts, hkIntVectorParameter lowShorts )
{
	if(HK_ENDIAN_BIG) 	{	setMergeHead16(highShorts, lowShorts); }
	else				{	setMergeHead16(lowShorts, highShorts); }
}

/// result.u32[i] = highshorts.u16[4+i]<<16 + lowShorts.u16[4+i]. highShorts.u16/lowShorts.u16[0..3] are ignored
HK_FORCE_INLINE void hkIntVector::setCombineTail16To32( hkIntVectorParameter highShorts, hkIntVectorParameter lowShorts )
{
	if(HK_ENDIAN_BIG) 	{	setMergeTail16(highShorts, lowShorts); }
	else				{	setMergeTail16(lowShorts, highShorts); }
}

/// result.u16[i] = highshorts.u8[i]<<16 + lowShorts.u8[i]. highShorts.u8/lowShorts.u8[8..15] are ignored
HK_FORCE_INLINE void hkIntVector::setCombineHead8To16( hkIntVectorParameter highShorts, hkIntVectorParameter lowShorts )
{
	if(HK_ENDIAN_BIG) 	{	setMergeHead8(highShorts, lowShorts); }
	else				{	setMergeHead8(lowShorts, highShorts); }
}

/// result.u16[i] = highshorts.u8[8+i]<<16 + lowShorts.u8[8+i]. highShorts.u8/lowShorts.u8[0..7] are ignored
HK_FORCE_INLINE void hkIntVector::setCombineTail8To16( hkIntVectorParameter highShorts, hkIntVectorParameter lowShorts )
{
	if(HK_ENDIAN_BIG) 	{	setMergeTail8(highShorts, lowShorts); }
	else				{	setMergeTail8(lowShorts, highShorts); }
}

#endif //HK_MATH_INTVECTOR_H

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
