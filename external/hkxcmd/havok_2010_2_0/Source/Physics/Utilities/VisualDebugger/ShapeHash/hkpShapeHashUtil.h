/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SHAPE_HASH_UTIL_H
#define HK_SHAPE_HASH_UTIL_H

#include <Physics/Utilities/VisualDebugger/ShapeHash/hkCrc64StreamWriter.h>

#include <Physics/Collide/Shape/hkpShapeType.h>

class hkpSphereShape;
class hkpCylinderShape;
class hkpTriangleShape;
class hkpBoxShape;
class hkpCapsuleShape;
class hkpConvexVerticesShape;
class hkpConvexTranslateShape;
class hkpConvexTransformShape;
class hkpTransformShape;
class hkpSampledHeightFieldShape;
class hkpBvShape;
class hkpMultiRayShape;
class hkpPhantomCallbackShape;
class hkpShapeContainer;
class hkpPlaneShape;
class hkpMoppBvTreeShape;
class hkTransform;

/// Contains functions for producing a hash value from shapes.
///
/// Note that the mapping between shapes and hash values is not guaranteed to be consistent between
/// releases of the Havok SDK.
class hkpShapeHashUtil
{
	public:
			/// The utility can operate in various modes.
		enum Mode
		{
			IGNORE_CONVEX_RADIUS,
			USE_CONVEX_RADIUS
		};

	public:
			/// Constructor
		inline hkpShapeHashUtil( hkUint64 startValue );

			/// Get the hash.
			/// \return True if a hash could be computed (i.e., there were no user shapes or deprecated shapes)
			///
			/// See the hkpUserShapeHashUtil for how to add support for user shapes.
		inline hkBool getHash( hkUint64& hashOut ) const;

			/// Make the shape contribute to the hash.
		void writeShape( const hkpShape* shape, Mode mode );

		inline void writeReal( hkReal x );
		inline void writeUint32( hkUint32 x );
		inline void writeUint64( hkUint64 x );
		inline void writeVector3( const hkVector4& vector );
		void writeTransform( const hkTransform& transform );

	public:
		//
		// Internal
		//
		void writeSphereShape( const hkpSphereShape* shape );
		void writeCylinderShape( const hkpCylinderShape* shape, Mode mode );
		void writeTriangleShape( const hkpTriangleShape* shape, Mode mode );
		void writeBoxShape( const hkpBoxShape* shape, Mode mode );
		void writeCapsuleShape( const hkpCapsuleShape* shape );
		void writeConvexVerticesShape( const hkpConvexVerticesShape* shape, Mode mode );
		void writeConvexTranslateShape( const hkpConvexTranslateShape* shape, Mode mode );
		void writeConvexTransformShape( const hkpConvexTransformShape* shape, Mode mode );
		void writeTransformShape( const hkpTransformShape* shape, Mode mode );
		void writeSampledHeightFieldShape( const hkpSampledHeightFieldShape* shape );
		void writeMultiRayShape( const hkpMultiRayShape* shape );
		void writeBvShape( const hkpBvShape* shape, Mode mode );
		void writePhantomCallbackShape( const hkpPhantomCallbackShape* shape );
		void writePlaneShape( const hkpPlaneShape* shape );
		void writeMoppBvTreeShape( const hkpMoppBvTreeShape* shape, Mode mode );

		void writeShapeContainer( const hkpShapeContainer* container, Mode mode );

	protected:
			/// The current hash.
		hkCrc64StreamWriter m_hash;
			/// Whether the util could calculate a hash or not.
		hkBool m_hasHash;

#ifdef HK_DEBUG
	public:
		static void assertShapesUpToDate();
#endif
};

/// This class can be used to register functions to calculate user shapes.
class hkpUserShapeHashUtil : public hkReferencedObject, public hkSingleton<hkpUserShapeHashUtil>
{
		//+vtable(true)
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkpUserShapeHashUtil() {}

			/// The type of functions which calculate hash.
			/// \return True if a hash was computed.
		typedef hkBool (HK_CALL *WriteToHashFunction)( const hkpShape* shape, hkpShapeHashUtil& util );

			/// You can register functions to build display for your own user types with the shape display builder using this method
		void registerUserShapeHashFunction( WriteToHashFunction f, hkpShapeType type );

	public:

		struct UserShapeHashFunctions
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VDB, hkpUserShapeHashUtil::UserShapeHashFunctions );

			WriteToHashFunction m_f;
			hkpShapeType m_type;
		};

		hkArray< UserShapeHashFunctions > m_userShapeHashFunctions;
};

#include <Physics/Utilities/VisualDebugger/ShapeHash/hkpShapeHashUtil.inl>

#endif // HK_SHAPE_HASH_UTIL_H

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
