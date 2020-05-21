/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template <typename T>
inline hkTree<T>::hkTree()
	: m_root(HK_NULL)
{
}

template <typename T>
inline hkTree<T>::~hkTree()
{
	clear();
}

template <typename T>
inline void hkTree<T>::clear()
{
	if( m_root )
	{
		remove(m_root);
		m_root = HK_NULL;
	}
}

template <typename T>
inline typename hkTree<T>::Iter hkTree<T>::appendChild(Iter iter, const T& value)
{
	Node* newNode = new Node(value);
	newNode->m_parent = iter;

	if( iter != HK_NULL )
	{
		if( Node* n = iter->m_firstChild )
		{
			while( Node* m = n->m_nextSibling )
			{
				n = m;
			}
			n->m_nextSibling = newNode;
		}
		else
		{
			iter->m_firstChild = newNode;
		}
	}
	else if( Node* c = m_root ) // add to existing root
	{
		while( Node* n = c->m_nextSibling )
		{
			c = n;
		}
		c->m_nextSibling = newNode;
	}
	else // new root
	{
		m_root = newNode;
	}
	return newNode;
}

template <typename T>
inline typename hkTree<T>::Iter hkTree<T>::appendSibling(Iter iter, const T& value)
{
	HK_ASSERT(0x273481ab, iter != HK_NULL);
	HK_ASSERT(0x273481ac, iter->m_nextSibling == HK_NULL);
	Node* newNode = new Node();
	newNode->m_value = value;
	newNode->m_parent = iter->m_parent;
	newNode->m_nextSibling = iter->m_nextSibling;
	iter->m_nextSibling = newNode;
	return newNode;
}

template <typename T>
inline void hkTree<T>::remove(Iter iter)
{
	HK_ASSERT(0x21d5eb18, iter != HK_NULL );
	if( iter->m_parent ) // unhook node from tree
	{ 
		HK_ASSERT(0x1d043145, iter->m_parent->m_firstChild);
		Node* prev = iter->m_parent->m_firstChild;
		if( prev == iter )
		{
			iter->m_parent->m_firstChild = HK_NULL;
		}
		else
		{
			while( prev->m_nextSibling != iter )
			{
				prev = prev->m_nextSibling;
			}
			prev->m_nextSibling = iter->m_nextSibling;
		}
		iter->m_parent = HK_NULL;
	}
	// now destroy
	{ 
		Iter cur = iter;
		while(cur)
		{
			while( cur->m_firstChild )
			{
				cur = cur->m_firstChild;
			}
			Iter next = cur->m_nextSibling ? cur->m_nextSibling : cur->m_parent;
			if( Node* p = cur->m_parent )
			{
				p->m_firstChild = HK_NULL;
			}
			delete cur;
			cur = next;
		}
	}
}

template <typename T>
inline void hkTree<T>::setValue(Iter i, const T& t )
{
	HK_ASSERT(0x273481ab, i != HK_NULL);
	i->m_value = t;
}

template <typename T>
inline T& hkTree<T>::getValue(Iter i) const
{
	HK_ASSERT(0x273481ab, i != HK_NULL);
	return i->m_value;
}

template <typename T>
inline int hkTree<T>::getDepth(Iter i) const
{
	HK_ASSERT(0x273481ab, i != HK_NULL);
	int d = -1;
	Node* n = i;
	while( n )
	{
		n = n->m_parent;
		d += 1;
	}
	return d;
}

template <typename T>
inline int hkTree<T>::getNumChildren(Iter i) const
{
	HK_ASSERT(0x273481ab, i != HK_NULL);
	int c = 0;
	Node* n = i->m_firstChild;
	while( n )
	{
		n = n->m_nextSibling;
		c += 1;
	}
	return c;
}

template <typename T>
inline typename hkTree<T>::Iter hkTree<T>::iterGetFirst() const
{
	return const_cast<Iter>(m_root);
}

template <typename T>
inline typename hkTree<T>::Iter hkTree<T>::iterNext(Iter i) const
{
	HK_ASSERT(0x273481ab, i != HK_NULL);
	return i->m_nextSibling;
}

template <typename T>
inline typename hkTree<T>::Iter hkTree<T>::iterParent(Iter i) const
{
	HK_ASSERT(0x273481ab, i != HK_NULL);
	return i->m_parent;
}

template <typename T>
inline typename hkTree<T>::Iter hkTree<T>::iterChildren(Iter i) const
{
	return i ? i->m_firstChild : m_root;
}

template <typename T>
inline typename hkTree<T>::Iter hkTree<T>::iterNextPreOrder(Iter i) const
{
	HK_ASSERT(0x273481ab, i != HK_NULL);
	if( i->m_firstChild )
	{
		return i->m_firstChild;
	}
	else
	{
		while( i->m_nextSibling == HK_NULL )
		{
			i = i->m_parent;
			if( i == HK_NULL )
			{
				return HK_NULL;
			}
		}
		return i->m_nextSibling;
	}
}

template <typename T>
inline typename hkTree<T>::Iter hkTree<T>::iterNextPreOrderSkippingChildren(Iter i) const
{
	HK_ASSERT(0x273481ab, i != HK_NULL);
	while( i->m_nextSibling == HK_NULL )
	{
		i = i->m_parent;
		if( i == HK_NULL )
		{
			return HK_NULL;
		}
	}
	return i->m_nextSibling;
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
