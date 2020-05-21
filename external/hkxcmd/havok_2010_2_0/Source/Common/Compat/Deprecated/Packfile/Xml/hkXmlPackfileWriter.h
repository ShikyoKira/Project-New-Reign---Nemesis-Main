/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_XML_PACKFILE_WRITER_H
#define HK_XML_PACKFILE_WRITER_H

#include <Common/Serialize/Packfile/hkPackfileWriter.h>

/// Writes an XML packfile.
/// The format is deliberately analogous to the binary
/// format, so that it may be losslessly converted to binary.
class hkXmlPackfileWriter : public hkPackfileWriter
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkXmlPackfileWriter(const Options & options = Options());

			/// Save the contents to the given stream with the given options.
		virtual hkResult save( hkStreamWriter* stream, const Options& options );
};

#endif // HK_XML_PACKFILE_WRITER_H

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
