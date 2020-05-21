/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_IARCHIVE_H
#define HK_BASE_IARCHIVE_H

class hkStreamReader;

/// Endian-aware binary formatted data reader.
/// The data may optionally be byteswapped as it is read.
/// The default is to read in little endian format.
class hkIArchive: public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_STREAM);
		
			/// Constructs a new hkIArchive with the specified hkStreamReader.
		hkIArchive(hkStreamReader* sb, hkBool byteswap=HK_ENDIAN_BIG);

			/// Uses the hkFileSystem to get an hkStreamReader for filename.
		hkIArchive(const char* filename, hkBool byteswap=HK_ENDIAN_BIG);

			/// Uses the 'mem' inplace. 'mem' should be valid for the lifetime of this object.
		hkIArchive(const void* mem, int memSize, hkBool byteswap=HK_ENDIAN_BIG);

			/// Destructor.
		~hkIArchive();

			//
			// Read single elements.
			//

			/// Reads 8 bits.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkChar read8();

			/// Reads 8 bits.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkUchar read8u();

			/// Reads 16 bits.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkInt16 read16();

			/// Reads 16 bits.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkUint16 read16u();

			/// Reads 32 bits.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkInt32 read32();

			/// Reads 32 bits.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkUint32 read32u();

			/// Reads 64 bits
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkInt64 read64();

			/// Reads 64 bits.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkUint64 read64u();

			/// Reads 32 bit floats.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkFloat32 readFloat32();

			/// Reads 64 bit doubles.
			/// Value is undefined in the case of error / eof. Use isOk() to validate.
		HK_FORCE_INLINE hkDouble64 readDouble64();

			//
			// Read array elements.
			//

			/// Reads array of 8 byte data.
		HK_FORCE_INLINE void readArray8(hkInt8* buf, int nelem);

			/// Reads array of 8 byte data.
		HK_FORCE_INLINE void readArray8u(hkUint8* buf, int nelem);

			/// Reads array of 16 byte data.
		HK_FORCE_INLINE void readArray16(hkInt16* buf, int nelem);

			/// Reads array of 16 byte data.
		HK_FORCE_INLINE void readArray16u(hkUint16* buf, int nelem);
		
			/// Reads array of 32 byte data.
		HK_FORCE_INLINE void readArray32(hkInt32* buf, int nelem);

			/// Reads array of 32 byte data.
		HK_FORCE_INLINE void readArray32u(hkUint32* buf, int nelem);

			/// Reads array of 64 byte data.
		HK_FORCE_INLINE void readArray64(hkInt64* buf, int nelem);

			/// Reads array of 64 byte data.
		HK_FORCE_INLINE void readArray64u(hkUint64* buf, int nelem);

			/// Reads array of 32 byte float data.
		HK_FORCE_INLINE void readArrayFloat32(hkFloat32* buf, int nelem);

			/// Reads array of 64 byte float data.
		HK_FORCE_INLINE void readArrayDouble32u(hkDouble64* buf, int nelem);

			/// Reads array of sizeelem byte data.
		void readArrayGeneric(void* buf, int sizeelem, int nelem);

			//
			// Other.
			//

			/// Reads raw data.
		int readRaw( void* buf, int nbytes);

			/// Set byteswapping.
		HK_FORCE_INLINE void setByteSwap(hkBool on);

			/// Set byteswapping.
		HK_FORCE_INLINE hkBool getByteSwap() const;

			/// Returns the current error status of the stream.
			/// Notice that end-of-file will only be detected after an unsuccessful read
		hkBool isOk() const;
		
			/// Returns the underlying streambuf used by this hkIstream
		hkStreamReader* getStreamReader();

			/// Sets the underlying hkStreamReader for this hkIstream
		void setStreamReader(hkStreamReader* newReader);

	protected:

			/// The underlying stream reader.
		hkStreamReader* m_streamReader;

			/// Should we byteswap.
		hkBool m_byteSwap;
};

typedef hkIArchive hkIfArchive;

#include <Common/Base/System/Io/IArchive/hkIArchive.inl>

#endif // HK_BASE_IARCHIVE_H

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
