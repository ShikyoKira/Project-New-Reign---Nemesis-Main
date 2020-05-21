/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
TESSELATOR_HDR
inline										TESSELATOR_TYP::hkgpVolumeTesselator()
{
	clear();
}

//
TESSELATOR_HDR
inline void									TESSELATOR_TYP::clear()
{
	m_edgesTable.setBucketsCount(EDGE_TABLE_SIZE);
	m_edgesTable.reserve(32);
	m_verticesPool.clear();
	m_tetrahedraPool.clear();
	m_tetrahedra.clear();
	m_tetrahedra.reserve(1024);
	m_metric=HK_NULL;
	m_vertexUid=0;
}

//
TESSELATOR_HDR
inline void									TESSELATOR_TYP::initialize(METRIC* metric, const hkVector4* vertices,const int* tetrahedra,const typename METRIC::TETRAHEDRON_DATA* data, int numTetrahedra)
{
	clear();
	m_metric = metric;
	hkPointerMap<int,Vertex*,ALLOCATOR>	newVertices; newVertices.reserve(numTetrahedra*4);
	for(int tetrahedronIndex = 0; tetrahedronIndex<numTetrahedra; ++tetrahedronIndex)
	{
		const int*	vertexIndices = tetrahedra+tetrahedronIndex*4;
		Vertex*		usedVertices[4];
		for(int i=0;i<4;++i)
		{
			Vertex*	newVertex = newVertices.getWithDefault(vertexIndices[i],HK_NULL);
			if(!newVertex)
			{
				newVertex = createVertex(vertices[vertexIndices[i]]);
				newVertices.insert(vertexIndices[i],newVertex);
			}
			usedVertices[i] = newVertex;
		}
		Tetrahedron*	newTetrahedron =createTetrahedron(usedVertices[0],usedVertices[1],usedVertices[2],usedVertices[3],HK_NULL);
		if(data)
		{
			newTetrahedron->m_data = data[tetrahedronIndex];
		}
	}
}

//
TESSELATOR_HDR
inline void									TESSELATOR_TYP::initialize(METRIC* metric, const hkAabb& aabb)
{
	hkVector4	vertices[8];
	for(int i=0;i<8;++i)
	{
		hkAabbUtil::getVertex(aabb,i,vertices[i]);
	}
	const int	indices[][4]={{0,5,6,3},{0,1,5,3},{3,0,2,6},{0,5,4,6},{6,5,7,3}};
	initialize(metric,vertices,&indices[0][0],HK_NULL,5);
}

//
TESSELATOR_HDR
inline hkBool32								TESSELATOR_TYP::refine()
{	
	/* Prepare edges		*/ 
	for(int i=0;i<m_edgesTable.m_buckets.getSize();++i)
	{
		for(int j=0;j<m_edgesTable.m_buckets[i].getSize();++j)
		{
			Edge&		e	=	m_edgesTable.m_buckets[i][j];
			e.m_fraction	=	m_metric->prepare(&e);
			e.m_split		=	0;
			HK_ASSERT2(0x8C2900FF, 0 < e.m_fraction && e.m_fraction < 1, "Invalid fraction range");
		}
	}

	/* Evaluate metric		*/ 
	int			numTetrahedra = m_tetrahedra.getSize();
	for(int i=0;i<numTetrahedra;++i)
	{
		Tetrahedron*	t = m_tetrahedra[i];
		int				c = t->m_needUpdate? m_metric->refine(t) : 0;
		t->m_needUpdate = 0;
		if(c == 0x40)
		{
			hkSimdReal	maxLength=-HK_REAL_MAX;
			int			maxEdge=-1;
			for(int j=0;j<6;++j)
			{
				hkSimdReal	length = t->m_vertices[g_tetraEdges[j].m_vertices[0]]->m_position.distanceToSquared3(t->m_vertices[g_tetraEdges[j].m_vertices[1]]->m_position);
				if(length > maxLength)
				{
					maxEdge		=	j;
					maxLength	=	length;
				}
			}
			c = 1<<maxEdge;
		}
		if(c)
		{
			if(c == 0x80)
			{
				releaseTetrahedron(t);
				m_tetrahedra.removeAt(i--);
				--numTetrahedra;
				
			}
			else
			{
				for(int j=0;j<6;++j)
				{
					if(c&(1<<j))
					{
						const TetraEdge& e(g_tetraEdges[j]);
						m_edgesTable.find(Edge(t->m_vertices[e.m_vertices[0]],t->m_vertices[e.m_vertices[1]]))->m_split=1;
					}
				}
			}
		}		
	}

	/* Refine				*/ 
	hkBool32	refined = 0;
	for(int i=0;i<numTetrahedra;++i)
	{
		Tetrahedron*	t = m_tetrahedra[i];
		int				c = 0;
		Edge*			e[6];
		for(int j=0;j<6;++j)
		{
			const TetraEdge& te(g_tetraEdges[j]);
			e[j] = m_edgesTable.find(Edge(t->m_vertices[te.m_vertices[0]],t->m_vertices[te.m_vertices[1]]));
			if(e[j] && e[j]->m_split) c|=1<<j;
		}
		if(c)
		{
			refined=1;
			splitTetrahedronEdges(t,e,c);
			releaseTetrahedron(t);
			m_tetrahedra.removeAt(i);
		}
	}
	return refined;
}

