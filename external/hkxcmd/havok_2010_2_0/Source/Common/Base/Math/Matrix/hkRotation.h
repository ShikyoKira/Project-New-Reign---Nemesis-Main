/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_ROTATION_H
#define HK_MATH_ROTATION_H

#ifndef HK_MATH_MATH_H
#	error Please include Common/Base/hkBase.h instead of this file.
#endif

#	define HK_FORCE_ALIGN16 __declspec(align(16))

/// Stores an orthonormal rotation matrix.
/// This is the fastest way to represent a rotation in Havok.
/// If using an hkQuaternion is more convenient, it is possible to convert
/// back and forth between an hkRotation and an hkQuaternion.
/// hkRotations are assumed to be orthonormal i.e., that any given hkRotation r
/// should satisfy the property transpose(r) = (r^-1)
class HK_FORCE_ALIGN16 hkRotation : public hkMatrix3
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkRotation);
		HK_DECLARE_POD_TYPE();

			/// Sets/initializes this rotation matrix from the quaternion q.
		void set(hkQuaternionParameter q);

			/// Sets/initializes this rotation matrix from the given rotation axis and angle.
		void setAxisAngle(const hkVector4& axis, hkReal angle);

			/// Check if this rotation orthonormal to the given epsilon.
		hkBool isOrthonormal( hkReal epsilon = 1e-3f ) const;

			/// Checks for bad values (denormals or infinities) and orthogonality.
		hkBool isOk() const;

			/// Renormalize a drifted rotation.
		void renormalize();
};

#undef HK_FORCE_ALIGN16

#endif //HK_MATH_ROTATION_H

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
