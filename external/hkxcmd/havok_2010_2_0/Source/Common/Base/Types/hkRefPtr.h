/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKREFPTR_H
#define HKBASE_HKREFPTR_H


	/// A simple helper class to automatically add and remove references to classes
template <class TYPE>
class hkRefPtr
{
	public:
		typedef hkRefPtr<TYPE> ThisType;
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, ThisType );

			/// Default constructor.
			/// Stored pointer is set to HK_NULL.
		HK_FORCE_INLINE hkRefPtr();

			/// Copy constructor.
			/// Increase reference count for object in 'rp' and set pointer to it.
		HK_FORCE_INLINE hkRefPtr(const hkRefPtr& rp);

			/// Finish constructor.
		HK_FORCE_INLINE hkRefPtr (hkFinishLoadedObjectFlag );

			/// Constructor from pointer.
			/// Increase reference count for object 'e' and set the pointer to it.
		HK_FORCE_INLINE hkRefPtr(TYPE* e);

			/// Destructor.
			/// Decrease reference count for stored object.
		HK_FORCE_INLINE ~hkRefPtr();

			/// Assignment operator.
			/// Increase reference count for object in 'rp',
			/// decrease reference count for stored object and set pointer to object from 'rp'.
		HK_FORCE_INLINE void operator=(const hkRefPtr& rp);

			/// Assignment operator.
			/// Increase reference count for object 'e',
			/// decrease reference count for stored object and set pointer to 'e'.
		HK_FORCE_INLINE void operator=(TYPE* e);

			/// Return pointer to stored object.
		HK_FORCE_INLINE TYPE* val() const;

			/// Pointer to stored object.
		HK_FORCE_INLINE TYPE* operator->() const;

			/// Replace stored pointer with 'e' without incrementing reference count for 'e'.
			/// Reference count for previously stored object is decreased.
		HK_FORCE_INLINE void setAndDontIncrementRefCount( TYPE* e );

			/// Return pointer to stored object.
		HK_FORCE_INLINE operator TYPE*() const;

	private:

		TYPE* m_pntr;
};

#include <Common/Base/Types/hkRefPtr.inl>

#endif // HKBASE_HKREFPTR_H

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
