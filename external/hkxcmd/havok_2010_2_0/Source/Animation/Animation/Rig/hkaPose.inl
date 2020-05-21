/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifdef HK_DEBUG
	#define CHECK_INVARIANT()					checkPoseValidity();
	#define CHECK_INTERNAL_FLAG_IS_CLEAR(f)		checkInternalFlagIsClear(F_BONE_INTERNAL_FLAG##f);
#else
	#define CHECK_INVARIANT()					;
	#define CHECK_INTERNAL_FLAG_IS_CLEAR(f)		;
#endif


const hkaSkeleton* hkaPose::getSkeleton() const
{
	return m_skeleton;
}

void hkaPose::syncAll() const
{
	syncLocalSpace();
	syncModelSpace();
}

int hkaPose::isFlagOn (int boneIdx, hkaPoseFlag enumToCheck) const
{
	return m_boneFlags[boneIdx] & enumToCheck;
}

void hkaPose::setFlag (int boneIdx, hkaPoseFlag enumToCheck) const
{
	m_boneFlags[boneIdx] |= enumToCheck;
}

void hkaPose::clearFlag (int boneIdx, hkaPoseFlag enumToCheck) const
{
	m_boneFlags[boneIdx] &= static_cast<hkaPoseFlag>(~enumToCheck);
}

// Explicit versions of the above where we pass the flag to be modified/axmines
int hkaPose::isFlagOnExplicit (hkaPoseFlag flag, hkaPoseFlag enumToCheck) const
{
	return flag & enumToCheck;
}

void hkaPose::setFlagExplicit (hkaPoseFlag& flag, hkaPoseFlag enumToCheck) const
{
	flag |= enumToCheck;
}

void hkaPose::clearFlagExplicit (hkaPoseFlag& flag, hkaPoseFlag enumToCheck) const
{
	flag &= static_cast<hkaPoseFlag>(~enumToCheck);
}




void hkaPose::clearInternalFlags() const
{
	const int numBones = m_boneFlags.getSize();
	for (int i=0; i<numBones; ++i)
	{
		m_boneFlags[i] &= (~M_BONE_INTERNAL_FLAGS);
	}
}

hkaPose& hkaPose::operator= ( const hkaPose& other )
{
	m_skeleton = other.m_skeleton;
	m_localPose = other.m_localPose;
	m_modelPose = other.m_modelPose;
	m_boneFlags = other.m_boneFlags;
	m_modelInSync = other.m_modelInSync;
	m_localInSync = other.m_localInSync;
	m_floatSlotValues = other.m_floatSlotValues;

	return *this;
}


const hkQsTransform& hkaPose::getBoneModelSpace (int boneIdx) const
{
	if (!isFlagOn (boneIdx, F_BONE_MODEL_DIRTY))
	{
		return m_modelPose[boneIdx];
	}
	else
	{
		return calculateBoneModelSpace(boneIdx);
	}
}


const hkQsTransform& hkaPose::getBoneLocalSpace (int boneIdx) const
{
	if (! isFlagOn (boneIdx, F_BONE_LOCAL_DIRTY))
	{
		return m_localPose[boneIdx];
	}
	else
	{
		const hkQsTransform& modelFromBone = m_modelPose[boneIdx];
		HK_ASSERT2(0x5a3281f6, ! isFlagOn (boneIdx, F_BONE_MODEL_DIRTY), "Trying to access uninitialized bone in pose" );

		const hkInt16 parentIdx = m_skeleton->m_parentIndices[boneIdx];

		if (parentIdx != -1)
		{
			// ask for parent model (may be dirty)
			const hkQsTransform& modelFromParent = getBoneModelSpace(parentIdx);

			// parentfrombone = inv(modelfromparent) * modelfrombone
			m_localPose[boneIdx].setMulInverseMul(modelFromParent, modelFromBone);

		}
		else
		{
			m_localPose[boneIdx] = modelFromBone;
		}

		clearFlag (boneIdx, F_BONE_LOCAL_DIRTY);

		CHECK_INVARIANT();

		return m_localPose[boneIdx];
	}
}

void hkaPose::makeAllChildrenLocalSpace(int boneIdx) const
{
	CHECK_INTERNAL_FLAG_IS_CLEAR(1);

	const int numBones = m_skeleton->m_bones.getSize();

	setFlag(boneIdx, F_BONE_INTERNAL_FLAG1);

	// Synchronize all descendants local representation
	{
		for (int i= boneIdx+1; i<numBones; ++i)
		{
			const hkInt16 parentId = m_skeleton->m_parentIndices[i];
			if (parentId != -1 && isFlagOn(parentId, F_BONE_INTERNAL_FLAG1))
			{
				getBoneLocalSpace( i ); // sync local
				setFlag( i, F_BONE_INTERNAL_FLAG1);
				m_modelInSync = false;
			}
		} 
	}

	// Dirty all descendants model representation
	{
		for (int i= boneIdx+1; i < numBones; ++i)
		{
			hkaPoseFlag f = m_boneFlags[i];
			if (isFlagOnExplicit( f, F_BONE_INTERNAL_FLAG1))
			{
				setFlagExplicit( f, F_BONE_MODEL_DIRTY);
				clearFlagExplicit( f, F_BONE_INTERNAL_FLAG1);
				m_boneFlags[i] = f;
			}
		}
	}

}

