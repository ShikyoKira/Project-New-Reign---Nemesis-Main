/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_DATAOBJECT_IMPL_H
#define HK_SERIALIZE_DATAOBJECT_IMPL_H

#include <Common/Serialize/Data/hkDataObject.h>
#include <Common/Base/Reflection/hkClassMemberAccessor.h>
#include <Common/Base/Types/hkTypedUnion.h>

//////////////////////////////////////////////////////////////////////////
// Single threaded refcounted - refcount starts at zero
//////////////////////////////////////////////////////////////////////////

	/// This class defines private reference counting
	/// for objects that define and manage reflected data.
class hkDataRefCounted
{
	//+hk.MemoryTracker(ignore=True)
	public:

		HK_DECLARE_CLASS_ALLOCATOR_UNCHECKED(HK_MEMORY_CLASS_SERIALIZE, heap);

		virtual ~hkDataRefCounted() { HK_ASSERT(0x6ee903de, (m_count&~1)==0 ); }
		hkDataRefCounted() : m_externalCount(0), m_count(0) {}
		void addReference() const
		{
			HK_ASSERT(0x5557d721, m_count >= 0);
			++m_count;
		}
		void removeReference() const
		{
			HK_ASSERT(0x5557d720, m_count > 0);
			if(--m_count == 0 )
			{
				HK_ASSERT(0x5557d722, m_externalCount == 0);
				delete this;
			}
		}
		int getReferenceCount() const
		{
			return m_count;
		}

		void addExternalReference() const
		{
			HK_ASSERT(0x5557d72b, m_externalCount >= 0);
			++m_externalCount;
			addReference();
		}
		void removeExternalReference() const
		{
			HK_ASSERT(0x5557d72a, m_externalCount > 0);
			--m_externalCount;
			removeReference();
		}
		int getExternalReferenceCount() const
		{
			return m_externalCount;
		}

	private:
		mutable hkUint16 m_memSize;
		mutable hkInt16 m_externalCount;
		mutable hkInt32 m_count;
};


//////////////////////////////////////////////////////////////////////////
// Array
//////////////////////////////////////////////////////////////////////////

	/// This abstract class defines common private interfaces
	/// required to manage a reflected array of data.
class hkDataArrayImpl : public hkDataRefCounted
{
	//+hk.MemoryTracker(ignore=True)
	public:
		virtual hkDataObject::Type getType() const = 0;
		virtual void clear() = 0;
		virtual void reserve( int n ) = 0;
		virtual void setSize(int size) = 0;
		virtual int getSize() const = 0;
		virtual const hkDataClassImpl* getClass() const = 0;

		virtual int getUnderlyingIntegerSize() const { HK_ASSERT2(0x4845a142, 0, "internal error, should not be reached"); return 0; }

			// for arrays of homogeneous types (ARRAY_STRUCT) extract the member from each object in
			// the array into a virtual array.
		virtual hkDataArrayImpl* swizzleObjectMember(const char* name) const { HK_ASSERT(0x4a2c61dc, 0); return HK_NULL; }

#		define ITEM_ACCESSOR(TYPE, NAME, UMEMBER) \
				virtual TYPE as##NAME(int index) const { HK_ASSERT(0x5d74effa, 0); return (TYPE)(0); } \
				virtual void set##NAME(int index, TYPE val) { HK_ASSERT(0x7cf76c5a, 0); }
			ITEM_ACCESSOR(const hkReal*, Vec, ra);
			ITEM_ACCESSOR(const char*, String, s);
			ITEM_ACCESSOR(hkReal, Real, r);
			ITEM_ACCESSOR(hkHalf, Half, r);
			ITEM_ACCESSOR(int, Int, i);
			ITEM_ACCESSOR(hkInt64, Int64, i);
			ITEM_ACCESSOR(hkDataObjectImpl*, Object, o);
			ITEM_ACCESSOR(hkDataArrayImpl*, Array, a);
#		undef ITEM_ACCESSOR

#		define ARRAY_SET(A) virtual void setAll(const A*, int n) { HK_ASSERT(0x227cd5bd, 0); }
			ARRAY_SET(hkBool);
			ARRAY_SET(char);
			ARRAY_SET(hkInt8);
			ARRAY_SET(hkUint8);
			ARRAY_SET(hkInt16);
			ARRAY_SET(hkUint16);
			ARRAY_SET(hkInt32);
			ARRAY_SET(hkUint32);
			ARRAY_SET(hkInt64);
			ARRAY_SET(hkUint64);
			ARRAY_SET(hkReal);
			ARRAY_SET(hkHalf);
#		undef ARRAY_SET

