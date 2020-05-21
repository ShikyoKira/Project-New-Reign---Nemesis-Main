/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_STREAM_WRITER_H
#define HKBASE_STREAM_WRITER_H


extern const class hkClass hkStreamWriterClass;

/// A generic interface to writing a stream of bytes.
/// The reader may optionally support buffering or may be
/// wrapped as a child stream of an hkBufferedStreamWriter.
class hkStreamWriter : public hkReferencedObject
{
	public:
		//+serializable(false)  
		//+vtable(true)
		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_STREAM);


		hkStreamWriter() {}
			/// Return false if end of file has been reached or some other error.
			/// Otherwise return true.
		virtual hkBool isOk() const = 0;

			/// Write nbytes. Returns the number of bytes written.
			/// The number of bytes returned may be less than nbytes
			/// (in the case of nonblocking sockets for instance).
		virtual int write(const void* buf, int nbytes) = 0;

			/// Flush any internal buffers.
		virtual void flush() { }

			/// Return true if seeking is supported on this stream. By default not supported.
		virtual hkBool seekTellSupported() const;

			/// Parameter for seek method.
		enum SeekWhence { STREAM_SET=0, STREAM_CUR=1, STREAM_END=2 };

			/// Seek to offset from whence.
		virtual hkResult seek(int offset, SeekWhence whence);

			/// Get the current file offset if supported or -1 on error.
			/// Default implementation calls seek(0, STREAM_CUR);
		virtual int tell() const;
};

#endif //HKBASE_STREAM_WRITER_H

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
