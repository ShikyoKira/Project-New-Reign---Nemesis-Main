/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_BASE_OARCHIVE_H
#define HK_BASE_OARCHIVE_H


class hkStreamWriter;


/// Endian-aware binary formatted data writer.
/// Outputs all data in little endian format and converts on the fly if necessary.
class hkOArchive: public hkReferencedObject
{
	public:


		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_STREAM);
		
			/// Create with the specified hkStreamWriter.
		hkOArchive(hkStreamWriter* sb, hkBool byteswap=HK_ENDIAN_BIG);

			/// Create with a stream from the hkStreamWriter factory.
			/// The file is truncated.
		hkOArchive(const char* filename, hkBool byteswap=HK_ENDIAN_BIG);

			/// Create with a memory block. 'mem' must exist for the lifetime of this object.
		hkOArchive(void* mem, int memSize, hkBool byteswap=HK_ENDIAN_BIG);

			/// Creates with an expanding memory block. 'arr' must exist for the lifetime of this object.
		hkOArchive(hkArray<char>& arr, hkBool byteswap=HK_ENDIAN_BIG);

			/// Destructor.
		~hkOArchive();

			//
			// Write single elements.
			//

			/// Writes 8 bits.
		void write8(hkChar c);

			/// Writes 8 bits.
		void write8u(hkUchar u);
		
			/// Writes 16 bits.
		void write16(hkInt16 i);

			/// Writes 16 bits.
		void write16u(hkUint16 u);

			/// Writes 32 bits.
		void write32(hkInt32 i);

			/// Writes 32 bits.
		void write32u(hkUint32 u);

			/// Write 64 bits.
		void write64(hkInt64 i);

			/// Writes 64 bits.
		void write64u(hkUint64 u);

			/// Writes 32 bit IEEE floats.
		void writeFloat32(hkFloat32 f);

			/// Writes 64 bit IEEE doubles.
		void writeDouble64(hkDouble64 d);

			//
			// Write array elements.
			//

			/// Writes array of 8 byte data.
		void writeArray8(const hkInt8* buf, int nelem);

			/// Writes array of 8 byte data.
		void writeArray8u(const hkUint8* buf, int nelem);

			/// Writes array of 16 byte data.
		void writeArray16(const hkInt16* buf, int nelem);

			/// Writes array of 16 byte data.
		void writeArray16u(const hkUint16* buf, int nelem);
		
			/// Writes array of 32 byte data.
		void writeArray32(const hkInt32* buf, int nelem);

			/// Writes array of 32 byte data.
		void writeArray32u(const hkUint32* buf, int nelem);

			/// Writes array of 64 byte data.
		void writeArray64(const hkInt64* buf, int nelem);

			/// Writes array of 64 byte data.
		void writeArray64u(const hkUint64* buf, int nelem);

			/// Writes array of 32 byte float data.
		void writeArrayFloat32(const hkFloat32* buf, int nelem);

			/// Writes array of 64 byte float data.
		void writeArrayDouble64(const hkDouble64* buf, int nelem);

			/// Writes array of sizeelem byte data.
		void writeArrayGeneric(const void* buf, int sizeelem, int nelem);

			//
			// Other.
			//

			/// Writes raw data.
		int writeRaw(const void* buf, int nbytes);

			/// Set byteswapping.
		void setByteSwap(hkBool on);

			/// Set byteswapping.
		hkBool getByteSwap() const;

			/// Returns the current error status of the stream.
		hkBool isOk() const;

			/// Returns the underlying hkStreamWriter used by this hkOArchive.
		hkStreamWriter* getStreamWriter();

			/// Set the underlying hkStreamWriter for this hkOArchive
		void setStreamWriter(hkStreamWriter* writer);

	protected:

			/// The underlying hkStreamWriter.
		hkStreamWriter*		m_writer;

			/// Should we byteswap.
		hkBool m_byteSwap;
};		

typedef hkOArchive hkOfArchive;

#endif // HK_BASE_OARCHIVE_H

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
