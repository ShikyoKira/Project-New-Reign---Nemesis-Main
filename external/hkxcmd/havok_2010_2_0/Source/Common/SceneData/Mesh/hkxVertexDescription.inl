/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkxVertexDescription::hkxVertexDescription()
{

}

inline hkUint32 hkxVertexDescription::getUsageCount( DataUsage dt ) const
{
	int numDecl = 0;
	for (int di=0; di < m_decls.getSize(); ++di)
	{
		if (m_decls[di].m_usage == dt)
		{
			++numDecl;
		}
	}
	return numDecl;
}

inline hkxVertexDescription::ElementDecl* hkxVertexDescription::getElementDeclByIndex( int index )
{
	if ((index >= 0) && (index < m_decls.getSize()) )
	{
		return &m_decls[index];
	}
	return HK_NULL;
}

inline hkxVertexDescription::ElementDecl* hkxVertexDescription::getElementDecl( DataUsage usage, int index )
{
	int numDecl = 0;
	for (int di=0; di < m_decls.getSize(); ++di)
	{
		if (m_decls[di].m_usage == usage)
		{
			if (numDecl == index)
			{
				return &m_decls[di];
			}
			else
			{	
				++numDecl;
			}	
		}
	}
	return HK_NULL;
}

inline const hkxVertexDescription::ElementDecl* hkxVertexDescription::getElementDeclByIndex( int index ) const
{
	return const_cast<hkxVertexDescription*>( this )->getElementDeclByIndex( index );
}

inline const hkxVertexDescription::ElementDecl* hkxVertexDescription::getElementDecl( DataUsage usage, int index ) const
{
	return const_cast<hkxVertexDescription*>( this )->getElementDecl( usage, index );
}

inline hkUint32 hkxVertexDescription::getMask() const
{
	hkUint32 ret = 0;
	for (int di=0; di < m_decls.getSize(); ++di)
	{
		ret |= m_decls[di].m_usage;
	}
	return ret;
}

inline hkUint32 hkxVertexDescription::getElementStride( DataUsage usage, int index ) const
{
	const hkxVertexDescription::ElementDecl* decl = getElementDecl( usage, index );
	HK_ASSERT2(0x3245364, decl, "Non existant element");
	return decl->m_byteStride;
}

inline hkxVertexDescription::DataType hkxVertexDescription::getElementType( DataUsage usage, int index ) const
{
	const hkxVertexDescription::ElementDecl* decl = getElementDecl( usage, index );
	HK_ASSERT2(0x3245364, decl, "Non existant element");
	return decl->m_type;
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
