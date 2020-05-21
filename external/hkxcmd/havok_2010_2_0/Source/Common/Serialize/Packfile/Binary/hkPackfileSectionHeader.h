/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALIZE_SERIALIZE_BINARY_HKPACKFILESECTIONHEADER_XML_H
#define HKSERIALIZE_SERIALIZE_BINARY_HKPACKFILESECTIONHEADER_XML_H

#include <Common/Serialize/Resource/hkResource.h>

/// hkPackfileSectionHeader meta information
extern const class hkClass hkPackfileSectionHeaderClass;

/// Packfiles are composed of several sections.
/// A section contains several areas
/// | data | local | global | finish | exports | imports |
/// data: the user usable data.
/// local: pointer patches within this section (src,dst).
/// global: pointer patches to locations within this packfile (src,(section,dst)).
/// finish: offset and typename of all objects for finish functions (src, typename).
/// exports: named objects (src,name).
/// imports: named pointer patches outside this packfile (src,name).
class hkPackfileSectionHeader
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, hkPackfileSectionHeader);
		HK_DECLARE_REFLECTION();
	
			/// Default constructor
		hkPackfileSectionHeader()
		{
			hkString::memSet(this,0,sizeof(this));
		}
		
		void* getStart( void* absoluteStart )
		{
			return hkAddByteOffset(absoluteStart, m_absoluteDataStart);
		}

			/// Size in bytes of data part
		int getDataSize() const
		{
			return m_localFixupsOffset;
		}
			/// Size in bytes of intra section pointer patches
		int getLocalSize() const
		{
			return m_globalFixupsOffset - m_localFixupsOffset;
		}
			/// Size in bytes of inter section pointer patches
		int getGlobalSize() const
		{
			return m_virtualFixupsOffset - m_globalFixupsOffset;
		}
			/// Size in bytes of finishing table.
		int getFinishSize() const
		{
			return m_exportsOffset - m_virtualFixupsOffset;
		}
			/// Size in bytes of exports table.
		int getExportsSize() const
		{
			return m_importsOffset - m_exportsOffset;
		}
			/// Size in bytes of imports table.
		int getImportsSize() const
		{
			return m_endOffset - m_importsOffset;
		}
		
			/// Extract exports from the given section.
		void getExports( const void* sectionBegin, hkArray<hkResource::Export>& exports ) const;

			/// Extract imports from the given section.
		void getImports( const void* sectionBegin, hkArray<hkResource::Import>& imports ) const;

	public:
		
			/// 
		char m_sectionTag[19];
		
			/// 
		char m_nullByte;
		
			/// Absolute file offset of where this sections data begins.
		hkInt32 m_absoluteDataStart;
		
			/// Offset of local fixups from absoluteDataStart.
		hkInt32 m_localFixupsOffset;
		
			/// Offset of global fixups from absoluteDataStart.
		hkInt32 m_globalFixupsOffset;
		
			/// Offset of virtual fixups from absoluteDataStart.
		hkInt32 m_virtualFixupsOffset;
		
			/// Offset of exports from absoluteDataStart.
		hkInt32 m_exportsOffset;
		
			/// Offset of imports from absoluteDataStart.
		hkInt32 m_importsOffset;
		
			/// Offset of the end of section. Also the section size.
		hkInt32 m_endOffset;
};

#endif // HKSERIALIZE_SERIALIZE_BINARY_HKPACKFILESECTIONHEADER_XML_H

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
