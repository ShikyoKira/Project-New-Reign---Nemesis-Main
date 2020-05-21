/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE bool hkVertexFormat::Element::operator==(const Element& rhs) const
{
    return m_dataType == rhs.m_dataType &&
           m_numValues == rhs.m_numValues &&
           m_usage == rhs.m_usage &&
           m_subUsage == rhs.m_subUsage &&
           m_flags == rhs.m_flags;
}

HK_FORCE_INLINE bool hkVertexFormat::Element::operator!=(const Element& rhs) const 
{ 
	return !(*this == rhs); 
}

HK_FORCE_INLINE int hkVertexFormat::Element::calculateAlignedSize() const
{
    return (hkVertexFormat::s_dataTypeToSize[m_dataType] * m_numValues + 3) & ~3;

}

HK_FORCE_INLINE int hkVertexFormat::Element::calculateSize() const
{
    return hkVertexFormat::s_dataTypeToSize[m_dataType] * m_numValues;
}

HK_FORCE_INLINE void hkVertexFormat::Element::set(ComponentUsage usage, ComponentType dataType, int numValues, int subUsage, int flags )
{
	m_dataType = dataType;
	m_usage = usage;
	m_numValues = hkUint8(numValues);
	m_subUsage = hkUint8(subUsage);
	m_flags = hkUint8(flags);
}


HK_FORCE_INLINE bool hkVertexFormat::operator!=(const hkVertexFormat& rhs) const 
{ 
	return !(*this == rhs); 
}

HK_FORCE_INLINE void hkVertexFormat::clear() 
{ 
	m_numElements = 0; 
}

HK_FORCE_INLINE void hkMeshVertexBuffer::LockedVertices::Buffer::next() 
{ 
	m_start = ((hkUint8*)m_start) + m_stride; 
}

HK_FORCE_INLINE hkMeshVertexBuffer::LockInput::LockInput() 
	: m_startVertex(0)
	, m_numVertices(-1)
	, m_noWait(false)
	, m_contiguousAccess(false)
	, m_lockFlags(ACCESS_READ_WRITE)
{
}

HK_FORCE_INLINE hkMeshVertexBuffer::PartialLockInput::PartialLockInput()
	: m_numLockFlags(0)
{
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
