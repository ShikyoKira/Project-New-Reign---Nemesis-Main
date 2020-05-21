/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Remove left horizontal links
// Skew is a right rotation when an insertion or deletion creates a left red link.
HK_FORCE_INLINE void hkAATree::skew(Node** hlink)
{
	Node* t = *hlink;
	if ( (t->m_link[0]->m_level == t->m_level) && (t->m_level != 0) ) 
	{
		Node *save = t->m_link[0];
		t->m_link[0] = save->m_link[1];
		save->m_link[1] = t;
		*hlink = save;
	}
}

// Remove consecutive horizontal links
// Split is a conditional left rotation when an insertion or deletion creates two consecutive red links.
HK_FORCE_INLINE void hkAATree::split(Node** hlink)
{
	Node* t = *hlink;
	if ( (t->m_link[1]->m_link[1]->m_level == t->m_level) && (t->m_level != 0) )
	{
		Node *save = t->m_link[1];
		t->m_link[1] = save->m_link[0];
		save->m_link[0] = t;
		*hlink = save;
		++((*hlink)->m_level);
	}
}

HK_FORCE_INLINE hkAATree::Node* hkAATree::newNode( void *data )
{
	Node* rn = new Node;

	if ( rn == HK_NULL )
		return m_nil;

	rn->m_level = 1;
	rn->m_data = m_dup ( data );
	rn->m_link[0] = rn->m_link[1] = m_nil;

	return rn;
}

HK_FORCE_INLINE hkUint32 hkAATree::getSize() const
{
	return m_size;
}

HK_FORCE_INLINE void* hkAATree::Iterator::first( hkAATree* tree )
{
	return start( tree, 0 ); // ie. min value
}

HK_FORCE_INLINE void* hkAATree::Iterator::last( hkAATree* tree )
{
	return start( tree, 1 ); // ie. max value
}

HK_FORCE_INLINE void* hkAATree::Iterator::next()
{
	return move( 1 ); // towards larger items
}

HK_FORCE_INLINE void* hkAATree::Iterator::prev()
{
	return move( 0 ); // towards smaller items
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
