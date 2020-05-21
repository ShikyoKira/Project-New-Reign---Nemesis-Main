/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_Q_TRANSFORM_H
#define HK_MATH_Q_TRANSFORM_H

#ifndef HK_MATH_MATH_H
#	error Please include Common/Base/hkBase.h instead of this file.
#endif

class hkQTransform
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkQTransform);
	HK_DECLARE_REFLECTION();

	/// Default constructor - all elements are uninitialized.
	HK_FORCE_INLINE hkQTransform() { }

	/// Creates a new hkQTransform using the rotation quaternion q and translation t.
	HK_FORCE_INLINE hkQTransform(const hkQuaternion& q, const hkVector4& t);

	/// Copy constructor
	HK_FORCE_INLINE hkQTransform(const hkQTransform& qt);

	HK_FORCE_INLINE hkQTransform(class hkFinishLoadedObjectFlag f);

	/// Sets the hkQTransform's values using the rotation quaternion q and translation t.
	HK_FORCE_INLINE void set(const hkQuaternion& q, const hkVector4& t);

	/// Sets the hkQTransform's values from a hkTransform.
	HK_FORCE_INLINE void set(const hkTransform& otherTransform);

	/// Sets this hkQTransform to be the identity transform.
	HK_FORCE_INLINE void setIdentity();

	/// Returns a global identity transform.
	HK_FORCE_INLINE static const hkQTransform& HK_CALL getIdentity();

	/// Gets a writable translation component.
	HK_FORCE_INLINE hkVector4& getTranslation();

	/// Gets the translation component.
	HK_FORCE_INLINE const hkVector4& getTranslation() const;

	/// Sets the translation component.
	HK_FORCE_INLINE void setTranslation(const hkVector4& t);

	/// Gets a writable rotation component.
	HK_FORCE_INLINE hkQuaternion& getRotation();

	/// Gets the rotation component.
	HK_FORCE_INLINE const hkQuaternion& getRotation() const;

	/// Sets the rotation component.
	HK_FORCE_INLINE void setRotation(const hkQuaternion& q);

	/// Sets this transform to be the inverse of the given transform qt.
	void setInverse(const hkQTransform& qt);

	/// Sets this transform to be the product of qt1 and qt2. (this = qt1 * qt2)
	void setMul(const hkQTransform& qt1, const hkQTransform& qt2);

	/// Inlined. Sets this transform to be the product of qt1 and qt2. (this = qt1 * qt2)
	HK_FORCE_INLINE void _setMul(const hkQTransform& qt1, const hkQTransform& qt2);

	/// Sets this transform to be the product of t1 and qt2. (this = t1 * qt2)
	void setMul(const hkTransform& t1, const hkQTransform& qt2);

	/// Sets this transform to be the product of t1 and t2. (this = t1 * t2)
	void setMul(const hkTransform& t1, const hkTransform& t2);

	/// Sets this transform to be the product of qt1 and t2. (this = qt1 * t2)
	void setMul(const hkQTransform& qt1, const hkTransform& t2);

	/// Sets this transform to be the product of the inverse of qt1 by qt2. (this = qt1^-1 * qt2)
	void setMulInverseMul(const hkQTransform& qt1, const hkQTransform& qt2);

	/// Sets this transform to be the product of the inverse of t1 by qt2. (this = t1^-1 * qt2)
	void setMulInverseMul(const hkTransform& t1, const hkQTransform& qt2);

	/// Sets this transform to be the product of the inverse of qt1 by t2. (this = qt1^-1 * t2)
	void setMulInverseMul(const hkQTransform& qt1, const hkTransform& t2);

	/// Sets this transform to be the product of the inverse of t1 by t2. (this = t1^-1 * t2)
	void setMulInverseMul(const hkTransform& t1, const hkTransform& t2);

	/// Sets this transform to be the product of the inverse of qt1 by qt2. (this = qt1^-1 * qt2)
	HK_FORCE_INLINE void _setMulInverseMul(const hkQTransform& qt1, const hkQTransform& qt2);

	/// Sets this transform to be the product of qt1 and the inverse of qt2. (this = qt1 * qt2^-1)
	void setMulMulInverse(const hkQTransform &qt1, const hkQTransform &qt2);

	/// Sets this transform to be the product of qt1 and the inverse of qt2. (this = qt1 * qt2^-1)
	HK_FORCE_INLINE void _setMulMulInverse(const hkQTransform &qt1, const hkQTransform &qt2);

	/// Checks if this transform is equal to the other within an optional epsilon.
	hkBool isApproximatelyEqual( const hkQTransform& other, hkReal epsilon=1e-3f ) const;

	/// Sets this transform to a linear interpolation of the transforms qtA and qtB.
	/// Quaternions are checked for polarity and the resulting rotation is normalized
	void setInterpolate4(const hkQTransform& qtA, const hkQTransform& qtB, hkSimdRealParameter t);

private:
	
		/// The rotation part
	hkQuaternion m_rotation;

		/// The translation part
	hkVector4 m_translation;
};

#endif	// HK_MATH_Q_TRANSFORM_H

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