		void set( int index, hkDataObject::Value val )
		{
			if( getType() & hkDataObject::TYPE_TUPLE )
			{
				HK_ASSERT(0x132f8fef, val.asArray().getSize() == hkDataArray(asArray(index)).getSize());
				setArray(index, val.asArray().getImplementation());
				return;
			}
			switch( getType() )
			{
				case hkDataObject::TYPE_BYTE:
				case hkDataObject::TYPE_INT:
					setInt(index, val.asInt());
					break;
				case hkDataObject::TYPE_REAL:
					setReal(index, val.asReal());
					break;
				case hkDataObject::TYPE_VEC_4:
				case hkDataObject::TYPE_VEC_8:
				case hkDataObject::TYPE_VEC_12:
				case hkDataObject::TYPE_VEC_16:
				{
					int nr = hkDataObject::getNumRealsFromType( getType() );
					setVec( index, val.asVec(nr) );
					break;
				}
				case hkDataObject::TYPE_OBJECT:
				case hkDataObject::TYPE_STRUCT:
					setObject( index, val.asObject().getImplementation() );
					break;
				case hkDataObject::TYPE_CSTRING:
					setString( index, val.asString());
					break;
				default:
					HK_ASSERT(0x77ca1cb5, 0);
			}
		}
};

////////////////////////////////////////////////////////////////////////
// Class
//////////////////////////////////////////////////////////////////////////

	/// This abstract class defines common private interfaces
	/// required to define reflected data.
class hkDataClassImpl : public hkDataRefCounted
{
	//+hk.MemoryTracker(ignore=True)
	public:

		virtual const hkDataWorld* getWorld() const = 0;
		virtual const char* getName() const = 0;
		virtual int getVersion() const = 0;
		virtual hkDataClassImpl* getParent() const = 0;

		virtual hkBool isSuperClass(const hkDataClassImpl* k) const = 0;

		// this class only
		virtual int getNumDeclaredMembers() const = 0;
		virtual void getDeclaredMemberInfo(int i, hkDataClass::MemberInfo& info) const = 0;
		virtual int getDeclaredMemberIndexByName(const char* name) const = 0;

		// all members
		virtual int getNumMembers() const = 0;
		virtual void getMemberInfo(int i, hkDataClass::MemberInfo& info) const = 0;
		virtual int getMemberIndexByName(const char* name) const = 0;
};


//////////////////////////////////////////////////////////////////////////
// Object
//////////////////////////////////////////////////////////////////////////

	/// This abstract class defines common private interfaces
	/// required to manage reflected data.
class hkDataObjectImpl : public hkDataRefCounted
{
	//+hk.MemoryTracker(ignore=True)
	public:

		typedef hkDataObject::Iterator Iterator;

		virtual hkDataObject::Handle getHandle() const = 0;
		virtual const hkDataClassImpl* getClass() const = 0;
		virtual hkDataObject::Value accessByName(const char* name) = 0;
		virtual hkBool32 hasMember(const char* name) const = 0;

		virtual Iterator getMemberIterator() const = 0; //DEFAULT?
		virtual hkBool32 isValid(Iterator it) const = 0;
		virtual Iterator getNextMember(Iterator it) const = 0;
		virtual const char* getMemberName(Iterator it) const = 0;
		virtual const hkDataObject::Value getMemberValue(Iterator it) const = 0; //DEFAULT?

		virtual hkDataArrayImpl* asArray( const char* name ) = 0;
		virtual const char* asString( const char* name ) = 0;
		virtual hkInt64 asInt( const char* name ) = 0;
		virtual hkDataObjectImpl* asObject( const char* name ) = 0;
		virtual const hkReal* asVec( const char* name, int nreal ) = 0;
		virtual hkReal asReal( const char* name ) = 0;

		virtual void assign( const char* name, const hkDataObject::Value& valueIn ) = 0;
		virtual void assign( const char* name, hkDataObjectImpl* value ) = 0;
		virtual void assign( const char* name, hkDataArrayImpl* value ) = 0;
		virtual void assign( const char* name, const char* value ) = 0;
		virtual void assign( const char* name, hkReal r ) = 0;
		virtual void assign( const char* name, hkHalf r ) = 0;
		virtual void assign( const char* name, const hkReal* r, int nreal ) = 0;
		virtual void assign( const char* name, int valueIn ) = 0;
		virtual void assign( const char* name, hkInt64 valueIn ) = 0;
};

#endif // HK_SERIALIZE_DATAOBJECT_IMPL_H

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
