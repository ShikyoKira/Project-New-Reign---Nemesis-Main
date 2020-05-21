/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKARRAY_H
#define HKBASE_HKARRAY_H

#include <Common/Base/Container/Array/hkArrayUtil.h>
#include <Common/Base/Container/hkContainerAllocators.h>

class hkTrackerArrayLayoutHandler;

	/// Base of all Havok array types.
	/// This class is allocator-less. Operations which may change the capacity must supply
	/// an allocator. Derived classes often have an implicit allocator.
	/// Note that, for performance reasons, order may not be preserved when deleting elements.
	///
	/// The hkArray uses type traits for optimized handling the POD (plain old data)
	/// and non-POD (nontrivial constructor and/or destructor) items.
	///
	/// Object lifetimes: Memory at elements [0, getSize()) are live objects.
	/// Memory at elements [getSize(),getCapacity()) is uninitialized.
	///
	/// If the type of the array item is non-pod then the following must be satisfied:
	///     - relocatable with memmove
	///     - user-defined default constructor
	///     - user-defined copy constructor
	///     - user-defined destructor
	///     - user-defined assignment
	///
	/// Because of the first point, note that objects which contain pointers to data within
	/// themselves (such as hkInplaceArray or hkStringBuf) may not be used in a hkArray.
	///
	/// If the class contains member of hkArray type you must implement a copy constructor.
	/// This is because the hkArray copy constructor is private to avoid accidental pass-by-value usage.
	/// Thus the compiler generated copy constructor is unable to access the hkArray copy constructor.
	///
	/// Sometimes you may want to treat your class/struct as a pod type for hkArray
	/// performance reasons.
	///
	/// To do this you should use one of the following macros:
	///     - HK_DECLARE_POD_TYPE();
	///       It must be specified in public area of the class body.
	///     - HK_DECLARE_POD_TYPE_IF_POD( T );
	///       It must be specified in public area of the class
	///       template body, where the T is the template argument.
	///     - HK_DEFINE_AS_POD_TYPE( typename );
	///       It must be specified outside the class body. This macro can be used in
	///       the cpp file to define the class as a pod type locally.
	///
	/// Note: Please make sure that the class does not contain non-pod members when you
	/// define it as a pod.
	/// 
	/// Note: For class/struct defined as pod, the requirements for constructors and destructors
	/// are not applicable.
template <typename T>
class hkArrayBase
{
		friend class hkArrayUtil;
		friend class hkArraySpuUtil;
		friend class hkTrackerArrayLayoutHandler;
		template<typename TYPE> friend class hkArraySpu;

	public:

