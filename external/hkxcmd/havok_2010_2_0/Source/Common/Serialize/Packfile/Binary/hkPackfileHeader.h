/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALIZE_SERIALIZE_BINARY_HKPACKFILEHEADER_XML_H
#define HKSERIALIZE_SERIALIZE_BINARY_HKPACKFILEHEADER_XML_H


/// hkPackfileHeader meta information
extern const class hkClass hkPackfileHeaderClass;

/// The header of a binary packfile.
class hkPackfileHeader
{
	//+version(1)
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, hkPackfileHeader);
		HK_DECLARE_REFLECTION();
	
			/// Default constructor
		hkPackfileHeader()
		{
			hkString::memSet(this, -1, sizeof(*this));
			m_magic[0] = 0x57e0e057; // both endian agnostic (byte symmetric)
			m_magic[1] = 0x10c0c010;
			m_contentsVersion[0] = 0;
			m_flags = 0; // Set to 1 when a packfile is loaded in-place to signify that the packfile is loaded
		}
		
	public:
		
			/// Magic file identifier. See constructor for values.
		hkInt32 m_magic[2];
		
			/// This is a user settable tag.
		hkInt32 m_userTag;
		
			/// Binary file version. Currently 8
		hkInt32 m_fileVersion;
		
			/// The structure layout rules used by this file.
		hkUint8 m_layoutRules[4];
		
			/// Number of packfilesections following this header.
		hkInt32 m_numSections;
		
			/// Where the content's data structure is (section and offset within that section).
		hkInt32 m_contentsSectionIndex;
		hkInt32 m_contentsSectionOffset;

			/// Where the content's class name is (section and offset within that section).
		hkInt32 m_contentsClassNameSectionIndex;
		hkInt32 m_contentsClassNameSectionOffset;

			/// Future expansion
		char m_contentsVersion[16];

			///
		hkInt32 m_flags;
		hkInt32 m_pad[1];
};

#endif // HKSERIALIZE_SERIALIZE_BINARY_HKPACKFILEHEADER_XML_H

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
