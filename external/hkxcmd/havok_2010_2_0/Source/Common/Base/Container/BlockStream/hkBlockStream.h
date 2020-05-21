/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BLOCKSTREAM_H
#define HK_BLOCKSTREAM_H

#include <Common/Base/Container/BlockStream/hkBlockStreamBase.h>

template<typename T, typename Allocator = hkBlockStreamHeapAllocator<512> >
class hkBlockStream: protected hkBlockStreamBase<Allocator>
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkBlockStream );

		hkBlockStream( Allocator* tlAllocator, bool zeroNewBlocks = false ): hkBlockStreamBase<Allocator>( tlAllocator, zeroNewBlocks ){}

		HK_FORCE_INLINE ~hkBlockStream(){}

		HK_FORCE_INLINE bool isEmpty() const { return hkBlockStreamBase<Allocator>::isEmpty(); }

 		class Range;

			///	resets the stream after calling initialize, block stream has 1 empty block.
		void reset( Allocator* tlAllocator ){ hkBlockStreamBase<Allocator>::initialize(tlAllocator); }

			/// append blockstream to this stream and delete inStream
		void clearAndSteal( Allocator* tlAllocator, hkBlockStream* inStream ){ hkBlockStreamBase<Allocator>::clear(tlAllocator); hkBlockStreamBase<Allocator>::append(tlAllocator, inStream); }

			/// clear for no further use
		void clear(Allocator* tlAllocator){ hkBlockStreamBase<Allocator>::clear(tlAllocator); }
		
			/// append blockstream to this stream and delete inStream
		void append( Allocator* tlAllocator, hkBlockStream* inStream ){ hkBlockStreamBase<Allocator>::append(tlAllocator, inStream); }

			/// append multi threaded safe
		void appendMt( Allocator* tlAllocator, hkBlockStream* inStream ){ if ( !inStream->isEmpty() ) { hkBlockStreamBase<Allocator>::appendMt(tlAllocator, inStream); }else{ inStream->clear(tlAllocator); } }

			/// append a list of Ranges
		void append( Allocator* tlAllocator, Range* ranges, int numRanges, int rangeStriding = sizeof(Range) ){ hkBlockStreamBase<Allocator>::append( tlAllocator, (typename hkBlockStreamBase<Allocator>::Marker*)ranges, numRanges, rangeStriding); }

		int getTotalNumElems() const { return hkBlockStreamBase<Allocator>::getTotalNumElems(); }


		/// Helper class to write to a Block stream
		class Writer: protected hkBlockStreamBase<Allocator>::WriteIterator
		{
			public:

					/// Sets this writer to the end of the stream so that the data gets appended
				HK_FORCE_INLINE void setToEndOfStream( Allocator* allocator, hkBlockStream* blockStream){ hkBlockStreamBase<Allocator>::WriteIterator::setToEndOfStream(allocator, blockStream); }

					/// Sets the writer to the start of an empty stream
				HK_FORCE_INLINE void setToStartOfStream( Allocator* allocator, hkBlockStream* blockStream){ hkBlockStreamBase<Allocator>::WriteIterator::setToStartOfStream(allocator, blockStream); }

					/// Write numBytes to the stream, data must be allocated by access first
				HK_FORCE_INLINE void write( const T* HK_RESTRICT data, int numBytes )
				{
					//HK_ASSERT( 0xf04dadde, (numBytes&0xf) == 0);
					T *dstCache = (T*)hkBlockStreamBase<Allocator>::WriteIterator::access( numBytes );
					hkString::memCpy(dstCache, data, numBytes);
					//hkString::memCpy16NonEmpty(dstCache, data, numBytes>>4);
					advance( numBytes );
				}

					/// Write to a stream if data has getSizeInBytes() implemented
				HK_FORCE_INLINE void write( const T* HK_RESTRICT data ){ write( data, data->getSizeInBytes()); }

					/// advance and access the next element
				HK_FORCE_INLINE	T* advanceAndReserveNext( int thisElemSize, int nextElemSize ){ return (T*)hkBlockStreamBase<Allocator>::WriteIterator::advanceAndReserveNext(thisElemSize, nextElemSize ); }

					/// expand the stream by \a numBytes, so that you can write to the pointer returned. This does not advance the stream
				HK_FORCE_INLINE	T* reserve( int numBytes  ){ return (T*)hkBlockStreamBase<Allocator>::WriteIterator::access(numBytes); }

					/// Advance the stream by numBytes, this basically makes the data written to the pointer returned by \a access permanent.
				HK_FORCE_INLINE void advance( int numBytes ){ hkBlockStreamBase<Allocator>::WriteIterator::advance( numBytes ); }

					/// Once you have written to a stream, there might be an empty last bl
				HK_FORCE_INLINE void finalize(){ hkBlockStreamBase<Allocator>::WriteIterator::finalize(); }

				HK_FORCE_INLINE void hackMarkFinalized() { hkBlockStreamBase<Allocator>::WriteIterator::hackMarkFinalized(); }

				HK_FORCE_INLINE void checkConsistency(){ hkBlockStreamBase<Allocator>::WriteIterator::checkConsistency(); }

				HK_FORCE_INLINE int getTotalNumElems(){ return hkBlockStreamBase<Allocator>::WriteIterator::getTotalNumElems(); }

				HK_FORCE_INLINE bool isValid(){ return this->m_blockStream != HK_NULL; }

				friend class Range;
		};

		/// write iterator for fixed size objects, allows for better type checking and simpler accessors
		class FixedSizeWriter : public Writer
		{
			public:

				HK_FORCE_INLINE T* reserve(){ return Writer::reserve( sizeof(T)); }

				HK_FORCE_INLINE void advance(){ Writer::advance( sizeof(T) ); }
		};


		class Reader: protected hkBlockStreamBase<Allocator>::ReadIterator
		{
			public:

					/// Sets the reader to the start of a stream
				HK_FORCE_INLINE void setToStartOfStream( const hkBlockStream* stream ){ hkBlockStreamBase<Allocator>::ReadIterator::setToStartOfStream(stream); }

					/// Sets the reader to a subrange of a stream
				HK_FORCE_INLINE void setToRange( const Range* range ){ hkBlockStreamBase<Allocator>::ReadIterator::setToMarker( HK_NULL, range ); }

					/// advance and access the next element, assumes the current entry is valid
				HK_FORCE_INLINE	const T* advanceAndAccessNext( int thisElemSize ){ return (const T*)hkBlockStreamBase<Allocator>::ReadIterator::advanceAndAccessNext(thisElemSize); }

				HK_FORCE_INLINE	const T* access(){ return (T*)hkBlockStreamBase<Allocator>::ReadIterator::access(); }

					/// Read from a stream if data has getSizeInBytes() implemented
				HK_FORCE_INLINE const T* advanceAndAccessNext( const T* HK_RESTRICT data ){ return advanceAndAccessNext( data->getSizeInBytes()); }

				friend class Range;
		};

		class Modifier: protected hkBlockStreamBase<Allocator>::ModifyIterator
		{
			public:

					/// Sets the reader to the start of a stream
				HK_FORCE_INLINE void setToStartOfStream( Allocator* allocator, hkBlockStream* stream ){ hkBlockStreamBase<Allocator>::ModifyIterator::setToStartOfStream(allocator, stream); }

					/// Sets the reader to a subrange of a stream
				HK_FORCE_INLINE void setToRange( Allocator* allocator, const Range* range ){ hkBlockStreamBase<Allocator>::ModifyIterator::setToMarker( allocator, HK_NULL, range ); }

					/// advance and access the next element, assumes the current entry is valid
				HK_FORCE_INLINE	const T* advanceAndAccessNext( int thisElemSize ){ return (const T*)hkBlockStreamBase<Allocator>::ModifyIterator::advanceAndAccessNext(thisElemSize); }

				HK_FORCE_INLINE	T* advanceAndAccessNextRw( int thisElemSize ){ return (T*)hkBlockStreamBase<Allocator>::ModifyIterator::advanceAndAccessNextRw(thisElemSize); }

				HK_FORCE_INLINE	const T* access(){ return (T*)hkBlockStreamBase<Allocator>::ModifyIterator::access(); }

					/// Read from a stream if data has getSizeInBytes() implemented
				HK_FORCE_INLINE const T* advanceAndAccessNext( const T* HK_RESTRICT data ){ return advanceAndAccessNext( data->getSizeInBytes()); }


				HK_FORCE_INLINE	T* accessRw(){ return (T*)hkBlockStreamBase<Allocator>::ModifyIterator::accessRw(); }

				friend class Range;
		};

		/// A reader, which frees the memory once it is fully read
		class Consumer: public Modifier
		{
			public:

				/// Override to avoid calling the wrong function
				HK_FORCE_INLINE void setToRange( Allocator* allocator, const Range* marker ){ HK_ASSERT( 0x0f345467, false ); }	// use the other version

				/// Sets the consumer to a subrange of a stream
				HK_FORCE_INLINE void setToRange( Allocator* allocator, hkBlockStream* stream, const Range* range ){ hkBlockStreamBase<Allocator>::ModifyIterator::setToMarker( allocator, stream, range ); }


				/// Read the memory but free elements immediately
				HK_FORCE_INLINE	const T* advanceAndAccessNext( int thisElemSize ){ return (const T*)hkBlockStreamBase<Allocator>::ModifyIterator::advanceFreeAndAccessNext(thisElemSize); }
		};

		class FixedSizeConsumer : protected Consumer
		{
			public:
					/// Sets the reader to the start of a stream
				HK_FORCE_INLINE void setToStartOfStream( Allocator* allocator, hkBlockStream* stream ) { Consumer::setToStartOfStream( allocator, stream ); }

					/// Sets the consumer to a subrange of a stream
				HK_FORCE_INLINE void setToRange( Allocator* allocator, hkBlockStream* stream, const Range* range ){ Consumer::setToRange( allocator, stream, range ); }

					/// Read the memory but free elements immediately
				HK_FORCE_INLINE	const T* advanceAndAccessNext() { return Consumer::advanceAndAccessNext( sizeof(T) ); }
		};

		class Range : public hkBlockStreamBase<Allocator>::Marker
		{
			public:
				HK_FORCE_INLINE	void clear(){ hkBlockStreamBase<Allocator>::Marker::clear();}

				HK_FORCE_INLINE bool isEmpty() const { return this->m_numElements == 0; }

				HK_FORCE_INLINE int getNumElements() const { return this->m_numElements; }

				HK_FORCE_INLINE void setStartPoint(const Writer* it ){ hkBlockStreamBase<Allocator>::Marker::setStartPoint( it );	}

				HK_FORCE_INLINE void setNumElements(int num) { this->m_numElements = num; }

				/// set the number of elements
				HK_FORCE_INLINE void setEndPoint(const Writer* it){ hkBlockStreamBase<Allocator>::Marker::setEndPoint( it ); }

				HK_FORCE_INLINE void setEmpty(){ hkBlockStreamBase<Allocator>::Marker::setEmpty( ); }

				HK_FORCE_INLINE void setEntireStream( hkBlockStream<T, Allocator>* HK_RESTRICT stream ){ hkBlockStreamBase<Allocator>::Marker::setEntireStream( stream ); }

				friend class hkBlockStream;
		};

};

#endif

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