void hkaPose::makeFirstChildrenModelSpace(int boneIdx) const
{
	const int numBones = m_skeleton->m_bones.getSize();

	for (int i= boneIdx+1; i<numBones; ++i)
	{
		const hkInt16 parentId = m_skeleton->m_parentIndices[i];
		if (parentId == boneIdx)
		{
			getBoneModelSpace( i ); // sync model
			m_boneFlags[i] = F_BONE_LOCAL_DIRTY;
			m_localInSync = false;
		}
	}

}

void hkaPose::setBoneLocalSpace (int boneIdx, const hkQsTransform& boneLocal)
{
	// The model transform will be invalidated for all descendants
	// Make also sure the local transform for those is in sync
	makeAllChildrenLocalSpace(boneIdx);

	m_localPose[boneIdx] = boneLocal;

	m_boneFlags[boneIdx] = F_BONE_MODEL_DIRTY;

	m_modelInSync = false;

	CHECK_INVARIANT();

}

void hkaPose::setBoneModelSpace (int boneIdx, const hkQsTransform& boneModel, PropagateOrNot propagateOrNot)
{
	if (! propagateOrNot )
	{
		// The local transform will be invalidated for the first generation of descendants
		// Make also sure the model transform for those is in sync
		makeFirstChildrenModelSpace(boneIdx);
	}
	else
	{
		// Act as if were actually setting the transform in local space
		makeAllChildrenLocalSpace(boneIdx);
	}

	m_modelPose[boneIdx] = boneModel;

	m_boneFlags[boneIdx] = F_BONE_LOCAL_DIRTY;

	m_localInSync = false;

	CHECK_INVARIANT();

}

hkaPose::hkaPose (const hkaSkeleton* skeleton)
	: m_skeleton(skeleton), m_modelInSync(false), m_localInSync (false)
{
	const int numBones = m_skeleton->m_bones.getSize();
	const int numBonesRoundedUp = HK_NEXT_MULTIPLE_OF( 4, numBones );

	m_modelPose.reserveExactly(numBonesRoundedUp);
	m_localPose.reserveExactly(numBonesRoundedUp);
	m_boneFlags.reserveExactly(numBonesRoundedUp);
	m_modelPose.setSize(numBones);
	m_localPose.setSize(numBones);
	m_boneFlags.setSize(numBones);

	const int numSlots =  m_skeleton->m_floatSlots.getSize();
	const int numSlotsRoundedUp = HK_NEXT_MULTIPLE_OF( 4, numSlots );
	m_floatSlotValues.reserveExactly( numSlotsRoundedUp );
	m_floatSlotValues.setSize( numSlots, 0.0f );


#ifdef HK_DEBUG
	setNonInitializedFlags();
#endif

}

int hkaPose::getRequiredMemorySize (const hkaSkeleton* skeleton)
{
	const int numBonesRoundedUp = HK_NEXT_MULTIPLE_OF( 4, skeleton->m_bones.getSize() );

	const int floatSizeRoundedUp = HK_NEXT_MULTIPLE_OF( 4, skeleton->m_floatSlots.getSize());

	const int totalSize =  numBonesRoundedUp * ( 2 * hkSizeOf(hkQsTransform) + hkSizeOf(hkaPoseFlag) )
		+ floatSizeRoundedUp * hkSizeOf(hkReal);

	return totalSize;
}


hkaPose::hkaPose(const hkaSkeleton* skeleton, void* preallocatedMemory)
	:	m_skeleton  (skeleton), 
		m_localPose (reinterpret_cast<hkQsTransform*> (preallocatedMemory), skeleton->m_bones.getSize(), skeleton->m_bones.getSize()),
		m_modelPose (m_localPose.begin() + skeleton->m_bones.getSize(), skeleton->m_bones.getSize(), skeleton->m_bones.getSize()),
		m_boneFlags (reinterpret_cast<hkaPoseFlag*> (m_modelPose.begin() + skeleton->m_bones.getSize()), skeleton->m_bones.getSize(), skeleton->m_bones.getSize()),
		m_modelInSync (false),
		m_localInSync (false),
		m_floatSlotValues (reinterpret_cast<hkReal*> (m_boneFlags.begin() + skeleton->m_bones.getSize()), skeleton->m_floatSlots.getSize(), HK_NEXT_MULTIPLE_OF( 4, skeleton->m_floatSlots.getSize()))
{
	HK_ASSERT2 (0x4643f989, (reinterpret_cast<hkUlong> (preallocatedMemory) & 0xf) == 0, "Preallocated memory must be 16-byte aligned");

#ifdef HK_DEBUG
	setNonInitializedFlags();
#endif
}

const hkArray<hkReal>& hkaPose::getFloatSlotValues() const
{
	return m_floatSlotValues;
}

void hkaPose::setFloatSlotValues(const hkArrayBase<hkReal>& floatSlotValues)
{
	m_floatSlotValues = floatSlotValues;
}

hkReal hkaPose::getFloatSlotValue(int floatSlotIdx) const
{
	return m_floatSlotValues[ floatSlotIdx ];
}

void hkaPose::setFloatSlotValue(int floatSlotIdx, hkReal value)
{
	m_floatSlotValues[ floatSlotIdx ] = value;
}

hkArray<hkReal>& hkaPose::getFloatSlotValues()
{
	return m_floatSlotValues;
}

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
