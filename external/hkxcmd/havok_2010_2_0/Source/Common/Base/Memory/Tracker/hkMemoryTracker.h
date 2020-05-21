/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_MEMORY_TRACKER_H
#define HKBASE_MEMORY_TRACKER_H

struct hkSubString;
class hkMemorySystem;

class hkMemoryTracker
{
	//+reflected(false)
	//+hk.MemoryTracker(ignore=True)
    public:
        HK_DECLARE_PLACEMENT_ALLOCATOR();

		typedef hkMemoryRouter::Allocator Allocator;

		struct Member //+reflected(false)
        {
			//+hk.MemoryTracker(ignore=True)
			enum
			{
				FLAG_BACK_POINTER = 1
			};
            hkUint16 m_offset;                  ///< Offset from object start (in bytes)
            hkUint16 m_memberSize;              ///< Size of the member - this is the only way I can see if a member is a reference
            const char* m_typeName;				///< The type of the field
			const char* m_name;					///< The name of the field
			hkUint32 m_flags;
        };

		// Basic types are guarenteed to not have any pointers (such as enums, or structs that contain only ints/floats)
		// NOTE! Do not change the order of these members without updating the init structure
		struct TypeDefinition //+reflected(false)
		{
			//+hk.MemoryTracker(ignore=True)
			enum Type
			{
				TYPE_BASIC,							///< Contains no pointers to anything else
				TYPE_CLASS,							///< Is a class (or struct). May contain pointers.
				TYPE_SCAN,							///< Its a class that contains variable amount of pointers. Will be scanned.
			};
			const char* m_typeName;		///< The type
			const char* m_library;					///< The library this type belongs to
			hkUint16 m_size;						///< The size
			hkUint8 m_alignment;					///< the alignment
			hkUint8 m_type;							///< The type
		};
		struct ClassDefinition: public TypeDefinition //+reflected(false)
        {
			//+hk.MemoryTracker(ignore=True)
			// Extra members
            const Member* m_members;                ///< The members that belong to this class (or HK_NULL)
            const char* m_parentTypeName;			///< The parent classes name (or HK_NULL if doesn't have one)
            
			hkUint16 m_numMembers;                  ///< The number of members
			hkUint8 m_isVirtual;					///< True if the type is virtual
			hkInt8 m_parentOffset;					///< Num bytes from this type to the parent (often 0, but not always). I.e., add to this to get to the parent.
        };
		struct InitClassDefinition //+reflected(false)
		{
			//+hk.MemoryTracker(ignore=True)
			// TypeDefinition
			const char* m_typeName;					///< The type
			const char* m_library;					///< The library this belongs to
			hkUint16 m_size;						///< The size
			hkUint8 m_alignment;					///< the alignment
			hkUint8 m_type;							///< The type

			// Class definition
			const Member* m_members;                ///< The members that belong to this class (or HK_NULL)
            const char* m_parentTypeName;			///< The parent classes name (or HK_NULL if doesn't have one)
            
			hkUint16 m_numMembers;                  ///< The number of members
			hkUint8 m_isVirtual;					///< True if the type is virtual
			hkInt8 m_parentOffset;					///< Num bytes from this type to the parent (often 0, but not always). I.e., add to this to get to the parent.
		};

            /// New hkReferenced derived object
        virtual void onNewReferencedObject(const char* typeName, hk_size_t size, void* ptr) = 0;
            /// Delete hkReferenced derived object
        virtual void onDeleteReferencedObject(void* ptr) = 0;

            /// New object
        virtual void onNewObject(const char* typeName, hk_size_t size, void* ptr) = 0;
            /// Delete hkReferenced derived object
        virtual void onDeleteObject(void* ptr) = 0;

            /// Add a class definition. NOTE the clsDef structure, and its members are assumed to be stay in scope for
            /// the life of the tracker.
        virtual void addTypeDefinition(const TypeDefinition& def)= 0;
            /// Find a class definition
        virtual const TypeDefinition* findTypeDefinition(const char* name) = 0;

			/// For type determination
		virtual const hkClass* getClassType() const { return HK_NULL; }

			/// Get all of the class. Returns number of definitions. Passing typeDefinition as HK_NULL just returns size.
		virtual hk_size_t getTypeDefinitions(const TypeDefinition** typeDefinitions) = 0;

			/// Find a class definition from a regular c string
		const TypeDefinition* findTypeDefinition(const hkSubString& name);

			/// Returns true if its a basic (i.e., has no pointers etc) type.
		hkBool isBasicType(const hkSubString& name);

			/// Get the class definition
		const ClassDefinition* findClassDefinition(const hkSubString& name);
		const ClassDefinition* findClassDefinition(const char* name);

            /// Virtual Dtor
        virtual ~hkMemoryTracker() {}

			/// Get the instance - may return null if tracking is not initialized.
		static inline hkMemoryTracker* HK_CALL getInstancePtr() { return s_singleton; }
			/// Get the instance - if not set, returns a dummy tracker that ignores all events.
		static hkMemoryTracker& HK_CALL getInstance();
            /// Set the instance
        static void HK_CALL setInstance(hkMemoryTracker* tracker);


			/// Returns true if the system has the allocation in its memory walk (if doesn't implement memory walk will return true)
		static hkBool HK_CALL hasAllocationInMemoryWalk(hkMemorySystem* system, void* ptr);

    protected:

        static hkMemoryTracker* s_singleton;
};

#endif // HKBASE_MEMORY_TRACKER_H

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
