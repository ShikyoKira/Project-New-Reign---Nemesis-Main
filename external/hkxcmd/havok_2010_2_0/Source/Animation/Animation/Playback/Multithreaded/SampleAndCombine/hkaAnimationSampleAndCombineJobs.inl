/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkaAnimationSampleAndCombineJobBase::hkaAnimationSampleAndCombineJobBase( JobSubType type, hkUint16 size )
	: hkJob( HK_JOB_TYPE_ANIMATION_SAMPLE_AND_COMBINE, type, size, HK_JOB_SPU_TYPE_ENABLED ),
	  m_bufferSize(DEFAULT_MAX_COMPRESSED_ANIM_DATA)
{
	// Should run setRunOnSpuOrPpu() to set to ENABLED.
	m_jobSpuType = HK_JOB_SPU_TYPE_DISABLED;
}


hkaAnimationSampleAndCombineJob::hkaAnimationSampleAndCombineJob()
	: hkaAnimationSampleAndCombineJobBase( hkaAnimationSampleAndCombineJobBase::ANIMATION_SAMPLE_AND_COMBINE_JOB_SAMPLE_AND_COMBINE, sizeof(hkaAnimationSampleAndCombineJob) )
{
	m_animatedSkeletonOnCPU = HK_NULL;
	m_animationControls = HK_NULL;
	m_referencePose = HK_NULL;
	m_referencePoseWeightThreshold = 0.0f;
	m_numSkeletonBones = 0;
	m_parentIndices = HK_NULL;

	m_maxBone = 0;
	m_maxFloatSlot = 0;

	m_numAnimationControls = 0;
	m_maxTransformTracksInAllAnimations = 0;
	m_maxFloatTracksInAllAnimations = 0;

	m_cache = HK_NULL;

	m_poseOut = HK_NULL;
	m_jobDoneSemaphore= HK_NULL;
	m_jobDoneFlag = 0;
}


hkaAnimationSampleOnlyJob::hkaAnimationSampleOnlyJob()
	: hkaAnimationSampleAndCombineJobBase( hkaAnimationSampleAndCombineJobBase::ANIMATION_SAMPLE_AND_COMBINE_JOB_SAMPLE_ONLY, sizeof(hkaAnimationSampleOnlyJob) )
{
	m_animData = HK_NULL;
	m_numAnims = 0;

	m_cache = HK_NULL;

	m_jobDoneSemaphore = HK_NULL;
	m_jobDoneFlag = 0;
}


hkaQuantizedSampleAndCombineJob::hkaQuantizedSampleAndCombineJob()
	: hkaAnimationSampleAndCombineJobBase( hkaAnimationSampleAndCombineJobBase::ANIMATION_SAMPLE_AND_COMBINE_JOB_QUANTIZED_SAMPLE_AND_COMBINE, sizeof( hkaQuantizedSampleAndCombineJob ) )
{
	m_numAnimations = 0;
	m_animations = HK_NULL;
	m_numBones = 0;
	m_numFloats = 0;
	m_referenceBones = HK_NULL;
	m_referenceFloats = HK_NULL;
	m_parentIndices = HK_NULL;
	m_minimumWeight = 0.0f;
	m_frameSteppingTolerance = 0.0f;
	m_bonesOut = HK_NULL;
	m_floatsOut = HK_NULL;
	m_boneWeightsOut = HK_NULL;
	m_floatWeightsOut = HK_NULL;
	m_mirroredBonePairMap = HK_NULL;
	m_mirroredBoneInvariants = HK_NULL;
	m_jobDoneSemaphore = HK_NULL;
	m_jobDoneFlag = HK_NULL;
}


hkBool hkaAnimationSampleOnlyJob::isValid() const
{
	hkBool valid = true;
	for(int i = 0; i < m_numAnims; i++)
	{
		valid = valid & m_animData[i].isValid();
		if(!valid)	return false;
	}

	return true;
}


