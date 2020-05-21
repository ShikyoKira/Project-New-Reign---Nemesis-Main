/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_TAGFILE_READER_H
#define HK_TAGFILE_READER_H

class hkDataObject;
class hkStreamReader;
class hkDataWorld;

class hkTagfileReader : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Load the contents from the given stream.
		virtual hkDataObject load( hkStreamReader* stream, hkDataWorld& world ) = 0;

			/// The format of a tagfile.
		enum FormatType
		{
				/// read or open error.
			FORMAT_ERROR,
				/// readable but not recognised.
			FORMAT_UNKNOWN,
				/// binary tagfile
			FORMAT_BINARY
		};
			/// Detect the type of a tagfile stream.
		static hkEnum<FormatType,hkInt32> HK_CALL detectFormat( hkStreamReader* stream );
};

#endif // HK_TAGFILE_READER_H

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
