/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_RELOCATION_INFO_H
#define HK_RELOCATION_INFO_H

class hkClass;
template<typename T, typename A> class hkStorageStringMap;

/// Contains data needed to move buffer in memory.
class hkRelocationInfo
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, hkRelocationInfo);

			/// Create an empty object.
		hkRelocationInfo()
			: m_pool(HK_NULL)
		{
		}

		hkRelocationInfo(const hkRelocationInfo& r)
		{
			HK_ASSERT(0x4aa2092e,0);
		}

			/// Destroy object.
		~hkRelocationInfo();

			/// Pointer fixups within a buffer.
		struct Local
		{
			HK_DECLARE_POD_TYPE();
			Local(int f, int t)
				:	m_fromOffset(f),
					m_toOffset(t)
			{
			}
				/// source offset in buffer
			int m_fromOffset;
				/// destination offset in buffer
			int m_toOffset;
		private:
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkRelocationInfo::Local );
		};

			/// Pointer fixups to a location outside the buffer.
		struct Global
		{
			HK_DECLARE_POD_TYPE();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkRelocationInfo::Global);
			Global( int f, void* ta, const hkClass* k, hkBool32 related )
				:	m_fromOffset(f),
					m_toAddress(ta),
					m_toClass(k),
					m_related(related)
			{
			}
				/// source offset in buffer
			int m_fromOffset;
				/// dest pointer in global address
			void* m_toAddress;
				/// type of pointed to object
			const hkClass* m_toClass;
				/// indicates if object must be kept near the source
			hkBool32 m_related;
		};

			/// Objects which will need a "finish" step on load.
		struct Finish
		{
			HK_DECLARE_POD_TYPE();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkRelocationInfo::Finish);
			Finish( int f, const char* k )
				:	m_fromOffset(f),
					m_className(k)
			{
			}
				/// Object start in buffer
			int m_fromOffset;
				/// name of fixup
			const char* m_className;
		};

			/// Pointers to objects not in this data block.
		struct Import
		{
			HK_DECLARE_POD_TYPE();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkRelocationInfo::Import);
			Import( int fromOffset, const char* identifier )
				:	m_fromOffset(fromOffset),
					m_identifier(identifier)
			{
			}
				/// source offset in buffer
			int m_fromOffset;
				/// unique identifier
			const char* m_identifier;
		};

			/// Add a local relocation.
		void addLocal( int fromOffset, int toOffset )
		{
			m_local.pushBack( Local(fromOffset,toOffset) );
		}

			/// Add a global relocation.
		void addGlobal( int fromOffset, void* toAddr, const hkClass* klass, hkBool32 related = false )
		{
			m_global.pushBack( Global(fromOffset, toAddr, klass, related ) );
		}

			/// Add a finish marker.
		void addFinish( int fromOffset, const char* klassName )
		{
			m_finish.pushBack( Finish(fromOffset, klassName ) );
		}

			/// Add an import marker.
		void addImport( int fromOffset, const char* name );

			/// Apply local and global relocations to the supplied buffer.
		void applyLocalAndGlobal( void* buffer );

			/// Clear all lists.
		void clear()
		{
			m_local.clear();
			m_global.clear();
			m_finish.clear();
			m_imports.clear();
		}

	public:

		hkArray<Local> m_local;
		hkArray<Global> m_global;
		hkArray<Finish> m_finish;
		hkArray<Import> m_imports;
		hkStorageStringMap<int, hkContainerHeapAllocator>* m_pool;
};

#endif // HK_RELOCATION_INFO_H

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
