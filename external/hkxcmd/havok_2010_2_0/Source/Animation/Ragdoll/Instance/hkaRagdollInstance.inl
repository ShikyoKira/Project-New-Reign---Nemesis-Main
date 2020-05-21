/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



int hkaRagdollInstance::getNumBones () const
{
	return m_skeleton->m_bones.getSize();
}

hkpRigidBody* hkaRagdollInstance::getRigidBodyOfBone (int i) const
{
	const int rbIdx = m_boneToRigidBodyMap[i];
	return (rbIdx<0) ? HK_NULL : m_rigidBodies[rbIdx];
}

int hkaRagdollInstance::getBoneIndexOfRigidBody (hkpRigidBody* rb) const
{
	int rbIndex = m_rigidBodies.indexOf( rb );
	if(rbIndex == -1)
	{
		return -1;
	}
	return m_boneToRigidBodyMap.indexOf( rbIndex );
}

hkpConstraintInstance* hkaRagdollInstance::getConstraintOfBone (int i) const
{
	const int ctIdx = m_boneToRigidBodyMap[i]-1;
	return (ctIdx<0) ? HK_NULL : m_constraints[ctIdx];
}

int hkaRagdollInstance::getParentOfBone (int i) const
{
	return (m_skeleton->m_parentIndices[i]);
}

const hkaSkeleton* hkaRagdollInstance::getSkeleton() const
{
	return m_skeleton;
}

const hkArray<hkpRigidBody*>& hkaRagdollInstance::getRigidBodyArray() const
{
	return m_rigidBodies;
}

const hkArray<hkpConstraintInstance*>& hkaRagdollInstance::getConstraintArray() const
{
	return m_constraints;
}

hkBool hkaRagdollInstance::hasNonRigidBodyBones () const
{
	return m_rigidBodies.getSize() != m_skeleton->m_bones.getSize();
}


hkpWorld* hkaRagdollInstance::getWorld() const
{
	hkpRigidBody* root = m_rigidBodies[0];

	if (root)
	{
		return root->getWorld();
	}
	else
	{
		return HK_NULL;
	}
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
