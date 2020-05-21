/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VISUALIZE_DISPLAY_SERIALIZE_O_STREAM_H
#define HK_VISUALIZE_DISPLAY_SERIALIZE_O_STREAM_H

#include <Common/Base/System/Io/OArchive/hkOArchive.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Common/Visualize/hkDebugDisplayHandler.h>

// non primitive datatypes which are streamable
class hkVector4;
class hkRotation;
class hkTransform;
class hkTriangleIndices;
class hkDisplayGeometry;
class hkAabb;

/// This utility class extends the hkOArchive to provide a endian
/// safe stream interface for serializing all of the basic data
/// types associated with the visual debugger.
class hkDisplaySerializeOStream : public hkOArchive
{
	//+vtable(true)
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES );
			
			/// Create a hkDisplaySerializeOStream from a basic hkStreamWriter.
		hkDisplaySerializeOStream(hkStreamWriter* sw);

			/// Create a hkDisplaySerializeOStream which writes to the buffer 'buf'.
		hkDisplaySerializeOStream(hkArray<char>& buf);

			/// Serialize a hkVector4.
		void writeQuadVector4(const hkVector4& v);

			/// Serialize a hkTransform.
		void writeTransform(const hkTransform& t);

			/// Serialize a hkTriangleIndices.
		void writeTriangle(const hkGeometry::Triangle& ti);

			/// Serialize a hkGeometry.
		void writeGeometry(const hkGeometry& g);

			/// Serialize a hkDisplayGeometry.
		void writeDisplayGeometry(hkDisplayGeometry* dg);

			/// Serialize a display geometry hash.
		void writeHash( const hkDebugDisplayHandler::Hash& hash );

			/// Serialize a hkAabb.
		void writeAabb( const hkAabb& aabb );			

			/// Before serializing an arbitrary display geom, compute the bytes it will take
			/// up so that we don't have to buffer it on the Server before making a packet out of it.
		static hkUint32 computeBytesRequired( const hkGeometry& g );
		static hkUint32 computeBytesRequired( hkDisplayGeometry* dg );
};

#endif //  HK_VISUALIZE_DISPLAY_SERIALIZE_O_STREAM_H

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
