/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//HK_REFLECTION_PARSER_EXCLUDE_FILE

#ifndef HK_MX_VECTOR4_H
#define HK_MX_VECTOR4_H

#define hkMxSIZE 4


#	define hkMxVector4Parameter const hkMxVector4&
#	define hkMxRealParameter   const hkMxReal&
#	define hkMxSingleParameter const hkMxSingle&
#	define hkMxMask4Parameter  const hkMxMask4&


#define hkMxQuaternionParameter const hkMxQuaternion&
#define hkMxUNROLL( X ) { {	const int hkMxI = 0; X; } { const int hkMxI = 1; X; } { const int hkMxI = 2; X; } { const int hkMxI = 3; X; } }	
#define hkMxUNROLL3( X ) { {	const int hkMxI = 0; X; } { const int hkMxI = 1; X; } { const int hkMxI = 2; X; } }	

class hkMxVector4;
class hkMxQuaternion;
class hkMxSingle;
class hkMxMask4;

#include <Common/Base/Math/Vector/hkVector4MxVector4.h>

class hkMxVector4Util;
#define hkMxPackedReal hkVector4 

// 4x4 vectors 
class hkMxVector4
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkMxVector4);
//	HK_DECLARE_POD_TYPE();

//#ifndef HK_DISABLE_VECTOR4_CONSTRUCTORS
	/// Empty constructor.
	HK_FORCE_INLINE hkMxVector4() { }
