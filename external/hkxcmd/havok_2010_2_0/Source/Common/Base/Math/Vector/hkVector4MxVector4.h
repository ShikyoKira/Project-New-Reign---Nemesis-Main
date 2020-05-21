/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//HK_REFLECTION_PARSER_EXCLUDE_FILE

#ifndef HK_VECTOR4_MX_VECTOR4_H
#define HK_VECTOR4_MX_VECTOR4_H

class hkMxReal;
class hkMxSingle;

// 4x4 vectors internal layout abcd efgh ijkl mnop
struct hkQuadVec
{
	//+hk.MemoryTracker(ignore=True)
	hkVector4 m_vec[4];
};

class hkMxMask4
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkMxMask4);

		typedef hkVector4Comparison::Mask Mask;

		HK_FORCE_INLINE void horizontalAnd4( hkVector4Comparison& andOut ) const;
		HK_FORCE_INLINE void set( hkVector4ComparisonParameter c );
		HK_FORCE_INLINE void set( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b, hkVector4ComparisonParameter c, hkVector4ComparisonParameter d );

		template <int I> HK_FORCE_INLINE const hkVector4Comparison&	get() const;

		//HK_FORCE_INLINE void storeHorizontal( hkMaskStorage& out ) const;

		HK_FORCE_INLINE void setAnd(hkMxMask4Parameter a, hkMxMask4Parameter b);

		HK_FORCE_INLINE hkBool32 anyIsSet() const;

	private:

		hkVector4Comparison m_comp[4];
};