		typedef hkArrayBase<T> ThisType;
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ARRAY, ThisType);

			/// Creates a zero length array.
		HK_FORCE_INLINE hkArrayBase();

			/// Noncopying initialization from an existing external buffer.
			/// This does not copy the array but uses it in place until its capacity
			/// is exceeded at which point a reallocation occurs and the array behaves
			/// like a normal hkArray.
			/// The caller must ensure that the buffer is valid for the lifetime
			/// of this array and for deallocation of the buffer.
		HK_FORCE_INLINE hkArrayBase(T* buffer, int size, int capacity);

	protected:

			/// Array cannot be copied without explicit allocator.
		HK_FORCE_INLINE hkArrayBase(const hkArrayBase& a)  { HK_ASSERT(0x54a6d8d1, 0); }

			/// Array cannot be copied without explicit allocator.
		HK_FORCE_INLINE hkArrayBase& operator= (const hkArrayBase& a) { HK_ASSERT(0x54a6d8d0, 0); return *this; }

	public:

			/// Destructs array members.
		HK_FORCE_INLINE ~hkArrayBase();

			/// Read/write access to the i'th element.
		HK_FORCE_INLINE T& operator[] (int i);

			/// Read only access to the i'th element.
		HK_FORCE_INLINE const T& operator[] (int i) const;

			/// Read/write access to the last element.
		HK_FORCE_INLINE T& back();

			/// Read only access to the last element.
		HK_FORCE_INLINE const T& back() const;

			/// Returns the size.
		HK_FORCE_INLINE int getSize() const;

			/// Returns the capacity.
		HK_FORCE_INLINE int getCapacity() const;

			/// Checks if the size is zero.
		HK_FORCE_INLINE hkBool isEmpty() const;

			/// Sets the size to zero.
		HK_FORCE_INLINE void clear();

			/// Sets the size to zero and deallocates storage.
		HK_FORCE_INLINE void _clearAndDeallocate(hkMemoryAllocator& alloc);

			/// Tries to reduce the capacity to avoid wasting storage. If shrinkExact is true the resulting capacity
			/// is size+numElementsLeft
		HK_FORCE_INLINE void _optimizeCapacity( hkMemoryAllocator& alloc, int numFreeElemsLeft, hkBool32 shrinkExact=false );

			/// Removes the element at the specified index. The last array element is used to replace the removed element, and the size is reduced by 1.
			/// This is very fast, but note that the order of elements is changed.
		HK_FORCE_INLINE void removeAt(int index);

			/// Removes the element at the specified index, copying elements down one slot as in the STL array.
			/// Slower than removeAt(), but the order is unchanged.
		HK_FORCE_INLINE void removeAtAndCopy(int index);

			/// Removes several elements at the specified index, copying elements down as in the STL array.
		HK_FORCE_INLINE void removeAtAndCopy(int index, int numToRemove);

			/// Returns the index of the first occurrence of t, or -1 if not found.
		HK_FORCE_INLINE int indexOf(const T& t, int start=0, int end=-1) const;

			/// Returns index of the last occurrence of t, or -1 if not found.
		HK_FORCE_INLINE int lastIndexOf(const T& t) const;

			/// Removes the last element.
		HK_FORCE_INLINE void popBack( int numElemsToRemove = 1 );

			/// Adds an element to the end.
		HK_FORCE_INLINE void _pushBack(hkMemoryAllocator& alloc, const T& e);

			/// Adds an element to the end. No check for resize.
		HK_FORCE_INLINE void pushBackUnchecked(const T& e);

			/// Ensures no reallocation occurs until at least size n.
		HK_FORCE_INLINE void _reserve(hkMemoryAllocator& alloc, int n);

			/// Ensures no reallocation occurs until size n.
		HK_FORCE_INLINE void _reserveExactly(hkMemoryAllocator& alloc,int n);

			/// Sets the size.
			/// If the array is expanded, new elements are uninitialized.
		HK_FORCE_INLINE void _setSize(hkMemoryAllocator& alloc, int size);

			/// Sets the size to n.
			/// If the array is expanded, new elements initialized with 'fill'.
		HK_FORCE_INLINE void _setSize(hkMemoryAllocator& alloc, int n, const T& fill);

			/// Sets the size assuming the capacity to be sufficient.
			/// If the array is expanded, new elements are uninitialized.
		HK_FORCE_INLINE void setSizeUnchecked(int size);

			/// Increments the size by 1 and returns a reference to the first element created.
		HK_FORCE_INLINE T& _expandOne(hkMemoryAllocator& alloc);

			/// Increments the size by n and returns a pointer to the first element created.
		HK_FORCE_INLINE T* _expandBy( hkMemoryAllocator& alloc, int n );

			/// Increments the size by n and returns a pointer to the first element created. No check for resize!
		HK_FORCE_INLINE T* expandByUnchecked( int n );

			/// Expands the array by numToInsert at the specified index.
		HK_FORCE_INLINE T* _expandAt( hkMemoryAllocator& alloc, int index, int numToInsert );			

			/// Inserts the array a at index i.
		void _insertAt(hkMemoryAllocator& alloc, int i, const T* a, int numElems );

			/// Inserts t at index i.
			/// Elements from i to the end are copied up one place.
		void _insertAt(hkMemoryAllocator& alloc, int i, const T& t);

			/// Appends the array a.
		void _append(hkMemoryAllocator& alloc, const T* a, int numElems );

			/// Replaces elements [i,i+ndel) with the supplied array.
			/// This method avoids redundant copying associated with separate remove & insert steps.
		void _spliceInto(hkMemoryAllocator& alloc, int i, int ndel, const T* p, int numElems );


			/// Removes all occurrences of t, preserving the order of the remaining elements.
		HK_FORCE_INLINE void removeAllAndCopy(const T& t);

			/// Adds the element at the end of the array, or returns false if the array would have
			/// to be resized first.
		HK_FORCE_INLINE hkBool tryPushBack( const T& t );

			///
		typedef T* iterator;
			///
		typedef const T* const_iterator;

			/// Returns an STL-like iterator to the first element.
		HK_FORCE_INLINE iterator begin();
			/// Returns an STL-like iterator to the 'one past the last' element.
		HK_FORCE_INLINE iterator end();
			/// Returns an STL-like const iterator to the first element.
		HK_FORCE_INLINE const_iterator begin() const;
			/// Returns an STL-like const iterator to the 'one past the last' element.
		HK_FORCE_INLINE const_iterator end() const;

		static HK_FORCE_INLINE void HK_CALL copy(T* dst, const T* src, int n);

			/// Set the storage of the array to use the provided
			/// external memory allocation. Set size and capacity as provided
			/// by the user. Existing array data is forgotten without deallocating
			/// or destructing objects, use clearAndDeallocate if required.
			/// The array objects must be of POD type. In particular, destructors will
			/// be called on any objects remaining in the array when it is destructed
			/// The memory block is owned by the Array and will be deleted on destruction
			/// or resizing
		HK_FORCE_INLINE void setDataAutoFree(T* ptr, int size, int capacity);

			/// Set the storage of the array to use the provided
			/// external memory allocation. Set size and capacity as provided
			/// by the user. Existing array data is forgotten without deallocating
			/// or destructing objects, use clearAndDeallocate if required.
			/// The array objects must be of POD type. In particular, destructors will
			/// be called on any objects remaining in the array when it is destructed
			/// The memory block is not owned by the Array and will not be deleted on destruction
			/// or resizing
		HK_FORCE_INLINE void setDataUserFree(T *ptr, int size, int capacity);

			/// Advanced use only. Set the storage of the array, bypassing
			/// correctness checks. Size, capacity and flags are set as provided.
			/// Note that constructor/destructor semantics of non-POD types may not
			/// be respected.
		HK_FORCE_INLINE void _setDataUnchecked(T *ptr, int size, int capacityAndFlags);

            /// Get the capacity and the flags - advanced use
        HK_FORCE_INLINE int getCapacityAndFlags() const;

	protected:

			// Internal type checking
		HK_FORCE_INLINE void _setData(T *ptr, int size, int capacityAndFlags, hkTypeIsPod);

		HK_FORCE_INLINE hkArrayBase<T>& copyFromArray(hkMemoryAllocator&, const hkArrayBase<T>& src, hkTypeIsPod);
		HK_FORCE_INLINE hkArrayBase<T>& copyFromArray(hkMemoryAllocator&, const hkArrayBase<T>& src, hkTypeIsClass);

	public:

		// Public so that the serialization can access it.
		enum
		{
			CAPACITY_MASK = int(0x3FFFFFFF),
			FLAG_MASK = int(0xC0000000),
			DONT_DEALLOCATE_FLAG = int(0x80000000), // Indicates that the storage is not the array's to delete
			FORCE_SIGNED = -1
		};

	protected:

		friend class hkStatisticsCollector;

		T* m_data;
		int m_size;
		int m_capacityAndFlags; // highest 2 bits indicate any special considerations about the allocation for the array

	public:

			/// For serialization, we want to initialize the vtables
			/// in classes post data load, and NOT call the default constructor
			/// for the arrays (as the data has already been set).
		hkArrayBase(hkFinishLoadedObjectFlag f) { }
};

	/// Array which uses the hkMemoryRouter::heap allocator.
