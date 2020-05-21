/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkXmlParser::StartElement* hkXmlParser::Node::asStart()
{
	return (type==START_ELEMENT) ? static_cast<hkXmlParser::StartElement*>(this) : HK_NULL;
}
inline hkXmlParser::EndElement* hkXmlParser::Node::asEnd()
{
	return (type==hkXmlParser::END_ELEMENT) ? static_cast<hkXmlParser::EndElement*>(this) : HK_NULL;
}
inline hkXmlParser::Characters* hkXmlParser::Node::asCharacters()
{
	return (type==hkXmlParser::CHARACTERS)? static_cast<hkXmlParser::Characters*>(this) : HK_NULL;
}

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
