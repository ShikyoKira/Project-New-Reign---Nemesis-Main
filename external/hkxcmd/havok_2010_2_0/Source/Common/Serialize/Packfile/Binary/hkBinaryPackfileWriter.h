/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_BINARY_PACKFILE_WRITER_H
#define HK_SERIALIZE_BINARY_PACKFILE_WRITER_H

#include <Common/Serialize/Packfile/hkPackfileWriter.h>

/// Writes a memory image packfile.
/// This class writes objects so that they are a memory image
/// and can be loaded with the minimum of processing. i.e.
/// a call to read() and some pointer fixups.
/// This class can write objects for a platform which has
/// different structure layouts from the host platform.
class hkBinaryPackfileWriter : public hkPackfileWriter
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			///
		hkBinaryPackfileWriter(const Options & options = Options());
		
			///	
		~hkBinaryPackfileWriter();

			/// Section to contain names of types requiring finishing.
		static SectionTag SECTION_TAG_CLASSNAMES;

			/// Save the contents to the given stream with the given options.
			/// The options contain the rules used for writing.
			/// See also hkStructureLayout.
		hkResult save( hkStreamWriter* stream, const Options& options );

	private:

		struct SaveContext;
		void writeClassName( SaveContext& context, const hkClass&, int );
		void fillSectionTags( SaveContext& context );
		void writeAllObjects( SaveContext& context );
		void doDeferredWrites( SaveContext& context, int start );

		hkBool m_writeSerializeIgnore;
};

#endif // HK_SERIALIZE_BINARY_PACKFILE_WRITER_H

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