template <typename T, typename Allocator=hkContainerHeapAllocator>
class hkArray : public hkArrayBase<T>
{
	public:

		typedef Allocator AllocatorType;
		friend class hkArrayUtil;

		typedef hkArray<T,hkContainerTempAllocator> Temp;
		typedef hkArray<T,hkContainerDebugAllocator> Debug;
		typedef hkArray<T,hkContainerHeapAllocator> Heap;

			/// Creates a zero length array.
		HK_FORCE_INLINE hkArray() : hkArrayBase<T>() {}

			///
		HK_FORCE_INLINE ~hkArray();

			/// Creates an array of size n. All elements are uninitialized.
		explicit HK_FORCE_INLINE hkArray(int size);

			/// Creates an array of n elements initialized to 'fill'.
		HK_FORCE_INLINE hkArray(int size, const T& fill);

			/// Noncopying initialization from an existing external buffer.
			/// This does not copy the array but uses it in place until its capacity
			/// is exceeded at which point a reallocation occurs and the array behaves
			/// like a normal hkArray.
			/// The caller must ensure that the buffer is valid for the lifetime
			/// of this array and for deallocation of the buffer.
		HK_FORCE_INLINE hkArray(T* buffer, int size, int capacity) : hkArrayBase<T>(buffer, size, capacity) {}

