/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


HK_FORCE_INLINE hkReal hkpSampledHeightFieldShape::getHeightAt( int x, int z ) const
{
	return getHeightAtImpl(x, z);
}

HK_FORCE_INLINE hkBool hkpSampledHeightFieldShape::getTriangleFlip() const
{
	return getTriangleFlipImpl();
}

template<class IMPL>
HK_FORCE_INLINE void HK_CALL hkSampledHeightFieldShape_collideSpheres(
	const IMPL& impl,
	const hkpHeightFieldShape::CollideSpheresInput& input,
	hkpHeightFieldShape::SphereCollisionOutput* outputArray )
{
	const hkSphere* spheres = input.m_spheres;
	hkpHeightFieldShape::SphereCollisionOutput* output = outputArray;

	hkVector4 defNormal;
	defNormal.set( 0,1,0,HK_REAL_MAX);

	for (int i = input.m_numSpheres -1; i>=0; output++, spheres++, i--)
	{
		output[0] = defNormal;

		hkVector4 pos; 	pos.setMul4( spheres->getPosition(), impl.m_floatToIntScale );

		hkIntUnion64 out;
		hkVector4Util::convertToUint16( spheres->getPosition(), impl.m_floatToIntOffsetFloorCorrected, impl.m_floatToIntScale, out );
		hkInt32 x = out.u16[0];
		hkInt32 z = out.u16[2];


		//
		//	Check for boundaries
		//
		if ( (x >= (impl.m_xRes-1) ) || (z >= (impl.m_zRes-1) ) )
		{
			continue;
		}

		hkReal subX = pos(0) - hkReal(x);
		hkReal subZ = pos(2) - hkReal(z);

		//HK_ASSERT(0x668a555b,  subX >= 0.0f && subX <= 1.0f );
		//HK_ASSERT(0x20de02d8,  subZ >= 0.0f && subZ <= 1.0f );

		hkReal height;
		hkReal vertHeight;
		int 	vertX;
 		if ( impl.IMPL::getTriangleFlip() )
		{
			hkReal h00 = impl.IMPL::getHeightAt( x+0, z   );
			vertHeight = h00;
			vertX = x;
			hkReal h11 = impl.IMPL::getHeightAt( x+1, z+1 );
			if ( subX > subZ )
			{
				hkReal h10 = impl.IMPL::getHeightAt( x+1, z   );

				hkReal dx = h10 - h00;
				hkReal dz = h11 - h10;

				height = h00  + subZ * dz + subX * dx;

				output[0](0) = -dx;
				output[0](2) = -dz;
			}
			else
			{
				hkReal h01 = impl.IMPL::getHeightAt( x, z+1 );

				hkReal dx = h11 - h01;
				hkReal dz = h01 - h00;

				height = h00  + subZ * dz + subX * dx;

				output[0](0) = -dx;
				output[0](2) = -dz;
			}
		}
		else
		{
			hkReal h10 = impl.IMPL::getHeightAt( x+1, z  );
			vertHeight = h10;
			vertX = x+1;
			hkReal h01 = impl.IMPL::getHeightAt( x,   z+1 );
			if ( subX + subZ > 1.0f )
			{
				hkReal h11 = impl.IMPL::getHeightAt( x+1, z+1 );

				hkReal dx = h11 - h01;
				hkReal dz = h11 - h10;

				height = h10  + subZ * dz + (subX - 1.0f) * dx;

				output[0](0) = -dx;
				output[0](2) = -dz;
			}
			else
			{
				hkReal h00 = impl.IMPL::getHeightAt( x+0, z   );

				hkReal dx = h10 - h00;
				hkReal dz = h01 - h00;

				height = h00  + subZ * dz + subX * dx;

				output[0](0) = -dx;
				output[0](2) = -dz;
			}
		}

		output->mul4( impl.m_floatToIntScale ); // reverse multiply to correct for optimized cross product
		output->normalize3();

		hkReal absoluteDepth;
		if (!impl.m_useProjectionBasedHeight)
		{
			absoluteDepth = (pos(1) - height) * impl.m_intToFloatScale(1) - spheres->getRadius();
		}
		else
		{	
			hkVector4 triangleVert;	triangleVert.set((hkReal)vertX, vertHeight,(hkReal)z);
	
			hkVector4 vectorToTri;
			vectorToTri.setSub4(pos, triangleVert);
			vectorToTri.mul4(impl.m_intToFloatScale);

			hkReal distanceToTri = output->dot3(vectorToTri).getReal();
			absoluteDepth = distanceToTri - spheres->getRadius();
		}
		output[0](3) = absoluteDepth;
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