hkBool hkaAnimationSampleAndCombineJob::isValid() const
{
	hkBool valid = true;

	// DMA: SC0
	if((m_referencePose) && (m_referencePoseWeightThreshold > 0.0f))
	{
		valid = valid & testValidForDMA("reference pose", m_referencePose,  sizeof(hkQsTransform) * m_maxBone);
		if(!valid)	return false;
	}

	// Parentindices handled differently - alignment not assumed

	// DMA: SC6
	if(m_maxBone != 0)
	{
		valid = valid & testValidForDMA("pose transforms out", m_poseOut,  sizeof(hkQsTransform) * m_maxBone);
		if(!valid)	return false;
	}

	// DMA: SC7
	if(m_maxFloatSlot != 0)
	{
		const int sizeRoundedUp = HK_NEXT_MULTIPLE_OF( 16, sizeof(hkReal) * m_maxFloatSlot);
		valid = valid & testValidForDMA("pose floats out", m_floatSlotsOut,  sizeRoundedUp);
		if(!valid)	return false;
	}

	return true;
}


hkaAnimationSampleOnlyJob::AnimationData::AnimationData()
{
	init();
}

void hkaAnimationSampleOnlyJob::AnimationData::init()
{
	m_frameIndex = 0;
	m_frameDelta = 0.0f; 
	m_maxTransformTrack = 0;
	m_maxFloatTrack = 0;
	m_animationOnCPU = HK_NULL;
	m_animationType = hkaAnimation::HK_UNKNOWN_ANIMATION;
	m_numValidChunks = 0;
	m_poseOut = HK_NULL;
	m_floatSlotsOut = HK_NULL;

	for(int i = 0; i < HK_MAX_NUM_DATA_CHUNKS; i++)
	{
		m_chunks[i].init();
	}
}

hkBool hkaAnimationSampleOnlyJob::AnimationData::isValid() const
{
	hkBool valid = true;

	// Test frame delta
	{
		valid = valid && ((m_frameDelta >= 0.0f) && (m_frameDelta <= 1.0f));
		HK_ASSERT2(0x73a3d963, valid, "m_frameDelta not in range [0.0f, 1.0f]");
		if(!valid) return false;
	}

	// Test chunks
	{
		valid = valid && (m_numValidChunks <= HK_MAX_NUM_DATA_CHUNKS);
		HK_ASSERT2(0x7df92210, valid, "Num valid chunks is > HK_MAX_NUM_DATA_CHUNKS");
		if(!valid) return false;

		for(int i = 0; i < m_numValidChunks; i++)
		{
			const hkaAnimation::DataChunk& chunk = m_chunks[i];

			// Size
			{
				valid = valid && (chunk.m_size > 0);
				HK_ASSERT2(0x2434dfd1, valid, "Chunk " << i << " has invalid size (" << chunk.m_size << ")" );
				if(!valid) return false;
			}

			// Alignment
			{
				valid = valid && (((hkUlong)chunk.m_data & 0xf) == 0);
				HK_ASSERT2(0x5fb9d262, valid, "Chunk " << i << " has unaligned data (" << chunk.m_data << ")" );
				if(!valid) return false;
			}

			// Offset
			{
				valid = valid && (chunk.m_offset < 16);
				HK_ASSERT2(0x4d25db6b, valid, "Chunk " << i << " has offset outside range [0,15] (" << chunk.m_offset<< ")" );
				if(!valid) return false;
			}


		}
	}

	// Test DMA validity

	// DMA: SA1
	if(m_maxTransformTrack != 0)
	{
		valid = valid & testValidForDMA("pose transforms out", m_poseOut,  sizeof(hkQsTransform) * m_maxTransformTrack);
		if(!valid)	return false;
	}

	// DMA: SA2
	if(m_maxFloatTrack != 0)
	{
		const int sizeRoundedUp = HK_NEXT_MULTIPLE_OF( 16, sizeof(hkReal) * m_maxFloatTrack);
		valid = valid & testValidForDMA("pose floats out", m_floatSlotsOut,  sizeRoundedUp);
		if(!valid)	return false;
	}


	return valid;
}


hkaAnimationSampleAndCombineJob::ControlData::ControlData()
{
	init();
}