		hkArray(hkFinishLoadedObjectFlag f) : hkArrayBase<T>(f) { }

	protected:

			/// Not publicly accessible, too easy to call accidentally.
		HK_FORCE_INLINE hkArray(const hkArray& a);

	public:

			/// Copy another array
		HK_FORCE_INLINE hkArray& operator= (const hkArrayBase<T>& a);
		HK_FORCE_INLINE hkArray& operator= (const hkArray& a);

			/// Swaps this array's internal storage with 'a'.
		void swap(hkArray& a);

		HK_FORCE_INLINE void clearAndDeallocate() { hkArrayBase<T>::_clearAndDeallocate(AllocatorType().get(this)); }
		HK_FORCE_INLINE void optimizeCapacity(int numFreeElemsLeft, hkBool32 shrinkExact=false) { hkArrayBase<T>::_optimizeCapacity(AllocatorType().get(this), numFreeElemsLeft, shrinkExact); }
		HK_FORCE_INLINE void pushBack(const T& e) { hkArrayBase<T>::_pushBack(AllocatorType().get(this), e); }
		HK_FORCE_INLINE void reserve(int size) { hkArrayBase<T>::_reserve(AllocatorType().get(this), size); }
		HK_FORCE_INLINE void reserveExactly(int size) { hkArrayBase<T>::_reserveExactly(AllocatorType().get(this), size); }
		HK_FORCE_INLINE void setSize(int size) { hkArrayBase<T>::_setSize(AllocatorType().get(this), size); }
		HK_FORCE_INLINE void setSize(int size, const T& fill) { hkArrayBase<T>::_setSize(AllocatorType().get(this), size, fill); }
		HK_FORCE_INLINE T& expandOne() { return hkArrayBase<T>::_expandOne(AllocatorType().get(this)); }
		HK_FORCE_INLINE T* expandBy(int n) { return hkArrayBase<T>::_expandBy(AllocatorType().get(this), n); }
		HK_FORCE_INLINE T* expandAt(int i, int n) { return hkArrayBase<T>::_expandAt(AllocatorType().get(this), i, n); }
		HK_FORCE_INLINE void insertAt(int i, const T& e) { hkArrayBase<T>::_insertAt(AllocatorType().get(this),i,e); }
		HK_FORCE_INLINE void insertAt(int i, const T* a, int numElems ) { hkArrayBase<T>::_insertAt(AllocatorType().get(this),i,a,numElems); }
		HK_FORCE_INLINE void append(const T* a, int numElems ) { hkArrayBase<T>::_append(AllocatorType().get(this),a,numElems); }
		HK_FORCE_INLINE void spliceInto(int i, int ndel, const T* p, int numElems ) { return hkArrayBase<T>::_spliceInto(AllocatorType().get(this),i,ndel,p,numElems); }		
};

	/// Array that has an internal storage capacity within the class itself.
	/// Originally hkArray::m_data points to hkInplaceArray::m_storage.
	/// It is safe to expand the capacity beyond the internal capacity. In this
	/// case the array behaves like a normal hkArray (i.e., m_data points to heap
	/// memory instead of to &m_storage[0]).
	/// Note that once the builtin capacity has been exceeded,
	/// the inplace elements are unused even if subsequently resized smaller
	/// than the original capacity.
