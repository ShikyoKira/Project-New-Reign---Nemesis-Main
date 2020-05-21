/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TYPE_TREE_TEXT_CACHE_H
#define HK_TYPE_TREE_TEXT_CACHE_H

#include <Common/Base/Reflection/TypeTree/hkTrackerTypeTreeNode.h>

class hkTrackerTypeTreeTextCache: public hkReferencedObject 
{
    public:
		typedef hkTrackerTypeTreeNode Node;
		HK_DECLARE_CLASS_ALLOCATOR(hkTrackerTypeTreeTextCache);

			/// Get the type index
		int getTypeIndex(const Node* type);
			/// Get the type text
		const char* getTypeText(const Node* type);

			/// Get the type names - the index to the start of each one is the index that is returned by the
			/// getTypeIndex method.
			/// Also note that all names are 0 terminated.
		const hkArray<char>& getTypeNames() const { return m_typeNames; }

    protected:
		hkArray<char> m_typeNames;
		hkPointerMap<const Node*, int> m_map;
};


#endif // HKBASE_RTTI_NODE_TEXT_CACHE_H

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
