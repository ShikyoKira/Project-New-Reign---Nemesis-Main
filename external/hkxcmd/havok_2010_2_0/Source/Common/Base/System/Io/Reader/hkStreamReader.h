/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKSTREAMREADER_H
#define HKBASE_HKSTREAMREADER_H

/// A generic interface to reading a stream of bytes.
/// The reader may optionally support mark/rewind or may be
/// wrapped as a child stream of an hkBufferedStreamReader.
/// Derived classes need only override isOk() and read(). Usually
/// readers which do not support buffering are wrapped in an
/// hkBufferedStreamReader.
class hkStreamReader : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_STREAM);


		hkStreamReader() {}
			/// Return false after we have tried to read past the end of file or some other error.
			/// Otherwise return true.
		virtual hkBool isOk() const = 0;

			/// Read 'nbytes' into the buffer 'buf'.
			/// Return the number of characters read or zero for error.
		virtual int read(void* buf, int nbytes) = 0;

			/// Skip nbytes bytes of input.
			/// Return the actual number of bytes skipped.
		virtual int skip(int nbytes);

			/// Read a single char. Return the character or -1 on error.
		int readChar();

			/// Return true if marks may be set. By default marks are not supported.
			/// Derived classes may override this and setMark, rewindToMark.
			/// You can make any class support the mark interface by wrapping it
			/// in a hkBufferedStreamReader.
		virtual hkBool markSupported() const;

			/// Remember a point in the input stream. Rewind to it using rewindToMark().
			/// At most marklimit characters are stored. If more than marklimit
			/// characters are read before a call to rewindToMark() is made or
			/// seek is called then the mark is discarded.
			/// Calling setMark() overwrites the previous mark if it exists.
			/// This may fail if markLimit is larger than the buffer size.
		virtual hkResult setMark(int markLimit);

			/// Rewind to a position marked with setMark().
			/// If a mark is set and no more than marklimit characters have been
			/// read, rewind the stream and return HK_SUCCESS. It is permitted to
			/// call rewindToMark() more than once.
			/// If more than marklimit characters have been read, the stream is not
			/// rewound and HK_FAILURE is returned.
		virtual hkResult rewindToMark();


			/// Return true if seeking is supported on this stream. By default not supported.
		virtual hkBool seekTellSupported() const;

			/// Parameter for seek method.
		enum SeekWhence { STREAM_SET=0, STREAM_CUR=1, STREAM_END=2 };

			/// Seek to offset from whence.
		virtual hkResult seek(int offset, SeekWhence whence);

			/// Get the current file offset if supported or -1 on error.
		virtual int tell() const;
};

#endif //HKBASE_HKSTREAMREADER_H

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