template <typename T, unsigned N, typename Allocator=hkContainerHeapAllocator>
class hkInplaceArray : public hkArray<T,Allocator>
{
	public:

		typedef hkInplaceArray<T,N,Allocator> mytype;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ARRAY, mytype );

			/// Creates an array with the specified initial size.
		explicit HK_FORCE_INLINE hkInplaceArray(int size = 0);

		HK_FORCE_INLINE hkInplaceArray(const hkInplaceArray<T,N,Allocator>& a);

		HK_FORCE_INLINE ~hkInplaceArray() {}

			/// Copies the array a.
		HK_FORCE_INLINE hkArray<T,Allocator>& operator= (const hkArrayBase<T>& a);

			/// Copies the array a.
		HK_FORCE_INLINE hkArray<T,Allocator>& operator= (const hkInplaceArray<T,N,Allocator>& a);

			/// Tries to reduce the capacity to avoid wasting storage
		HK_FORCE_INLINE void optimizeCapacity( int numFreeElemsLeft, hkBool32 shrinkExact=false );

		HK_FORCE_INLINE hkBool wasReallocated() const;

			/// returns true if the array is still using its inplace buffer.
			/// This check is done using only the mask field, so do not use
			/// this function for arrays which can be serialized
		HK_FORCE_INLINE int stillInplaceUsingMask() const;

	public:

		T m_storage[N];

	public:

		/// For serialization, we want to initialize the vtables
		/// in classes post data load, and NOT call the default constructor
		/// for the arrays (as the data has already been set).
		hkInplaceArray(hkFinishLoadedObjectFlag f) : hkArray<T,Allocator>(f) { }
};


	/// An array that has a small internal storage capacity, aligned to 16 bytes within the class itself.
template <typename T, unsigned N>
class hkInplaceArrayAligned16 : public hkArray<T>
{
	public:

		typedef hkInplaceArray<T,N> mytype;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ARRAY, mytype);

			/// Creates an array with the specified initial size.
		explicit HK_FORCE_INLINE hkInplaceArrayAligned16(int size = 0);

		HK_FORCE_INLINE ~hkInplaceArrayAligned16(){}

			/// Copies the array a.
		HK_FORCE_INLINE hkArray<T>& operator= (const hkArrayBase<T>& a);

			/// Copies the array a.
		HK_FORCE_INLINE hkArray<T>& operator= (const hkInplaceArrayAligned16<T,N>& a);

		HK_FORCE_INLINE hkBool wasReallocated() const;

	protected:

		int m_padding; // sizeof(base class) + padding == 16 bytes
		HK_ALIGN16( hkUint8 m_storage[sizeof(T) * N] );

	public:

		/// For serialization, we want to initialize the vtables
		/// in classes post data load, and NOT call the default constructor
		/// for the arrays (as the data has already been set).
		hkInplaceArrayAligned16(hkFinishLoadedObjectFlag f) : hkArray<T>(f) { }
};

#include <Common/Base/Container/Array/hkArray.inl>

#endif // HKBASE_HKARRAY_H

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
