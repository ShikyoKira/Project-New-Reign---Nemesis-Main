/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_OBJECT_COPIER_H
#define HK_SERIALIZE_OBJECT_COPIER_H

#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Serialize/hkObjectWriter.h>

class hkOArchive;

/// Copy objects using hkClass information.
/// This is an extremely general purpose copier which can
/// convert on the fly between layouts for different compilers,
/// platforms with different pointer sizes and endianess.
class hkObjectCopier : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Option flags for how to copy objects.
		enum ObjectCopierFlagBits
		{
				/// Normal behavior, copy all reflected members.
			FLAG_NONE = 0x0,

				/// Apply the default to variables which have the SERIALIZE_IGNORED bit set.
				/// Otherwise they get zeroed.
			FLAG_APPLY_DEFAULT_IF_SERIALIZE_IGNORED = 0x1,

				/// Don't copy members which have the SERIALIZE_IGNORED bit set.
				/// Normally all reflected members are copied.
			FLAG_RESPECT_SERIALIZE_IGNORED = 0x2,
		};

			/// A type for combining the flags from ObjectCopierFlagBits.
		typedef hkFlags<ObjectCopierFlagBits,hkUint32> ObjectCopierFlags;

			/// Create an hkObjectCopier which will copy between the specified layouts.
		hkObjectCopier(const hkStructureLayout& layoutIn, const hkStructureLayout& layoutOut, ObjectCopierFlags flags = FLAG_NONE);

			/// Destroy an hkObjectCopier.
		virtual ~hkObjectCopier();

			/// Copy a single object using class information.
			/// The object data is appended to "dataOut". Relocations
			/// are appended to "reloc".
		virtual hkResult copyObject(const void* dataIn, const hkClass& klassIn,
			hkStreamWriter* dataOut, const hkClass& klassOut, hkRelocationInfo& reloc );

			/// Get the source layout.
		const hkStructureLayout& getLayoutOut() const { return m_layoutOut; }

			/// Get the target layout.
		const hkStructureLayout& getLayoutIn() const { return m_layoutIn; }

	protected:

		virtual hkBool32 areMembersCompatible(const hkClassMember& src, const hkClassMember& dst);

	private:

		void writeZero( hkOArchive& oa, const hkClassMember& member );
		int saveBody( const void* dataIn, const hkClass& klassIn,
			hkOArchive& dataOut, const hkClass& klassOut );
		void saveExtras( const void* dataIn, const hkClass& klassIn,
			hkOArchive& dataOut, const hkClass& klassOut,
			int classStart, hkRelocationInfo& fixups, int level = 0 );

			/// 
		virtual const hkClass* lookupClass( const hkClass& klass )
		{
			return &klass;
		}

	protected:

		hkStructureLayout m_layoutIn;
		hkStructureLayout m_layoutOut;
		ObjectCopierFlags m_flags;
		hkBool m_byteSwap;
};

#endif //HK_SERIALIZE_OBJECT_COPIER_H

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
