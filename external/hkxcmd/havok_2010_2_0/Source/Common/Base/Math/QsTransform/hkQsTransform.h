/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_QSTRANSFORM_H
#define HK_MATH_QSTRANSFORM_H

#ifndef HK_MATH_MATH_H
#	error Please include Common/Base/hkBase.h instead of this file.
#endif

/// An hkQsTransform represents a T*R*S transformation where translation is
/// represented with a vector4, rotation is represented with a quaternion and scale
/// is represented with a vector4.
/// When applied to a point (hkVector4::setTransformedPos()), the point is first scaled, then rotated and
/// finally translated.
/// Direct read/write access to the three components of the hkQsTransform is available through he public
/// members m_translation, m_rotation, m_scale. Const access can be done through get() methods.
/// It is important to notice that, due to the nature of the TRS representation, and since not all affine
/// transformations can be represented with an hkQsTransform, operations like multiplications have specific definitions
/// in order to keep the result inside the space of hkQsTransforms.
/// For more information about this, please check the Havok User Manual.
class hkQsTransform
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkQsTransform);
		HK_DECLARE_POD_TYPE();

			// This enum is used to specify a constructor that initializes to identity
		enum IdentityInitializer
		{
			IDENTITY,
		};

			// This enum is used to specify a constructor that initializes to zero
		enum ZeroInitializer
		{
			ZERO,
		};

			/// Default constructor - all elements are uninitialized.
		HK_FORCE_INLINE hkQsTransform() { }

			/// Constructor - hkQsTransform(hkQsTransform::IDENTITY) initializes to identity
		HK_FORCE_INLINE hkQsTransform(IdentityInitializer init);

			/// Constructor - hkQsTransform(hkQsTransform::ZERO) initializes to zero
		HK_FORCE_INLINE hkQsTransform(ZeroInitializer init);

			/// Creates a new hkQsTransform using the rotation r, translation t and scale s
		HK_FORCE_INLINE hkQsTransform(const hkVector4& translation, const hkQuaternion& rotation, const hkVector4& scale);

			/// Creates a new hkQsTransform using the rotation r, translation t. Sets scale to identity (1,1,1)
		HK_FORCE_INLINE hkQsTransform(const hkVector4& translation, const hkQuaternion& rotation);

			/// Copy constructor
		HK_FORCE_INLINE hkQsTransform(const hkQsTransform& other);

			/// Sets the hkQsTransform's values using the rotation quaternion q, translation t and scale s
		HK_FORCE_INLINE void set(const hkVector4& translation, const hkQuaternion& rotation, const hkVector4& scale);

			/// Sets this hkQsTransform to be the identity transform.
		HK_FORCE_INLINE void setIdentity();

			/// Returns a global identity transform.
		HK_FORCE_INLINE static const hkQsTransform& HK_CALL getIdentity();

			/// Gets the translation component.
		HK_FORCE_INLINE const hkVector4& getTranslation() const;

			/// Sets the translation component.
		HK_FORCE_INLINE void setTranslation(const hkVector4& t);

			/// Gets the scale component.
		HK_FORCE_INLINE const hkVector4& getScale() const;
	
			/// Sets the scale component.
		HK_FORCE_INLINE void setScale(const hkVector4& s);

			/// Gets the rotation component.
		HK_FORCE_INLINE const hkQuaternion& getRotation() const;

			/// Sets the rotation component (using a hkQuaternion).
		HK_FORCE_INLINE void setRotation(const hkQuaternion& rotation);

			/// Sets the rotation component (using a hkRotation).
		HK_FORCE_INLINE void setRotation(const hkRotation& rotation);

			/// Sets this transform to a linear interpolation of the transforms a and b.
			/// Quaternions are checked for polarity and the resulting rotation is normalized
		HK_FORCE_INLINE void setInterpolate4( const hkQsTransform& a, const hkQsTransform& b, hkSimdRealParameter t);

			/// Conversion from hkTransform. Assume no scale
		void setFromTransformNoScale (const hkTransform& transform);

			/// Conversion to hkTransform. Assume no scale
		void copyToTransformNoScale (hkTransform& transformOut) const;

			/// Conversion from hkTransform - scale.
		void setFromTransform (const hkTransform& transform);

			/// Conversion from hkQTransform
		void setFromTransform(const hkQTransform& qt);

			/// Conversion to hkTransform - scale.
		void copyToTransform (hkTransform& transformOut) const;

			/// Sets this transform to be the inverse of the given transform t.
		HK_FORCE_INLINE void setInverse( const hkQsTransform &t );

			/// Sets this transform to be the product of t1 and t2. (this = t1 * t2)
		HK_FORCE_INLINE void setMul( const hkQsTransform &t1, const hkQsTransform &t2 );

			/// Sets this transform to be the product of the inverse of t1 by t2. (this = t1^-1 * t2)
		HK_FORCE_INLINE void setMulInverseMul( const hkQsTransform& t1, const hkQsTransform &t2 );

		/// Sets this transform to be the product of t1 and the inverse of t2. (this = t1 * t2^-1)
		HK_FORCE_INLINE void setMulMulInverse( const hkQsTransform &t1, const hkQsTransform &t2 );

		/// Sets this transform to be the product of itself and the transform b. (this *= b)
		HK_FORCE_INLINE void setMulEq( const hkQsTransform& b );

			/// Writes a 4x4 matrix suitable for rendering into p.
		void get4x4ColumnMajor(hkReal* p) const;

			/// Sets the components from a 4x4 matrix. Returns false if the matrix cannot be
			/// decomposed to form a proper hkQsTransform.
		hkBool set4x4ColumnMajor(const hkReal* p);


			/// Checks for bad values (denormals or infinities).
		hkBool isOk() const;

			/// Checks if this transform is equal to the other within an optional epsilon.
		hkBool isApproximatelyEqual( const hkQsTransform& other, hkReal epsilon=1e-3f ) const;

			/*
			** Blending operations
			*/

			/// Prepares the qs transform for blending (sets all to zero)
		HK_FORCE_INLINE void setZero();
			
			/// Blends a weighted qs transform into this one.
			/// Quaternions are checked for polarity but the resulting rotation is NOT normalized.
			/// Start with a hkQsTransform with all zeros (including the quaternion).
			/// Call blendAddMul for each transform.
			/// Call blendNormalize or fastRenormalize when finished blending.
		HK_FORCE_INLINE void blendAddMul(const hkQsTransform& other,  hkSimdRealParameter weight = hkSimdReal::getConstant(HK_QUADREAL_1) );

			/// Renormalizes after blending. If a total weight is given,
			/// the whole transformation is weighted by 1.0f / total weight.
			/// Takes the "zero" cases into account.
		HK_FORCE_INLINE void blendNormalize( hkSimdRealParameter totalWeight = hkSimdReal::getConstant(HK_QUADREAL_1));

			/// Renormalizes without error checking
		HK_FORCE_INLINE void fastRenormalize( hkSimdRealParameter totalWeight = hkSimdReal::getConstant(HK_QUADREAL_1));

			/// Renormalizes a group of quaternions at once. This allows for some
			/// vector optimizations on different platforms.
		static void HK_CALL fastRenormalizeBatch( hkQsTransform* poseOut, hkReal* weight, hkUint32 numTransforms);

	public:

			/// Direct access to the translation component (vector4) of this hkQsTransform.
			/// Fourth component is not used.
		hkVector4 m_translation;

			/// Direct access to the rotation component (quaternion) of this hkQsTransform
		hkQuaternion m_rotation;

			/// Direct access to the scale component (vector4) of this hkQsTransform.
			/// The fourth component is not used.
		hkVector4 m_scale;
};

#endif //HK_MATH_QSTRANSFORM_H

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