void hkaAnimationSampleAndCombineJob::ControlData::init()
{
	m_frameIndex = 0;
	m_frameDelta = 0.0f; 
	m_weight = 0.0f;
	m_animationType = hkaAnimation::HK_UNKNOWN_ANIMATION;
	m_numValidChunks = 0;
	m_transformTrackWeights = HK_NULL;
	m_numTransformTrackWeights = 0;
	m_floatTrackWeights = HK_NULL;
	m_numFloatTrackWeights = 0;

	for(int i = 0; i < HK_MAX_NUM_DATA_CHUNKS; i++)
	{
		m_chunks[i].init();
	}
}

hkBool hkaAnimationSampleAndCombineJob::ControlData::isValid() const
{
	hkBool valid = true;

	// Test frame delta
	{
		valid = valid && ((m_frameDelta >= 0.0f) && (m_frameDelta <= 1.0f));
		HK_ASSERT2(0x652a900b, valid, "m_frameDelta not in range [0.0f, 1.0f]");
		if(!valid) return false;
	}

	// Test chunks
	{
		valid = valid && (m_numValidChunks <= HK_MAX_NUM_DATA_CHUNKS);
		HK_ASSERT2(0x23b2c9ed, valid, "Num valid chunks is > HK_MAX_NUM_DATA_CHUNKS");
		if(!valid) return false;

		for(int i = 0; i < m_numValidChunks; i++)
		{
			const hkaAnimation::DataChunk& chunk = m_chunks[i];

			// Size
			{
				valid = valid && (chunk.m_size <= 1024 * 256);
				HK_ASSERT2(0x42385842, valid, "Chunk " << i << " has invalid size (" << chunk.m_size << ")" );
				if(!valid) return false;
			}

			// Alignment
			{
				valid = valid && (((hkUlong)chunk.m_data & 0xf) == 0);
				HK_ASSERT2(0x5f3aec86, valid, "Chunk " << i << " has unaligned data (" << chunk.m_data << ")" );
				if(!valid) return false;
			}

			// Offset
			{
				valid = valid && (chunk.m_offset < 16);
				HK_ASSERT2(0x20731b63, valid, "Chunk " << i << " has offset outside range [0,15] (" << chunk.m_offset<< ")" );
				if(!valid) return false;
			}


		}
	}

	// DMA: SC1
	if(m_binding.m_transformTrackToBoneIndices.getSize() != 0)
	{
		const int bindingSize = HK_NEXT_MULTIPLE_OF(16, m_binding.m_transformTrackToBoneIndices.getSize() * sizeof(hkInt16) );
		valid = valid & testValidForDMA("transform track binding indices", m_binding.m_transformTrackToBoneIndices.begin(),  bindingSize);	
		if(!valid)	return false;
	}

	// DMA: SC2
	if(m_binding.m_floatTrackToFloatSlotIndices.getSize() != 0)
	{
		const int bindingSize = HK_NEXT_MULTIPLE_OF(16, m_binding.m_floatTrackToFloatSlotIndices.getSize() * sizeof(hkInt16) );
		valid = valid & testValidForDMA("float track binding indices", m_binding.m_floatTrackToFloatSlotIndices.begin(),  bindingSize);
		if(!valid)	return false;
	}

	// DMA: SC3
	if(m_numTransformTrackWeights != 0)
	{
		const int trackWeightSize = HK_NEXT_MULTIPLE_OF(16, m_numTransformTrackWeights * sizeof(hkUint8) );
		valid = valid & testValidForDMA("transform track weights", m_transformTrackWeights,  trackWeightSize);
		if(!valid)	return false;
	}

	// DMA: SC4
	if(m_numFloatTrackWeights != 0)
	{
		const int trackWeightSize = HK_NEXT_MULTIPLE_OF(16, m_numFloatTrackWeights * sizeof(hkUint8) );
		valid = valid & testValidForDMA("float track weights", m_floatTrackWeights,  trackWeightSize);	
		if(!valid)	return false;
	}

	return valid;
}

void hkaQuantizedSampleAndCombineJob::QuantizedAnimationEntry::setAnimData( const hkaQuantizedAnimation* anim )
{
	m_animData = anim->getData();
	m_numAnimData = *reinterpret_cast< const hkUint16* >( m_animData );
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
