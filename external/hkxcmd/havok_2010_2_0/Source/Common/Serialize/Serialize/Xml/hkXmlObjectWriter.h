/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_XML_OBJECT_WRITER_H
#define HK_SERIALIZE_XML_OBJECT_WRITER_H

#include <Common/Serialize/Serialize/hkObjectWriter.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

class hkStreamWriter;
class hkClass;

/// Writes a single object to an XML stream.
class hkXmlObjectWriter : public hkObjectWriter
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Callback for naming pointers.
		struct NameFromAddress
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkXmlObjectWriter::NameFromAddress);
			virtual ~NameFromAddress() { }
			virtual int nameFromAddress( const void* addr, char* buf, int bufSize ) = 0;
		};
			/// Numbers each pointer sequentially from 1.
		struct SequentialNameFromAddress : public NameFromAddress
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkXmlObjectWriter::SequentialNameFromAddress);
			virtual int nameFromAddress( const void* addr, char* buf, int bufSize );
			hkPointerMap<const void*, int> m_map;
		};

			/// Create an XML object writer.
			/// The nameFromAddress object must remain valid for the lifetime
			/// of this writer.
		hkXmlObjectWriter(NameFromAddress& nameFromAddress, hkBool writeSerializeIgnored = hkBool(false));

			/// Opens a new element with optional attributes.
			/// "attributes" is a optional null-terminated array of attribute name value pairs.
			/// The indentation is increased by one.
			/// Optionally write a newline before the tag if prefixNewline is true.
		void beginElement(hkStreamWriter* writer, const char* name, const char*const* attributes=HK_NULL, hkBool prefixNewline=true);

			/// End an element. Optionally write a newline before the tag if prefixNewline is true.
		void endElement(hkStreamWriter* writer, const char* name, hkBool prefixNewline=true);

			/// Convenience wrapper for beginElement, writeObject, endElement.
			/// "attributes" is a optional null-terminated array of attribute name value pairs.
		virtual hkResult writeObjectWithElement(hkStreamWriter* writer, const void* data, const hkClass& klass, const char* name, const char*const* attributes=HK_NULL);

			/// Save object data, using class information klass.
			/// Note that this writer does not supply relocations, since
			/// these must be computed at load time on the host.
		virtual hkResult writeObject(hkStreamWriter* writer, const void* data, const hkClass& klass, hkRelocationInfo& reloc);

			/// Write a raw binary chunk.
		virtual hkResult writeRaw(hkStreamWriter* writer, const void* buf, int len );

			/// Change the indentation level by delta tabstops.
		void adjustIndent( int delta );

			/// Forwards to m_nameFromAddress.
		int nameFromAddress( const void* addr, char* buf, int bufSize ) { return m_nameFromAddress.nameFromAddress(addr,buf,bufSize); }

		hkBool getWriteSerializeIgnored() { return m_writeSerializeIgnored; }
	public:

		static hkResult HK_CALL base64write( hkStreamWriter* w, const void* buf, int len );

	private:

		hkInplaceArray<char,16> m_indent;
		NameFromAddress& m_nameFromAddress;
		hkBool m_writeSerializeIgnored;
};

#endif //HK_SERIALIZE_XML_OBJECT_WRITER_H

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
