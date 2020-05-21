/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_QUATERNION_H
#define HK_MATH_QUATERNION_H

#ifndef HK_MATH_MATH_H
#	error Please include Common/Base/hkBase.h instead of this file.
#endif

/// Stores a unit quaternion.
/// "Unitness" is not enforced, but certain functions (such as division
/// and inversion ) DO assume that the quaternions are unit quaternions.
///
/// \b NOTE - The constructor taking(hkVector4, hkReal) does not set real,imag
///                     but assumes [angle,axis] format.
///
/// \b NOTE - getAxis() returns a valid value only if the angle of the rotation
///                     is nonzero, since the axis is undefined for a rotation of zero degrees
///                     and it is not possible to return it even if the Quaternion was created
///                     from an angle and axis. It was decided that returning an arbitrary
///                     axis would be confusing, so it is up to you to check that the
///                     stored axis is nonzero before attempting to get the axis.
///                     Otherwise, a division by zero Will occur.
///
/// Conventions adopted:
///
///    - When specified by an (angle,axis) pair, the standard mathematical convention is
///      followed, with the angle taken to be such that Quaternion(PI/2, Vector4(0,0,1))
///      will rotate the X vector (1,0,0) to the Y vector (0,0,1). If you are converting to/from a system
///      where such a quaternion would rotate X to -Y, then you will need to flip the angle or axis
///      to convert successfully.
///
///    - p/q is assumed to be p * ( q-inverse ). This
///      is an arbitrary decision since ( q-inverse ) * p is also plausible.
///
///    - The angle extracted by getAngle() is ALWAYS in the range 0 -> PI, since we
///      wish to resolve the ambiguity of storing R(a,v) as either q(a,v) or -q(a,v)
///      It would be nice to assume the real part of q was always positive,
///      but operations involving conversion from transform to quaternion or
///      quaternion multiplication may not preserve this. The best we can do
///      without more code checking) is to let R(a,v) be stored as either,
///      and overload the == operator to check both possibilities.
///      Note that the storage is (imag, real): ((xyz), w)
class hkQuaternion
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkQuaternion);
		HK_DECLARE_POD_TYPE();

			/// Empty constructor.
		inline hkQuaternion() { }

			/// Constructs a quaternion directly from its component elements.
			/// The imaginary part is (ix,iy,iz) and the real part r.
		inline hkQuaternion(hkReal ix, hkReal iy, hkReal iz, hkReal r);

			/// Constructs a quaternion from the rotation matrix r.
		explicit inline hkQuaternion(const hkRotation& r);

			/// Constructs a quaternion from an axis and an angle. The rotation
			/// will take place about that axis by angle radians.
			/// N.B. The axis MUST be normalized.
		inline hkQuaternion(const hkVector4& axis, hkReal angle);

			/// Copy this quaternion.
		inline void operator= (const hkQuaternion& q);

			/// Set this quaternion with imaginary part (ix,iy,iz) and real part r.
		inline void set(hkReal ix, hkReal iy, hkReal iz, hkReal r);

			/// Get a reference to an identity quaternion.
			/// Note that setIdentity will likely be faster as it does not incur a memory access.
		static inline const hkQuaternion& HK_CALL getIdentity();

			/// Sets this quaternion to the unit quaternion. (this = (0, 0, 0, 1))
		inline void setIdentity();

			/// Sets this quaternion to be the inverse of the quaternion q. (this = q^-1)
		inline void setInverse( const hkQuaternion &q );

			/// Normalizes the quaternion. (this = q/|q|)
		inline void normalize();


			/// Sets this quaternion to the product of r by q. (this = r * q)
		inline void setMul(hkSimdRealParameter r, const hkQuaternion& q);
			/// Adds the product of r and q to this quaternion. (this += r * q)
		inline void addMul(hkSimdRealParameter r, const hkQuaternion& q);
			/// Sets this quaternion to the product of q0 and q1. (this = q0 * q1)
		inline void setMul(const hkQuaternion& q0, const hkQuaternion& q1);
			/// Multiplies this quaternion by q. (this *= q)
		inline void mul(const hkQuaternion& q);

			/// Set this quaternion to be the product of q0 and the inverse of q1.
			/// (this = (q0 * q1^-1)
		inline void setMulInverse(const hkQuaternion& q0, const hkQuaternion& q1);

			/// Set this quaternion to be the product of inverse q0 and q1.
			/// (this = (q0^-1 * q1)
		inline void setInverseMul(const hkQuaternion& q0, const hkQuaternion& q1);

			/// returns an estimate for an angle to get from 'this' to 'to'
			/// This function has a pretty good accuracy for angles less than 20 degrees
			/// and underestimates the angle for bigger values.
		inline void estimateAngleTo(const hkQuaternion& to, hkVector4& angleOut) const;

			/// Sets this quaternion to be the shortest rotation which brings 'from' to 'to'
			/// NOTE: The vectors 'from ' and 'to' must be normalized.
		inline void setShortestRotation(const hkVector4& from, const hkVector4& to);

			/// Sets this quaternion to be the shortest rotation which brings 'from' to 'to'
			/// NOTE: The vectors 'from ' and 'to' must be normalized.
			/// This version is damped and the result interpolates from 'from' to to' as gain goes from 0 to 1
			/// This is similar to scaling the angle of rotation according to the gain.
		inline void setShortestRotationDamped(hkReal gain, const hkVector4& from, const hkVector4& to);

			/// Sets/initializes this quaternion given a rotation axis and angle.
			/// N.B. The axis MUST be normalized.
		void setAxisAngle(const hkVector4& axis, hkReal angle);

			/// Sets/initializes this quaternion from a given rotation matrix.
			/// The rotation r must be orthonormal.
		void set(const hkRotation& r);

			/// Inlined. Sets/initializes this quaternion from a given rotation matrix.
			/// The rotation r must be orthonormal.
		HK_FORCE_INLINE void _set(const hkRotation& r);

			/// Inlined. Sets/initializes this quaternion from a given rotation matrix.
			/// The rotation r must be orthonormal.
		HK_FORCE_INLINE void setFromRotationSimd(const hkRotation& r);

			/// Inlined. Sets/initializes this quaternion from a given rotation matrix.
			/// The rotation r must be orthonormal.
		HK_FORCE_INLINE void setFromRotationFpu(const hkRotation& r);

			/// Sets/initializes this quaternion from a given rotation matrix.
		void setAndNormalize(const hkRotation& r);

			/// Removes the component of this quaternion that represents a rotation (twist) around the given axis.
			/// In more accurate terms, it converts this quaternion (q) to the smallest (smallest angle)
			/// rotation (q') that still satisfies q * axis = q' * axis.
			/// This is done by applying calculating axis' = q * axis. Then, from axis and axis' a perpendicular
			/// vector (v) is calculated, as well as the angle between axis and axis' (ang). The result of the
			/// operation is the rotation specified by the angle (ang) and the axis (v).
		void removeAxisComponent (const hkVector4& axis);
	
			/// Decomposes the quaternion and returns the amount of rotation around the given axis, and the rest
			/// (this) == Quaternion(axis, angle) * rest , where "rest" is the rest of rotation. This is done
			/// by calculating "rest" using removeAxisComponent() and then calculating (axis,angle) as
			/// this * inv (rest).
		void decomposeRestAxis (const hkVector4& axis, hkQuaternion& restOut, hkReal& angleOut) const;

			/// Calculates the spherical linear interpolation between q0 and q1
			/// parameterized by t. If t is 0 then the result will be q0.
		void setSlerp(const hkQuaternion& q0, const hkQuaternion& q1, hkReal t);
		
		/* access access */

			/// Sets the real component of this quaternion.
		inline void setReal(hkReal r);
			/// Returns the real component of this quaternion.
		inline hkReal getReal() const;

			/// Sets the imaginary component of this quaternion.
		inline void setImag(const hkVector4& );
			/// Returns a read only reference to the imaginary component of this quaternion.
		HK_ALWAYS_INLINE const hkVector4& getImag() const;

			/// This method extracts the angle of rotation, always returning it as a real in
			/// the range [0,PI].
			/// NOTE - The standard mathematical convention is followed, with the angle taken to be
			/// such that Quaternion((0,0,1), PI/2,) will rotate the X vector (1,0,0) to the Y vector (0,0,1).
			/// If you are converting to/from a system where such a quaternion would rotate X to -Y,
			/// then you will need to flip the angle or axis to convert successfully.
			/// \code
			/// hkQuaternion q((1,0,0), -0.7);
			/// hkReal ang = q.getAngle() ;
			/// // ang is now 0.7 radians (and a call to getAxis() will return (-1,0,0) )
			/// \endcode
		inline hkReal getAngle()const;

			/// Return the normalized axis of rotation IF DEFINED.
			/// The axis is not defined for a zero rotation (getAngle() returns 0.0f, or getReal() returns 1 or -1)
			/// The direction of the axis is determined by the sign of the angle returned by getAngle(), so that
			/// getAxis() and getAngle() return consistent values. N.B. getAngle() always returns a *positive* value.
			///
			/// NOTE - The standard mathematical convention is followed, with the angle taken to be
			/// such that Quaternion((0,0,1), PI/2) will rotate the X vector (1,0,0) to the Y vector (0,0,1).
			/// If you are converting to/from a system where such a quaternion would rotate X to -Y,
			/// then you will need to flip the angle or axis to convert successfully.
			///
			/// NOTE - getAxis() returns a valid value ONLY if the angle of the rotation
			/// is (numerically) nonzero, since the axis is undefined for a rotation of zero degrees
			/// and it is not possible to return it even if the Quaternion was created
			/// from an angle and axis. It was decided that returning an arbitrary
			/// axis would be confusing, so it is up to the user to check that the
			/// stored axis is nonzero before attempting to get the axis.
			/// Otherwise, a division by zero will occur. The function hasValidAxis()
			/// may be called to determine if calls to getAxis() will return a valid vector.
			/// e.g., hkQuaternion((1,0,0), 1.5 PI) will return:
			///     - 0.5 PI as angle
			///     - (-1,0,0) as axis.
		inline void getAxis(hkVector4 &axis) const;

			/// Determines if the quaternion has a valid axis of rotation. See getAxis()
		inline hkBool hasValidAxis() const;

			/// Read only access to the i'th component of this quaternion. (stored as (ix,iy,iz,real))
		inline const hkReal& operator()(int i) const;

			/// Checks that all elements are valid numbers, and length is 1.0 (within 1e-3)
		hkBool isOk() const;

			/// Sets the quaternion to the given quaternion q, eventually transforming it so q and qReference are in the same hemisphere
		inline void setClosest(const hkQuaternion& q, const hkQuaternion& qReference);

			///	Computes the logarithm of a quaternion
		inline void getLog(hkVector4& vOut) const;

			///	Set the quaternion as the exponential of the given (axis-angle) vector
		inline void setExp(const hkVector4& vIn);

	public:

		hkVector4 m_vec;

	protected:

		void setFlippedRotation(const hkVector4& from);
};

typedef const hkQuaternion& hkQuaternionParameter;


inline const hkQuaternion& HK_CALL hkQuaternion::getIdentity()
{
	return (const hkQuaternion&)hkVector4::getConstant( HK_QUADREAL_0001 );
}

#endif //HK_MATH_QUATERNION_H

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
