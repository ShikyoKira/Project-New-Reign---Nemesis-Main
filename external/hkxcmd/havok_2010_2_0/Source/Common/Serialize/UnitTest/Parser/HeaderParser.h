/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALIZE_HEADER_PARSER
#define HKSERIALIZE_HEADER_PARSER


extern const class hkClass HeaderParserClass;

extern const class hkClass HeaderParser2Class;

class HeaderParser
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, HeaderParser );

		HK_DECLARE_REFLECTION();

		enum NestedEnum
		{
			VALUE_ZERO
		};

		hkArray<char> m_charBuf; //+nosave

		int* m_vertices;
		int m_numVertices;
		
		int* m_noSaveVertices;
		int m_numNoSaveVertices; //+nosave

		// this should generate an error +nosave should be on second member
		//int* m_noSaveVertices; //+nosave
		//int m_numNoSaveVertices;

		hkEnum<NestedEnum,hkInt8> m_enum;

		hkStringPtr m_name; //+default("my name")
};

class HeaderParser2
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, HeaderParser2 );

		HK_DECLARE_REFLECTION();

		enum NestedEnum
		{
			VALUE_ZERO
		};

		hkEnum<NestedEnum,hkInt8> m_enum2;
};

#endif // HKSERIALIZE_HEADER_PARSER

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
