/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_LAYOUT_CALCULATOR_H
#define HKBASE_LAYOUT_CALCULATOR_H

#include <Common/Base/Reflection/TypeTree/hkTrackerTypeTreeCache.h>

#include <Common/Base/Container/StringMap/hkStorageStringMap.h>

class hkTrackerTypeLayout: public hkReferencedObject
{	
	public:
		HK_DECLARE_CLASS_ALLOCATOR(hkTrackerTypeLayout);
		typedef hkTrackerTypeTreeNode Node;

		struct Member
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,Member);
			enum
			{
				FLAG_BACK_POINTER = 0x1		///< If set this is a back pointer
			};

			const Node* m_type;			///< The type of the member (HK_NULL is void*)
			hkUint16 m_offset;				///< Offset from the object start
			hkUint16 m_size;				///< The members size
			const char* m_name;				///< The name of the member
			int m_flags;					///< Flags
		};

			/// Ctor
		hkTrackerTypeLayout(const Node* type, int alignment, int size):
			m_type(type),
			m_alignment(hkUint8(alignment)),
			m_size(hkUint16(size)),
			m_isVirtual(false),
			m_fullScan(false),
			m_hasReferenceInfo(false)
		{}

			
		const Node* m_type;
		hkArray<Member> m_members;			///< The members
		hkArray<char> m_nameBuffer;			///< Storage for all member names
		hkUint8 m_alignment;				///< The alignment of a type
		hkUint16 m_size;					///< Total size of an instance
		hkBool m_isVirtual;					///< True if virtual
		hkBool m_fullScan;					///< If set then do a complete scan of the type
		
		hkBool m_hasReferenceInfo;			///< Can describe other blocks of memory
};

struct hkTrackerLayoutTypeInfo
{
	hk_size_t m_size;
	int m_alignment;						
	hkBool m_hasReferences;				///< Contains references to other objects
	hkBool m_hasReferenceInfo;			///< If true can produce reference info data (via calcReferencedTypes)
};


class hkTrackerLayoutCalculator;
class hkTrackerSnapshot;

class hkTrackerLayoutHandler: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(hkTrackerTypeLayout);
		typedef hkTrackerTypeTreeNode Node;


		struct ReferenceInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkTrackerLayoutHandler::ReferenceInfo);
			enum
			{
				INVALID_USED_SIZE = hk_size_t(0) - 1
			};

			const Node* m_type;			///< The underlying type
			int m_numMembers;				///< -1 if not an array
			void* m_start;					///< Start address (should be the start of an allocation generally)
			hk_size_t m_usedSize;			///< Total used size (in bytes).
		};

		virtual hkTrackerTypeLayout* calcTypeLayout(const Node* type, hkTrackerLayoutCalculator* calc) = 0;			
		virtual void calcReferencedTypes(const hkTrackerTypeLayout* layout, hkTrackerLayoutCalculator* calc, const hkTrackerSnapshot* snapshot, void* data, hkArray<ReferenceInfo>& infosOut) = 0;
		virtual hkResult calcTypeInfo(const Node* type, hkTrackerLayoutCalculator* calc, hkTrackerLayoutTypeInfo& typeInfo) = 0;
};

class hkTrackerLayoutCalculator: public hkReferencedObject
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(hkTrackerLayoutCalculator);
		typedef hkTrackerTypeTreeNode Node;


			/// Add a handler
		void addHandler(const char* name, hkTrackerLayoutHandler* handler);
			/// Get the handler associated with type name
		hkTrackerLayoutHandler* getHandler(const hkSubString& name) const;

			/// Get the layout of the type (can only be class/struct types)
		hkTrackerTypeLayout* createLayout(const Node* node);	
			/// Get the layout of the type
		const hkTrackerTypeLayout* getLayout(const Node* node);		
			/// Set the layout - replaces if there is one already there.
		void setLayout(const Node*, const hkTrackerTypeLayout* layout);
			/// Appends info about known objects referenced by this type to infosOut
		void calcReferencedTypes(const Node* node, const hkTrackerSnapshot* snapshot, void* data, hkArray<hkTrackerLayoutHandler::ReferenceInfo>& infosOut);
			/// For a types which are unknown
		void calcUnknownTypes(const Node* type, int baseOffset, int flags, hk_size_t size, hkArray<hkTrackerTypeLayout::Member>& membersOut);
			/// Returns true if this is a known type
		hkBool isKnownType(const Node* type);

			/// Remove all the handlers, and layout map associated with the calculator
		void clear();
			/// Work out a types size. Return 0 if not known.
		hk_size_t calcTypeSize(const Node* type);
			/// Set a size for a type
		void setTypeSize(const Node* type, hk_size_t size);

			/// This will recursively work out all of the references (i.e., pointers) in the type. The names will NOT be set.
		void calcMembers(const Node* type, hk_size_t size, hkArray<hkTrackerTypeLayout::Member>& membersOut, int baseIndex = 0, int flags = 0);
		/// Works out the name of members. The names are stored in buffer (null terminated) at offsets specified in namesOffset.
		void calcMemberNames(const Node* type, hkArray<char>& buffer, hkArray<int>& namesOffset);

			/// Goes through all types 'flattening them' (so all members are pointers), and such that they have the correct names
		void flattenTypes();

			/// Calculate type info
		hkResult calcTypeInfo(const Node* type, hkTrackerLayoutTypeInfo& typeInfo);

			/// Get the type cache
		hkTrackerTypeTreeCache* getTypeCache() const { return m_typeCache; }

			/// Returns true if type is derived from the base class
		hkBool isDerived(const hkSubString& type, const hkSubString& baseClass) const;

			/// Ctor
		hkTrackerLayoutCalculator(hkTrackerTypeTreeCache* typeCache); 
			/// Dtor
		~hkTrackerLayoutCalculator();

			/// Returns the alignment for a basic type (0 otherwise)
		static hk_size_t HK_CALL calcBasicAlignment(Node::Type);
			/// Returns the size of a basic type (0 otherwise)
		static hk_size_t HK_CALL calcBasicSize(Node::Type type);

	protected:
		hkTrackerTypeLayout* _createClassLayout(const Node* type);
		const hkTrackerTypeLayout* _getLayout(const Node* type) const { return  m_layoutMap.getWithDefault(type, HK_NULL); }
			/// Determines a types size, by looking for member in a type that has the size
		hk_size_t _calcTypeSizeFromMember(const Node* type);
		void _calcMemberNames(const Node* type, hkStringBuf& baseName, hkArray<char>& buffer, hkArray<int>& namesOffset);

		hkPointerMap<const Node*, const hkTrackerTypeLayout*> m_layoutMap;
		hkPointerMap<const Node*, int> m_reportedType;			///< Holds map of all types which have been reported that their layout couldn't be created

		hkRefPtr<hkTrackerTypeTreeCache> m_typeCache;
		hkStorageStringMap<hkTrackerLayoutHandler*> m_handlers;

		hkPointerMap<const Node*, hk_size_t> m_sizeMap;			///< Maps a type to its size
};

#endif // HKBASE_LAYOUT_CALCULATOR_H

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
