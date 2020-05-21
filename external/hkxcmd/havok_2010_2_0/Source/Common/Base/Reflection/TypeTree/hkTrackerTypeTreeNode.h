/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_TYPE_TREE_NODE_H
#define HK_TYPE_TREE_NODE_H

#include <Common/Base/Container/SubString/hkSubString.h>

/// This type is not created with new - but created from a hkTrackerTypeTreeCache, with newNode
class hkTrackerTypeTreeNode
{
	public:
		HK_DECLARE_PLACEMENT_ALLOCATOR();
		
		enum Type
		{
			TYPE_UNKNOWN,				///< If it cannot return type
			TYPE_POINTER,				///< Its a pointer
			TYPE_REFERENCE,				///< Its a reference

			TYPE_CLASS_TEMPLATE,		///< Its a templated class/struct

			// Must be kept in order
			TYPE_INT8,					///< Int8
			TYPE_INT16,					///< Int16
			TYPE_INT32,					///< Int32
			TYPE_INT64,					///< Int64

			// Must be kept in order
			TYPE_UINT8,					///< Uint8
			TYPE_UINT16,				///< Uint16
			TYPE_UINT32,				///< UInt32
			TYPE_UINT64,				///< UInt64

			TYPE_FLOAT32,				///< Float32
			TYPE_FLOAT64,				///< Float64

			TYPE_BOOL,					///< Its a bool
			TYPE_VOID,					///< void

			TYPE_NAMED,					///< A named type - could be ENUM/CLASS

			TYPE_ENUM,					///< Its an enum
			TYPE_CLASS,					///< Its a class or struct

			TYPE_ARRAY,					///< Its an array

			TYPE_INT_VALUE,				///< Its an integer value

			TYPE_END,					///< Marks the end
		};
			/// Returns true if the type is named
		hkBool isNamedType() const;
			/// Returns true if the type is named, and the name is the same as the passed in name
		hkBool isNamedType(const char* name) const;
			/// Returns true if it's a straight built in type (not pointer, reference, enum, array, class etc)
		HK_FORCE_INLINE static hkBool isBuiltInType(Type type) { return type >= TYPE_INT8 && type <= TYPE_VOID; }

			/// Dumps this type to a stream
		void dumpType(hkOstream& stream) const;

			/// Ctor
		hkTrackerTypeTreeNode(Type type);

			/// Dumps out 'node' and handles case if node is HK_NULL
		static void HK_CALL dumpType(const hkTrackerTypeTreeNode* node, hkOstream& stream);

		// Note a templated type will hold the linked list of its contained types from the m_contains pointer.
		Type m_type;
		hkSubString m_name;				///< The name of this type (or empty if not needed)
		int m_dimension;				///< The dimension of the array if its an int (or its the value if TYPE_INT_VALUE)
		mutable const hkTrackerTypeTreeNode* m_contains;		///< A type that this references (or HK_NULL if doesn't)
		mutable const hkTrackerTypeTreeNode* m_next;			///< Singly linked list when we have a linked list of nodes (as say parameters to a template)
};

#endif // HK_TYPE_TREE_NODE_H

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
