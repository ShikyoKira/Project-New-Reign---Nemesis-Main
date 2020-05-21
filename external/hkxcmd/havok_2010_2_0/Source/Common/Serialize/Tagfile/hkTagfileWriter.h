/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_TAGFILE_WRITER_H
#define HK_TAGFILE_WRITER_H

class hkDataObject;

class hkTagfileWriter : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Called when a data object is about to be added to the pending writes.
		class AddDataObjectListener : public hkReferencedObject
		{
			public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				/// Called when a data object is about to be added.
				/// The implementation may return new object/type if required,
				/// otherwise it should return passed object.
				/// If obj is NULL, the object is skipped.
				virtual hkDataObject addDataObjectCallback( const hkDataObject& object ) = 0;
		};

		struct Options
		{
			Options():
				m_verbose(false),
				m_exact(true),
				m_writeSerializeIgnored(false)
			{
			}
				/// Set verbose
			Options& useVerbose(hkBool verbose) { m_verbose = verbose; return *this; }	
				/// Set exact
			Options& useExact(hkBool exact) { m_exact = exact; return *this; }
				/// Set write serialize ignored
			Options& useWriteSerializeIgnored(hkBool enable) { m_writeSerializeIgnored = enable; return *this; }

			hkBool m_verbose;					///< Add extra information to make more human readable (if a text file type)
			hkBool m_exact;						///< If set will write in a format such that the data when written and read should be identical
			hkBool m_writeSerializeIgnored;
		};

			/// Save the contents to the given stream.
		virtual hkResult save( const hkDataObject& obj, hkStreamWriter* stream, AddDataObjectListener* userListener, const Options& options = Options()) = 0;
};

#endif // HK_TAGFILE_WRITER_H

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