//
TESSELATOR_HDR
inline typename TESSELATOR_TYP::Edge*		TESSELATOR_TYP::getEdge(const Vertex* a,const Vertex* b)
{	
	Edge*	edge = m_edgesTable.find(Edge(a,b));
	if(!edge)
	{
		edge				=	m_edgesTable.insert(Edge(a,b));
		edge->m_split		=	0;
		edge->m_fraction	=	0.5f;
		edge->m_refs		=	0;
		edge->m_midPoint	=	HK_NULL;
		m_metric->onCreate(edge);
	}	
	return edge;
}

//
TESSELATOR_HDR
inline typename TESSELATOR_TYP::Vertex*		TESSELATOR_TYP::createVertex(const hkVector4& position)
{
	Vertex*	vertex		=	m_verticesPool.allocate();
	vertex->m_position	=	position;
	vertex->m_uid		=	++m_vertexUid;
	vertex->m_refs		=	0;
	m_metric->onCreate(vertex);
	return vertex;
}

//
TESSELATOR_HDR
inline void									TESSELATOR_TYP::releaseTetrahedron(Tetrahedron* t)
{
	for(int i=0;i<6;++i)
	{
		const TetraEdge&	te(g_tetraEdges[i]);
		Edge*				e(m_edgesTable.find(Edge(t->m_vertices[te.m_vertices[0]],t->m_vertices[te.m_vertices[1]])));
		if(!(--e->m_refs))	m_edgesTable.remove(*e);
	}
	for(int i=0;i<4;++i)
	{
		if(!(--t->m_vertices[i]->m_refs))	m_verticesPool.release(t->m_vertices[i]);
	}
	m_tetrahedraPool.release(t);
}

//
TESSELATOR_HDR
inline typename TESSELATOR_TYP::Tetrahedron*	TESSELATOR_TYP::createTetrahedron(Vertex* a,Vertex* b,Vertex* c,Vertex* d,const Tetrahedron* parent)
{
	Tetrahedron* tetrahedron = m_tetrahedraPool.allocate();
	tetrahedron->m_needUpdate	=	1;
	tetrahedron->m_vertices[0]	=	a; ++a->m_refs;
	tetrahedron->m_vertices[1]	=	b; ++b->m_refs;
	tetrahedron->m_vertices[2]	=	c; ++c->m_refs;
	tetrahedron->m_vertices[3]	=	d; ++d->m_refs;
	for(int i=0;i<6;++i)
	{
		const TetraEdge&	te(g_tetraEdges[i]);
		++getEdge(tetrahedron->m_vertices[te.m_vertices[0]],tetrahedron->m_vertices[te.m_vertices[1]])->m_refs;
	}
	if(parent)
	{
		tetrahedron->m_depth	=	parent->m_depth+1;
		tetrahedron->m_data		=	parent->m_data;
	}
	else
	{
		tetrahedron->m_depth	=	0;
	}
	m_tetrahedra.pushBack(tetrahedron);
	m_metric->onCreate(tetrahedron);
	return tetrahedron;
}

//
TESSELATOR_HDR
inline void									TESSELATOR_TYP::splitTetrahedronEdges(const Tetrahedron* tetrahedron,Edge** edges,int edgesToSplit)
{
	Vertex*	vertices[11]={tetrahedron->m_vertices[0],tetrahedron->m_vertices[1],tetrahedron->m_vertices[2],tetrahedron->m_vertices[3]};
	
	// Compute face diagonals orientation.
	edgesToSplit&=63;
	for(int i=0;i<4;++i)
	{
		const TetraFace&	face(g_tetraFaces[i]);
		const int			minUid(vertices[face.m_vertices[0]]->m_uid<vertices[face.m_vertices[1]]->m_uid		?
								(vertices[face.m_vertices[0]]->m_uid<vertices[face.m_vertices[2]]->m_uid?0:2)	:
								(vertices[face.m_vertices[1]]->m_uid<vertices[face.m_vertices[2]]->m_uid?1:2));
		edgesToSplit |= (vertices[minUid]->m_uid&1)<<(6+i);
	}

	// Compute edges midpoint.
	for(int i=0;i<6;++i)
	{		
		if(edges[i]->m_split && !edges[i]->m_midPoint)
		{
			hkVector4	midPoint; midPoint.setInterpolate4(edges[i]->m_vertices[0]->m_position,edges[i]->m_vertices[1]->m_position,edges[i]->m_fraction);
			edges[i]->m_midPoint = createVertex(midPoint);
		}
		vertices[4+i] = edges[i]->m_midPoint;
	}

	// Lookup split infos.
	const TetraSplitInfos&	split(g_tetraSplitInfosTable[edgesToSplit]);

	// Compute steiner vertex if required.
	vertices[10]=HK_NULL;
	if(split.m_steinerPoint)
	{
		int			numSamples=0;
		hkVector4	steiner; steiner.setZero4();
		for(int i=0;i<10;++i)
		{
			if(split.m_steinerPoint&(1<<i))
			{
				++numSamples;
				steiner.add4(vertices[i]->m_position);
			}
		}
		steiner.mul4(1.0f/(hkReal)numSamples);
		vertices[10]=createVertex(steiner);
	}

	// Evaluate splits
	for(int i=0;i<split.m_numTetra;++i)
	{
		createTetrahedron(	vertices[split.m_tetra[i].m_a],
							vertices[split.m_tetra[i].m_b],
							vertices[split.m_tetra[i].m_c],
							vertices[split.m_tetra[i].m_d],
							tetrahedron);
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