// 1x4 vectors internal layout aaaa eeee iiii mmmm
class hkMxReal
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkMxReal);


	/// broadcast xxxx yyyy zzzz wwww
	HK_FORCE_INLINE explicit hkMxReal(hkVector4Parameter v); 

	HK_FORCE_INLINE hkMxReal(){}

	//HK_FORCE_INLINE operator const hkMxVector4&() const;	/// this operator produces bad code on spu, use explicit hkMxVector4() instead

	/// load a single real value
	HK_FORCE_INLINE void load(const hkReal* r);     // read linear as abcd
	HK_FORCE_INLINE void store(hkReal* rOut) const; // vOut =  abcd

	HK_FORCE_INLINE void gather(const hkReal* base, hkUint32 byteAddressIncrement); // read non-linear as abcd
	HK_FORCE_INLINE void gather(const hkReal* base, hkUint32 byteAddressIncrement, const hkUint16* indices);
	HK_FORCE_INLINE void gather(const hkReal* base, hkUint32 byteAddressIncrement, const hkInt32* indices);

	HK_FORCE_INLINE void scatter(hkReal* base, hkUint32 byteAddressIncrement) const; // write non-linear as abcd
	HK_FORCE_INLINE void scatter(hkReal* base, hkUint32 byteAddressIncrement, const hkUint16* indices) const;
	HK_FORCE_INLINE void scatter(hkReal* base, hkUint32 byteAddressIncrement, const hkInt32* indices) const;

	HK_FORCE_INLINE void compareLessThan(hkMxRealParameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareLessThan(hkMxVector4Parameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareLessThan(hkMxSingleParameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareLessThanEqual(hkMxRealParameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareLessThanEqual(hkMxVector4Parameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareLessThanEqual(hkMxSingleParameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareGreaterThan(hkMxRealParameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareGreaterThan(hkMxVector4Parameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareGreaterThan(hkMxSingleParameter v, hkMxMask4& mask) const;

	HK_FORCE_INLINE void select(hkMxMask4Parameter mask, hkMxRealParameter trueValue, hkMxRealParameter falseValue );

	HK_FORCE_INLINE void add(hkMxRealParameter v);
	HK_FORCE_INLINE void sub(hkMxRealParameter v);
	HK_FORCE_INLINE void mul(hkMxRealParameter v);

	HK_FORCE_INLINE void setAdd(hkMxRealParameter v0, hkMxRealParameter v1);
	HK_FORCE_INLINE void setSub(hkMxRealParameter v0, hkMxRealParameter v1);
	HK_FORCE_INLINE void setMul(hkMxRealParameter v0, hkMxRealParameter v1);

	HK_FORCE_INLINE void setMax(hkMxRealParameter v0, hkMxRealParameter v1);
	HK_FORCE_INLINE void setMin(hkMxRealParameter v0, hkMxRealParameter v1);

	HK_FORCE_INLINE void setNeg(hkMxRealParameter v);

	HK_FORCE_INLINE void setDot3(hkMxVector4Parameter v0, hkMxVector4Parameter v1);
	HK_FORCE_INLINE void setDot3(hkMxSingleParameter s, hkMxVector4Parameter v);
	HK_FORCE_INLINE void setDot4xyz1(hkMxVector4Parameter v0, hkMxVector4Parameter v1);
	HK_FORCE_INLINE void setDot4xyz1(hkMxSingleParameter s, hkMxVector4Parameter v);

	template <int I> HK_FORCE_INLINE void getBroadcast(hkVector4& vOut) const; // vOut = [i=0]aaaa [i=1]eeee [i=2]iiii [i=3]mmmm
	template <int I> HK_FORCE_INLINE hkVector4 getBroadcast() const;			 // return [i=0]aaaa [i=1]eeee [i=2]iiii [i=3]mmmm
	template <int I> HK_FORCE_INLINE void setBroadcast(hkReal r);
	HK_FORCE_INLINE void storePacked(hkVector4& vOut) const; // vOut = aeim

private:

	hkQuadVec m_real;
	friend class hkMxVector4;
};

// 4x1 vectors internal layout abcd abcd abcd abcd
class hkMxSingle
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkMxSingle);

	HK_FORCE_INLINE explicit hkMxSingle() {}
	HK_FORCE_INLINE explicit hkMxSingle(hkSimdRealParameter v);
	HK_FORCE_INLINE explicit hkMxSingle(hkVector4Parameter v);
	HK_FORCE_INLINE explicit hkMxSingle(const hkQuadReal& q);

	HK_FORCE_INLINE void getVector(hkVector4& out);
	HK_FORCE_INLINE void setAdd(hkMxSingleParameter v0, hkMxSingleParameter v1);

public:

	hkVector4 m_single;
	friend class hkMxVector4;
	friend class hkMxReal;
	friend class hkMxQuaternion;
};

extern const hkMxSingle hkMxSingle0000;
extern const hkMxSingle hkMxSingle1111;
extern const hkMxSingle hkMxSingleMinus1111;
extern const hkMxSingle hkMxSingle1000;
extern const hkMxSingle hkMxSingle0100;
extern const hkMxSingle hkMxSingle0010;
extern const hkMxSingle hkMxSingle0001;
extern const hkMxSingle hkMxSingle3333;
extern const hkMxSingle hkMxSingleHalf;
extern const hkMxSingle hkMxSingleMinusHalf;
extern const hkMxSingle hkMxSingleMax;
extern const hkMxSingle hkMxSingleEps;
extern const hkMxSingle hkMxSingleMinusEps;


// 8x4 low precision vectors internal layout interleaved abcdABCD efghEFGH ijklIJKL mnopMNOP
// with lower/uppercase belonging together
class hkMxHalf8
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkMxHalf8);

		HK_FORCE_INLINE void pack(hkMxVector4Parameter v0, hkMxVector4Parameter v1 ); // reduce precision and interleave

		HK_FORCE_INLINE void unpack(hkMxVector4& v0, hkMxVector4& v1) const; // expand and sort into two full precision vectors
		HK_FORCE_INLINE void unpackFirst(hkMxVector4& v) const;
		HK_FORCE_INLINE void unpackSecond(hkMxVector4& v) const;

		HK_FORCE_INLINE void setZero();

		/// reduce precision to mimic the reduced accuracy
		static HK_FORCE_INLINE void reducePrecision( hkMxVector4& v ); 
	protected:

		HK_ALIGN16( hkHalf8 m_vec[4] );
};


#endif //HK_VECTOR4_MX_VECTOR4_H

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
