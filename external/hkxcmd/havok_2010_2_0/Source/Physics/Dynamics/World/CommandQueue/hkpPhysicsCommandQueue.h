/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_PHYSICS_COMMAND_QUEUE_H
#define HK_DYNAMICS2_PHYSICS_COMMAND_QUEUE_H

#include <Physics/Dynamics/World/CommandQueue/hkpPhysicsCommand.h>

template< typename TYPE>
class hkpRelativePointer32
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpRelativePointer32 );

		hkpRelativePointer32(){}

		inline ~hkpRelativePointer32(){}

		void set( TYPE* pntr){ m_relativeAddress = hkGetByteOffsetInt( this, pntr); }

		inline operator TYPE*() const
		{
			return (TYPE*)hkAddByteOffsetConst(this, m_relativeAddress);
		}

		HK_ALWAYS_INLINE TYPE* val() const
		{
			return (TYPE*)hkAddByteOffsetConst(this, m_relativeAddress);
		}

	public:
		hkUint32 m_relativeAddress;
};

class hkpPhysicsCommandQueue
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpPhysicsCommandQueue );
#if defined(HK_ARCH_X64)
		enum { BYTES_PER_COMMAND = 32 };
#else
		enum { BYTES_PER_COMMAND = 16 };
#endif
		HK_FORCE_INLINE  hkpPhysicsCommandQueue( hkpPhysicsCommand* bufferStart, int capacity );
		HK_FORCE_INLINE  hkpPhysicsCommandQueue(){}
		HK_FORCE_INLINE ~hkpPhysicsCommandQueue(){}

		HK_FORCE_INLINE void init(hkpPhysicsCommand* bufferStart, int capacity);

	public:
		template<typename COMMAND_STRUCT> 
		HK_FORCE_INLINE void addCommand(COMMAND_STRUCT command);

		template<typename COMMAND_STRUCT> 
		HK_FORCE_INLINE COMMAND_STRUCT& newCommand();

	public:
		hkpRelativePointer32<hkpPhysicsCommand> m_start;
		hkUint32 m_size;
		hkUint32 m_capacity;
};

template<int NUM_BYTES>
class hkpFixedSizePhysicsCommandQueue: public hkpPhysicsCommandQueue
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpFixedSizePhysicsCommandQueue );

		hkpFixedSizePhysicsCommandQueue();

		inline ~hkpFixedSizePhysicsCommandQueue(){}

	public:
		HK_ALIGN16( hkUchar m_buffer[NUM_BYTES] );
};

#include <Physics/Dynamics/World/CommandQueue/hkpPhysicsCommandQueue.inl>


#endif // HK_DYNAMICS2_PHYSICS_COMMAND_QUEUE_H

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