//#endif

	enum Permutation {
		SHIFT_RIGHT_CYCLIC,
		SHIFT_LEFT_CYCLIC,
		REVERSE
	};

	HK_FORCE_INLINE explicit hkMxVector4(hkVector4Parameter v); // replicate abcd abcd abcd abcd
	HK_FORCE_INLINE explicit hkMxVector4(hkReal v);             // broadcast aaaa aaaa aaaa aaaa

	/// reinterpret v as a vector (normally all values of a hkSimdReal are equal and can be trivially casted to a vector)
	HK_FORCE_INLINE explicit hkMxVector4(hkMxRealParameter v);

	HK_FORCE_INLINE void load(const hkVector4* v);     // read linear as abcd efgh ijkl mnop
	HK_FORCE_INLINE void store(hkVector4* vOut) const; // vOut =  abcd efgh ijkl mnop

	template <hkUint32 byteAddressIncrement> HK_FORCE_INLINE void gather(const hkVector4* base); // read non-linear as abcd efgh ijkl mnop
	HK_FORCE_INLINE void gatherSLOW(const hkVector4* base, hkUint32 byteAddressIncrement); // no preprocessed address calc
	HK_FORCE_INLINE void gather(const hkVector4* base, hkUint32 byteAddressIncrement, const hkUint16* indices);
	HK_FORCE_INLINE void gather(const hkVector4* base, hkUint32 byteAddressIncrement, const hkInt32* indices);

	template <hkUint32 byteAddressIncrement> HK_FORCE_INLINE void scatter(hkVector4* base) const; // write non-linear as abcd efgh ijkl mnop
	HK_FORCE_INLINE void scatterSLOW(hkVector4* base, hkUint32 byteAddressIncrement) const; // no preprocessed address calc
	HK_FORCE_INLINE void scatter(hkVector4* base, hkUint32 byteAddressIncrement, const hkUint16* indices) const;
	HK_FORCE_INLINE void scatter(hkVector4* base, hkUint32 byteAddressIncrement, const hkInt32* indices) const;

		/// Transposes the hkMxVector4 and stores it, only works for 4 elements
	HK_FORCE_INLINE void storeTransposed4(hkVector4* matrix4) const;
	

	HK_FORCE_INLINE void set(hkMxVector4Parameter v);
	HK_FORCE_INLINE void setXYZ(hkMxVector4Parameter v);
	HK_FORCE_INLINE void setW(hkMxVector4Parameter v);
	HK_FORCE_INLINE void setW(hkMxSingleParameter s);
	HK_FORCE_INLINE void setXYZW(hkMxVector4Parameter v0, hkMxVector4Parameter v1);

	HK_FORCE_INLINE void setZero();

	HK_FORCE_INLINE void setCross(hkMxVector4Parameter v0, hkMxVector4Parameter v1);
	HK_FORCE_INLINE void setCross(hkMxSingleParameter s, hkMxVector4Parameter v);
	HK_FORCE_INLINE void setCross(hkMxVector4Parameter v, hkMxSingleParameter s);

	HK_FORCE_INLINE void setAdd(hkMxVector4Parameter v0, hkMxVector4Parameter v1);
	HK_FORCE_INLINE void setAdd(hkMxVector4Parameter v, hkMxSingleParameter s);
	HK_FORCE_INLINE void setAddMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, hkMxVector4Parameter v2);
	HK_FORCE_INLINE void setAddMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, hkMxSingleParameter s);
	HK_FORCE_INLINE void setAddMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, const hkMxPackedReal& v2);

	HK_FORCE_INLINE void setSub(hkMxVector4Parameter v0, hkMxVector4Parameter v1);
	HK_FORCE_INLINE void setSub(hkMxVector4Parameter v, hkMxSingleParameter s);
	HK_FORCE_INLINE void setSubMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, hkMxVector4Parameter v2);
	HK_FORCE_INLINE void setSubMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, hkMxSingleParameter s);

	HK_FORCE_INLINE void setDiv(hkMxVector4Parameter v0, hkMxVector4Parameter v1);
	HK_FORCE_INLINE void setDivFast(hkMxVector4Parameter v0, hkMxVector4Parameter v1);

	HK_FORCE_INLINE void setSqrtInverse( hkMxVector4Parameter m );

	HK_FORCE_INLINE void setMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1);
	HK_FORCE_INLINE void setMul(hkMxRealParameter r, hkMxVector4Parameter v);
	HK_FORCE_INLINE void setMul(hkMxVector4Parameter v, hkMxSingleParameter s);

	template <Permutation P> HK_FORCE_INLINE void setPermutation(hkMxVector4Parameter m);
	template <int I> HK_FORCE_INLINE void setBroadcast(hkMxVector4Parameter v); // take the i component from every vector and broadcast within this vector only

	HK_FORCE_INLINE void add(hkMxVector4Parameter m);
	HK_FORCE_INLINE void add(hkMxSingleParameter s);

	HK_FORCE_INLINE void addMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1);
	HK_FORCE_INLINE void addMul(hkMxRealParameter r, hkMxVector4Parameter v);
	HK_FORCE_INLINE void addMul(hkMxVector4Parameter v, hkMxSingleParameter s);

	HK_FORCE_INLINE void sub(hkMxVector4Parameter m);
	HK_FORCE_INLINE void sub(hkMxSingleParameter s);

	HK_FORCE_INLINE void mul(hkMxVector4Parameter m);
	HK_FORCE_INLINE void mul(hkMxRealParameter r);
	HK_FORCE_INLINE void mul(hkMxSingleParameter s);

	// these are for storage
	template <int I> HK_FORCE_INLINE void getVector(hkVector4& vOut) const; // vOut = [i=0]abcd [i=1]efgh [i=2]ijkl [i=3]mnop
	template <int I> HK_FORCE_INLINE hkVector4 getVector() const;				// return [i=0]abcd [i=1]efgh [i=2]ijkl [i=3]mnop
	template <int I> HK_FORCE_INLINE void setVector(hkVector4Parameter v);

	// this is for calc and should actual become getVector
	template <int I> HK_FORCE_INLINE void getSingle(hkMxSingle& vOut) const; 

	HK_FORCE_INLINE void setVectorSLOW(int i, hkVector4Parameter v); // temp, needs to go
	HK_FORCE_INLINE void getVectorSLOW(int i, hkVector4& vOut) const;

	HK_FORCE_INLINE void compareGreaterThan(hkMxVector4Parameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareGreaterThan(hkMxSingleParameter s, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareLessThan(hkMxVector4Parameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareLessThan(hkMxSingleParameter s, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareLessThanEqual(hkMxVector4Parameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareEqual(hkMxVector4Parameter v, hkMxMask4& mask) const;
	HK_FORCE_INLINE void compareEqual(hkMxSingleParameter s, hkMxMask4& mask) const;

	HK_FORCE_INLINE void select(hkMxMask4Parameter mask, hkMxVector4Parameter trueValue, hkMxVector4Parameter falseValue );
	HK_FORCE_INLINE void select(hkMxMask4Parameter mask, hkMxSingleParameter  trueValue, hkMxVector4Parameter falseValue );
	HK_FORCE_INLINE void select(hkMxMask4Parameter mask, hkMxVector4Parameter trueValue, hkMxSingleParameter falseValue  );

	// change to storage pipeline
	HK_FORCE_INLINE void horizontalMin4Packed( hkVector4& minsOut ) const; // minsOut = xyzw with x=min(abcd) y=min(efgh) z=min(ijkl) w=min(mnop)
	HK_FORCE_INLINE void horizontalMax4Packed( hkVector4& maxsOut ) const;
	HK_FORCE_INLINE void horizontalAdd4( hkVector4& addsOut ) const;
	HK_FORCE_INLINE void horizontalAdd3( hkVector4& addsOut ) const;

	// stay in calc pipeline
	HK_FORCE_INLINE void horizontalMin4Packed( hkMxReal& minsOut ) const; // minsOut = xyzw with x=min(abcd) y=min(efgh) z=min(ijkl) w=min(mnop)
	HK_FORCE_INLINE void horizontalMax4Packed( hkMxReal& maxsOut ) const;

	HK_FORCE_INLINE void reduceAdd( hkVector4& addOut ) const;

	HK_FORCE_INLINE void normalize4(); // normalize every component
	HK_FORCE_INLINE void length4( hkMxReal& lensOut ) const;
	HK_FORCE_INLINE void lengthInverse4( hkMxReal& lensOut ) const;
	HK_FORCE_INLINE void dot4(hkMxVector4Parameter v, hkMxReal& lensOut ) const;

	HK_FORCE_INLINE void dot3(hkMxVector4Parameter v, hkMxReal& lensOut ) const;

	HK_FORCE_INLINE void dot3(hkMxVector4Parameter v, hkMxSingle& lensOut ) const;

	HK_FORCE_INLINE void setFastNormalize3NonZero(hkMxVector4Parameter m);


private:

	void operator=( const hkMxVector4& rhs);		// results in bad compiler output, use load instead
	hkMxVector4( const hkMxVector4& other);	// results in bad compiler output, use load instead

protected:

	hkQuadVec m_vec;
	friend class hkMxReal;
	friend class hkMxHalf8;
	friend class hkMxQuaternion;
	friend class hkMxVector4Util;
};

class hkMxTransform: public hkMxVector4
{

};


class hkMxVector4Util
{
	public:
		HK_FORCE_INLINE static void HK_CALL transformPosition( const hkMxTransform& mat, hkMxVector4Parameter vIn, hkMxVector4& vOut);
		HK_FORCE_INLINE static void HK_CALL transformTransposePosition(const hkMxTransform& mat, hkMxVector4Parameter vIn, hkMxVector4& vOut);

		HK_FORCE_INLINE static void HK_CALL rotateDir( const hkMxTransform& mat, hkMxVector4Parameter vIn, hkMxVector4& vOut);
		HK_FORCE_INLINE static void HK_CALL rotateInverseDir( const hkMxTransform& mat, hkMxVector4Parameter vIn, hkMxVector4& vOut);

		// these two need to go - vector register spill - unroll instead
		HK_FORCE_INLINE static void HK_CALL transformPositions(hkMxVector4Parameter mat0, 
			hkMxVector4Parameter mat1, 
			hkMxVector4Parameter mat2, 
			hkMxVector4Parameter mat3,
			hkMxVector4Parameter vIn,
			hkMxVector4& vOut);

		HK_FORCE_INLINE static void HK_CALL rotateDirs(hkMxVector4Parameter mat0, 
			hkMxVector4Parameter mat1, 
			hkMxVector4Parameter mat2, 
			hkMxVector4Parameter mat3,
			hkMxVector4Parameter vIn,
			hkMxVector4& vOut);

		template <hkUint32 byteAddressIncrement>
		HK_FORCE_INLINE static void HK_CALL gatherUnpack(const hkHalf8* base, hkMxVector4& vOutFirst, hkMxVector4& vOutSecond);

		template <hkUint32 byteAddressIncrement>
		HK_FORCE_INLINE static void HK_CALL gatherUnpackFirst(const hkHalf8* base, hkMxVector4& vOut);
		HK_FORCE_INLINE static void HK_CALL gatherUnpackFirstSLOW(const hkHalf8* base, hkUint32 byteAddressIncrement, hkMxVector4& vOut);
		HK_FORCE_INLINE static void HK_CALL gatherUnpackFirst(const hkHalf8* base, hkUint32 byteAddressIncrement, const hkInt32* indices, hkMxVector4& vOut);

		template <hkUint32 byteAddressIncrement>
		HK_FORCE_INLINE static void HK_CALL gatherUnpackSecond(const hkHalf8* base, hkMxVector4& vOut);
		HK_FORCE_INLINE static void HK_CALL gatherUnpackSecondSLOW(const hkHalf8* base, hkUint32 byteAddressIncrement, hkMxVector4& vOut);
		HK_FORCE_INLINE static void HK_CALL gatherUnpackSecond(const hkHalf8* base, hkUint32 byteAddressIncrement, const hkInt32* indices, hkMxVector4& vOut);

		template <hkUint32 byteAddressIncrement>
		HK_FORCE_INLINE static void HK_CALL scatterPack(hkMxVector4Parameter vFirst, hkMxVector4Parameter vSecond, hkHalf8* base);

		template <hkUint32 byteAddressIncrement>
		HK_FORCE_INLINE static void HK_CALL scatterPackFirst(hkMxVector4Parameter vFirst, hkHalf8* base);

		HK_FORCE_INLINE static void HK_CALL scatterPackFirst(hkMxVector4Parameter v, hkHalf8* base, hkUint32 byteAddressIncrement, const hkInt32* indices);
};


class hkMxQuaternion: public hkMxVector4
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkMxQuaternion);
		//	HK_DECLARE_POD_TYPE();

		HK_FORCE_INLINE void setMulQ(const hkMxQuaternion& p, const hkMxQuaternion& q);
		HK_FORCE_INLINE void setMulQ(const hkMxQuaternion& p, hkMxSingleParameter s);

		template <hkUint32 byteAddressIncrement>
		HK_FORCE_INLINE void gather(const hkQuaternion* base); // read non-linear as abcd efgh ijkl mnop
		HK_FORCE_INLINE void gatherSLOW(const hkQuaternion* base, hkUint32 byteAddressIncrement); // no preprocessed address calc

		template <hkUint32 byteAddressIncrement>
		HK_FORCE_INLINE void scatter(hkQuaternion* base) const; // write non-linear as abcd efgh ijkl mnop
		HK_FORCE_INLINE void scatterSLOW(hkQuaternion* base, hkUint32 byteAddressIncrement) const; // no preprocessed address calc

		template <hkUint32 byteAddressIncrement>
		HK_FORCE_INLINE void convertAndScatter(hkRotation* base) const;															// convert and write to a set of rotations
		HK_FORCE_INLINE void convertAndScatterSLOW(hkRotation* base, hkUint32 byteAddressIncrement) const;						// no preprocessed address calc
		HK_FORCE_INLINE void convertAndScatter(hkRotation* base, hkUint32 byteAddressIncrement, const hkInt32* indices) const;
		HK_FORCE_INLINE void convertAndScatter(hkRotation* base, hkUint32 byteAddressIncrement, const hkUint16* indices) const;
};


#	include <Common/Base/Math/Vector/hkVector4MxVector4.inl>

#endif //HK_MX_VECTOR4_H

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
