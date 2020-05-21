/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONVERT_CS_H
#define HK_CONVERT_CS_H

#include <Common/Base/hkBase.h>


/// This utility class handles the conversions required for switching between right handed to left handed
/// coordinates (or vice-versa). It can be used to transform data from a right-handed modeller (Max, Maya) into
/// data for the a left-handed renderer (directX for example). It is used extensively in the hkExport2 library
/// while importing data from the max and Maya exporters.
class hkConvertCS
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MATH, hkConvertCS );

		/// A list of the conversion types; apart from CT_IDENTITY (which does nothing), the rest convert from a
		/// right-handed system into a left-handed system (and vice-versa)
		enum ConversionType
		{
			/// No change
			CT_IDENTITY,
			/// Flips the X component (changes handedness)
			CT_FLIP_X,
			/// Flips the Y component (changes handedness)
			CT_FLIP_Y,
			/// Flips the Z component (changes handedness)
			CT_FLIP_Z,
			/// Switches the Y and Z components (changes handedness)
			CT_SWITCH_YZ,
		};

			/// Constructor, specifies the type of conversion to be done
		hkConvertCS (ConversionType type = CT_IDENTITY);

			/// Change the conversion type (calls setConversionRotation() with the right matrix)
		void setConversionType (ConversionType type);

			/// Allows to setup CS conversions different from the pre-defined ones. The rotation parameter
			/// specifies an orthogonal matrix defining the transformation
		void setConversionRotation (const hkRotation& rotation);

			/// Converts an hkVector4 specifying a point or direction in space
		void convertVector (hkVector4& vectorInOut) const;

			/// Converts a quaternion specifying a rotation in space
		void convertQuaternion (hkQuaternion& quaternionInOut) const;

			/// Converts a rotation matrix
		void convertRotation (hkRotation& rotationInOut) const;

			/// Converts an hkReal representing a rotation angle. This will flip the sign of the angle
			/// if the conversion changes handedness
		void convertRotationAngle (hkReal& angle) const;

			/// Converts two values representing angles and, if necessary, switches them to ensure that
			/// minAngle<=maxAngle
		void convertMinMaxAngles (hkReal& minAngle, hkReal& maxAngle) const;

			/// Converts a transform (rotation + translation)
		void convertTransform (hkTransform& transformInOut) const;


	private:

		hkRotation m_conversionRotation;

		// Derived from the matrix above, calculated by setConversionMatrix
		hkBool m_flipping;
		
};

#endif // HK_CONVERT_CS_H

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
