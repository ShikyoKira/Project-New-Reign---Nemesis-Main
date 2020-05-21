/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_XML_TAGFILE_WRITER_H
#define HK_XML_TAGFILE_WRITER_H

#include <Common/Serialize/Tagfile/hkTagfileWriter.h>

	/// Write tagfiles in a XML format.
class hkXmlTagfileWriter : public hkTagfileWriter
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		/// Save the contents to the given stream.
		/// 
		/// writeFloatAsHex - if true floating point numbers are written out as a 32 bit hex value - which is the IEEE floating point encoding of the floating point number. Otherwise
		/// floats are written out as human readable text.
		/// Whilst the hex style makes human interpretation of values more difficult it will make a write and read produce the same result, whereas, writing a float as text may
		/// due to precision issues not lead to exactly the same result.
	virtual hkResult save( const hkDataObject& obj, hkStreamWriter* stream, AddDataObjectListener* userListener, const Options& options = Options());
};

#endif // HK_XML_TAGFILE_WRITER_H

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
