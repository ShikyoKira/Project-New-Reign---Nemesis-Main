/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_MOPPCODE_STREAMER_H
#define HK_UTILITIES2_MOPPCODE_STREAMER_H


class hkpMoppCode;
class hkOArchive;
class hkIArchive;


/// This class implements a very basic streaming of a hkpMoppCode object to and from an
/// Archive (platform independent stream). It may be useful to use these methods as a guideline
/// for implementing your own file format, as the implementations used here read/write only the
/// minimum data required.
/// In particular, only basic corruption "safety" checks are included. It is up to the user to ensure
/// any "streamed" MOPP data files are in sync with the hkpMoppBvTreeShape you assign them to, otherwise
/// collision detection will fail. It may be advisable to "hash" the shape into a checksum, and
/// write this as additional data into the file, to avoid any such conflicts.
/// This code reads/writes only one hkMoppCodes per Archive, but you may wish to concatenate several
/// hkMoppCodes into a single Archive using the methods below as a guideline.
///
/// NOTICE : The serialization framework (hkSerialize library) available in Havok 2.2 and late versions offers a much more
/// integrated way to serialize MOPP code and other data.
class hkpMoppCodeStreamer
{
	public:
			/// Allocate and fill a hkpMoppCode from an Input Archive.
		static hkpMoppCode* HK_CALL readMoppCodeFromArchive(hkIArchive &inputArchive);

			/// Write a hkpMoppCode to an Output Archive.
		static void HK_CALL writeMoppCodeToArchive(const hkpMoppCode* code, hkOArchive &outputArchive);

};

#endif // HK_UTILITIES2_MOPPCODE_STREAMER_H

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
