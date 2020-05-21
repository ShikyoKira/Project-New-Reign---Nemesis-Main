/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKISTREAM_H
#define HKBASE_HKISTREAM_H

#include <Common/Base/hkBase.h>

extern const class hkClass hkIstreamClass;

class hkStreamReader;
class hkMemoryTrack;

/// Text formatted data reader. Provides functionality similar to std::istream.
/// All the usual operators are provided plus operators
/// for 64 bit integers. printf style output is also
/// supported.
class hkIstream : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_STREAM);

			/// Constructs an hkIstream using the given hkStreamReader.
			/// Adds a reference to the reader.
		explicit hkIstream(hkStreamReader* sr);

			/// An istream which is connected to a file.
		explicit hkIstream(const char* filename);

			/// An istream which is connected to an existing buffer.
			/// The buffer must exist for the lifetime of this istream.
		explicit hkIstream(const void* mem, int memSize);

			/// An istream which is connected to an existing memory track.
			/// The buffer must exist for the lifetime of this istream.
		explicit hkIstream(const hkMemoryTrack* track );

			/// Destroys the stream
			/// Removes a reference to its reader.
		~hkIstream();

			/// Checks that this stream has not had a read error.
		hkBool isOk() const;

			/// Inputs an hkBool.
		hkIstream& operator>> (hkBool& b);

			/// Inputs a char.
		hkIstream& operator>> (char& c);

			/// Inputs a signed char.
		hkIstream& operator>> (signed char& c);

			/// Inputs unsigned char.
		hkIstream& operator>> (unsigned char& c);

			/// Inputs a short.
		hkIstream& operator>> (short& s);

			/// Input an unsigned short.
		hkIstream& operator>> (unsigned short& s);

			/// Inputs an int.
		hkIstream& operator>> (int& i);

			/// Inputs an unsigned int.
		hkIstream& operator>> (unsigned int& u);

			/// Inputs a float (reads in full double and casts back)
		hkIstream& operator>> (float& f);

			/// Inputs a double.
			// Will actually read a float
		hkIstream& operator>> (double& d); 

			/// Inputs a 64 bit int.
		hkIstream& operator>> (hkInt64& i);

			/// Input a 64 bit unsigned int
			// Will actually read an int64
		hkIstream& operator>> (hkUint64& u);

			/// Inputs an hkStringBuf
		hkIstream& operator>> (hkStringBuf& str);

			/// Inputs an hkStringPtr
		hkIstream& operator>> (hkStringPtr& str);

			/// Gets a string with the given delimiter. Return number of chars read, or -1 if maxsize was reached
			/// without finding the delimiter. Note that any leading whitespace is discarded.
		int getline(char* str, int maxsize, char delim = '\n');
		
			/// Reads in one character.
		hkIstream& get( char& c );
		
			/// Reads raw data.
		int read( void* buf, int nbytes );
			
			/// Returns the underlying hkStreamReader used by this hkIstream.
		hkStreamReader* getStreamReader();

			/// Sets the underlying hkStreamReader for this hkIstream.
		void setStreamReader(hkStreamReader* newReader);

	protected:

		hkStreamReader* m_streamReader;
};

typedef hkIstream hkIfstream;

#include <Common/Base/System/Io/IStream/hkIStream.inl>

#endif // HKBASE_HKISTREAM_H

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
