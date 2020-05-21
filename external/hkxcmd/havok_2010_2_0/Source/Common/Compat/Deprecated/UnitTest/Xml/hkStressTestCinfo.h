/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef DEMOS_TEST_UNITTEST_HKSERIALIZE_XML_HKSTRESSTESTCINFO_XML_H
#define DEMOS_TEST_UNITTEST_HKSERIALIZE_XML_HKSTRESSTESTCINFO_XML_H

#include <Common/Base/hkBase.h>
#include <Common/Base/hkBase.h>

extern const class hkClass hkStressTestCinfoStructWithArraysClass;

extern const class hkClass hkStressTestCinfoStructWithVtableClass;

extern const class hkClass hkStressTestCinfoStructWithVirtualFunctionsClass;

extern const class hkClass hkStressTestCinfoSimpleStructClass;

/// hkStressTestCinfo meta information
extern const class hkClass hkStressTestCinfoClass;

/// A class to test all parts of the serialization infrastructure.
class hkStressTestCinfo
{
	public:
		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, hkStressTestCinfo);

			///
		enum AnEnum
		{
				///
			VAL_INVALID,
				///
			VAL_TEN=10,
				///
			VAL_ELEVEN,
				///
			VAL_TWENTY=20
		};

		///
		struct SimpleStruct
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, SimpleStruct);
			///
			hkUint32 m_key;

			///
			hkUint32 m_value;

			SimpleStruct() : m_key(0), m_value(0) {}
			SimpleStruct( const SimpleStruct& s ) : m_key(s.m_key), m_value(s.m_value) {}
		};

			///
		struct AllPodsStruct
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkStressTestCinfo::AllPodsStruct );

				///
			hkBool m_bool;

				///
			hkChar m_char;

				///
			hkInt8 m_int8;

				///
			hkUint8 m_uint8;

				///
			hkInt16 m_int16;

				///
			hkUint16 m_uint16;

				///
			hkInt32 m_int32;

				///
			hkUint32 m_uint32;

				///
			hkInt64 m_int64;

				///
			hkUint64 m_uint64;

				///
			hkReal m_real;

				///
			hkVector4 m_vector4;

				///
			hkQuaternion m_quaternion;

				///
			hkMatrix3 m_matrix3;

				///
			hkRotation m_rotation;

				///
			hkMatrix4 m_matrix4;

				///
			hkTransform m_transform;
		};

		///
		struct StructWithVirtualFunctions
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, StructWithVirtualFunctions);

			virtual ~StructWithVirtualFunctions() {}
			///
			hkUint32 m_value;

			virtual void doesNothingMuch() = 0;
		};

		///
		struct StructWithVtable : public hkStressTestCinfo::StructWithVirtualFunctions
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, StructWithVtable);
			///
			hkUint32 m_newvalue;

			virtual void doesNothingMuch() { m_newvalue = 1;	}

			StructWithVtable() {}
			StructWithVtable( const StructWithVtable& s ) : StructWithVirtualFunctions(s), m_newvalue(s.m_newvalue) {}

			StructWithVtable( hkFinishLoadedObjectFlag flag ) {}
		};


			///
		struct StructWithArrays
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkStressTestCinfo::StructWithArrays );
			HK_DECLARE_REFLECTION();
				///
			hkArray<hkUint32> m_anArray;

				///
			hkArray<char*> m_anArrayOfPointers;

				///
			hkArray<struct StructWithVtable> m_anArrayOfStructs;

			StructWithArrays() {}
			StructWithArrays( const StructWithArrays& s ) {}

		};


			/// Default constructor
	hkStressTestCinfo() { }

		//
		// Members
		//
	public:

			///
		hkBool m_simpleBool;

			///
		hkChar m_simpleChar;

			///
		hkInt8 m_simpleInt8;

			///
		hkUint8 m_simpleUint8;

			///
		hkInt16 m_simpleInt16;

			///
		hkUint16 m_simpleUint16;

			///
		hkInt32 m_simpleInt32;

			///
		hkUint32 m_simpleUint32;

			///
		hkInt64 m_simpleInt64;

			///
		hkUint64 m_simpleUint64;

			///
		hkReal m_simpleReal;

			///
		hkVector4 m_simpleVector4;

			///
		hkQuaternion m_simpleQuaternion;

			///
		hkMatrix3 m_simpleMatrix3;

			///
		hkRotation m_simpleRotation;

			///
		hkMatrix4 m_simpleMatrix4;

			///
		hkTransform m_simpleTransform;

			///
		void* m_optionalPtr;

			///
		hkEnum<AnEnum, hkInt8> m_simpleEnum;

			///
		const char* m_name;

			///
		const char* m_metaSyntacticVariable;

			///
		struct SimpleStruct m_simpleStruct;

			///
		struct SimpleStruct m_simpleStructCarray[6];

			///
		hkBool* m_simpleBoolPointer;

			///
		hkChar* m_simpleCharPointer;

			///
		hkInt8* m_simpleInt8Pointer;

			///
		hkUint8* m_simpleUint8Pointer;

			///
		hkInt16* m_simpleInt16Pointer;

			///
		hkUint16* m_simpleUint16Pointer;

			///
		hkInt32* m_simpleInt32Pointer;

			///
		hkUint32* m_simpleUint32Pointer;

			///
		hkInt64* m_simpleInt64Pointer;

			///
		hkUint64* m_simpleUint64Pointer;

			///
		hkReal* m_simpleRealPointer;

			///
		hkVector4* m_simpleVector4Pointer;

			///
		hkQuaternion* m_simpleQuaternionPointer;

			///
		hkMatrix3* m_simpleMatrix3Pointer;

			///
		hkRotation* m_simpleRotationPointer;

			///
		hkMatrix4* m_simpleMatrix4Pointer;

			///
		hkTransform* m_simpleTransformPointer;

			///
		hkArray<hkChar> m_arrayCharEmpty;

			///
		hkArray<hkInt8> m_arrayInt8Empty;

			///
		hkArray<hkUint8> m_arrayUint8Empty;

			///
		hkArray<hkInt16> m_arrayInt16Empty;

			///
		hkArray<hkUint16> m_arrayUint16Empty;

			///
		hkArray<hkInt32> m_arrayInt32Empty;

			///
		hkArray<hkUint32> m_arrayUint32Empty;

			///
		hkArray<hkInt64> m_arrayInt64Empty;

			///
		hkArray<hkUint64> m_arrayUint64Empty;

			///
		hkArray<hkReal> m_arrayRealEmpty;

			///
		hkArray<hkVector4> m_arrayVector4Empty;

			///
		hkArray<hkQuaternion> m_arrayQuaternionEmpty;

			///
		hkArray<hkMatrix3> m_arrayMatrix3Empty;

			///
		hkArray<hkRotation> m_arrayRotationEmpty;

			///
		hkArray<hkMatrix4> m_arrayMatrix4Empty;

			///
		hkArray<hkTransform> m_arrayTransformEmpty;

			///
		hkArray<hkBool> m_arrayBoolEmpty;

			///
		hkArray<hkReal> m_arrayRealWithIntializer;

			///
		hkArray<hkVector4> m_arrayVector4WithIntializer;

			///
		hkBool m_simpleCarrayBoolEmpty[5];

			///
		hkChar m_simpleCarrayCharEmpty[5];

			///
		hkInt8 m_simpleCarrayInt8Empty[5];

			///
		hkUint8 m_simpleCarrayUint8Empty[5];

			///
		hkInt16 m_simpleCarrayInt16Empty[5];

			///
		hkUint16 m_simpleCarrayUint16Empty[5];

			///
		hkInt32 m_simpleCarrayInt32Empty[5];

			///
		hkUint32 m_simpleCarrayUint32Empty[5];

			///
		hkInt64 m_simpleCarrayInt64Empty[5];

			///
		hkUint64 m_simpleCarrayUint64Empty[5];

			///
		hkReal m_simpleCarrayRealEmpty[5];

			///
		hkVector4 m_simpleCarrayVector4Empty[5];

			///
		hkQuaternion m_simpleCarrayQuaternionEmpty[5];

			///
		hkMatrix3 m_simpleCarrayMatrix3Empty[5];

			///
		hkRotation m_simpleCarrayRotationEmpty[5];

			///
		hkMatrix4 m_simpleCarrayMatrix4Empty[5];

			///
		hkTransform m_simpleCarrayTransformEmpty[5];

			///
		hkReal m_simpleCarrayRealOneInit[5];

			///
		hkReal m_simpleCarrayRealFullInit[5];

			///
		hkArray<struct SimpleStruct*> m_arrayStructPtrs;

			///
		hkArray<struct SimpleStruct> m_arrayStructEmpty;

			///
		struct SimpleStruct m_carrayStructEmpty[5];

			///
		struct SimpleStruct m_carrayStructInit[5];

			///
		hkInt8* m_simpleArray;
		hkInt32 m_numSimpleArray;

			///
		hkUint16 m_serializeAsZero;

			///
		hkUint16* m_serializePointerAsZero;

			///
		hkArray<hkUint16> m_serializeArrayAsZero;

			///
		//struct StructWithVtable m_structWithVtable;

			///
		struct StructWithArrays m_structWithArrays;

			///
		//struct StructWithArrays m_cArrayOfStructsWithArrays[5];
};

hkBool::CompileTimeTrueType hkIsVirtual(hkStressTestCinfo::StructWithVirtualFunctions*);

#endif // DEMOS_TEST_UNITTEST_HKSERIALIZE_XML_HKSTRESSTESTCINFO_XML_H

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
