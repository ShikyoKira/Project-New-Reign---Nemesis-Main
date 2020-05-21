/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkpMoppBvTreeShape::hkpMoppBvTreeShape( hkFinishLoadedObjectFlag flag ) : hkMoppBvTreeShapeBase(flag), m_child(flag) 
{ 	
	if (flag.m_finishing == 1)
	{
		m_type = HK_SHAPE_MOPP;  
		m_codeInfoCopy = m_code->m_info.m_offset;
		m_moppData = m_code->m_data.begin();
		m_moppDataSize = m_code->getCodeSize();
	}
}

inline const hkpShapeCollection* hkpMoppBvTreeShape::getShapeCollection() const
{
	return static_cast<const hkpShapeCollection*>(m_child.getChild());
}

inline const hkpShapeContainer* hkpMoppBvTreeShape::getContainer() const
{
	return getShapeCollection()->getContainer();
}


/// Get the internal data used by the MOPP algorithms
inline const hkpMoppCode*  hkpMoppBvTreeShape::getMoppCode() const 
{ 
	return m_code; 
}

inline void  hkpMoppBvTreeShape::setMoppCode(const hkpMoppCode* code) 
{ 
	HK_ASSERT2(0xcf45fedc, code, "You can not supply a null MoppCode. Was the MOPP setup correctly?.");
	code->addReference();
	m_code->removeReference();
	m_code = code;
	m_codeInfoCopy = code->m_info.m_offset;
	m_moppData = code->m_data.begin();
	m_moppDataSize = code->getCodeSize();
}


const hkpShape* hkpMoppBvTreeShape::getChild() const
{
	return m_child.getChild();
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
