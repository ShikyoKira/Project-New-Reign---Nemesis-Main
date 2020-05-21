/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
TESSELATOR_HDR
inline										TESSELATOR_TYP::hkgpSurfaceTesselator()
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
	m_trianglesPool.clear();
	m_triangles.clear();
	m_metric=HK_NULL;
	m_vertexUid=0;
}

//
TESSELATOR_HDR
inline void									TESSELATOR_TYP::initialize(METRIC* metric, const hkVector4* vertices,const int* triangles,const typename METRIC::TRIANGLE_DATA* data, int numTriangles)
{
	clear();
	m_metric = metric;
	hkArray<Vertex*,ALLOCATOR>	newVertices; newVertices.setSize(numTriangles*3,HK_NULL);
	for(int triangleIndex = 0; triangleIndex<numTriangles; ++triangleIndex)
	{
		const int*	vertexIndices = triangles+triangleIndex*3;
		Vertex*		usedVertices[3];
		for(int i=0;i<3;++i)
		{
			if(!newVertices[vertexIndices[i]])
			{
				newVertices[vertexIndices[i]]	=	createVertex(vertices[vertexIndices[i]]);
			}
			usedVertices[i] = newVertices[vertexIndices[i]];
		}
		Triangle*	newTriangle =createTriangle(usedVertices[0],usedVertices[1],usedVertices[2],HK_NULL);
		if(data)
		{
			newTriangle->m_data = data[triangleIndex];
		}
	}
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
			HK_ASSERT2(0x7B12CD8F, 0 < e.m_fraction && e.m_fraction < 1, "Invalid fraction range");
		}
	}

	/* Evaluate metric		*/ 
	int			numTriangles = m_triangles.getSize();	
	for(int i=0;i<numTriangles;++i)
	{
		Triangle*	t = m_triangles[i];
		int			c = t->m_needUpdate? m_metric->refine(t) : 0;
		t->m_needUpdate = 0;
		if(c == 0x08)
		{
			hkVector4	edgeLengths;
			edgeLengths.set(t->m_vertices[0]->m_position.distanceToSquared3(t->m_vertices[1]->m_position),
							t->m_vertices[1]->m_position.distanceToSquared3(t->m_vertices[2]->m_position),
							t->m_vertices[2]->m_position.distanceToSquared3(t->m_vertices[0]->m_position));
			c = 1<<edgeLengths.getMajorAxis3();
		}
		if(c)
		{
			if(c == 0x10)
			{
				releaseTriangle(t);
				m_triangles.removeAt(i--);
				--numTriangles;
				
			}
			else
			{
				for(int j=2,k=0;k<3;j=k++)
				{
					if(c&(1<<j))
					{
						m_edgesTable.find(Edge(t->m_vertices[j],t->m_vertices[k]))->m_split=1;
					}
				}
			}
		}		
	}

	/* Refine				*/ 
	hkBool32	refined = 0;
	for(int i=0;i<numTriangles;++i)
	{
		Triangle*	t = m_triangles[i];
		int			c = 0;
		Edge*		e[3];
		for(int j=2,k=0;k<3;j=k++)
		{
			e[j] = m_edgesTable.find(Edge(t->m_vertices[j],t->m_vertices[k]));
			if(e[j] && e[j]->m_split) c|=1<<j;
		}
		if(c)
		{
			refined=1;
			splitTriangleEdges(t,e,c);
			releaseTriangle(t);
			m_triangles.removeAt(i);
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
inline void									TESSELATOR_TYP::releaseTriangle(Triangle* t)
{
	for(int i=2,j=0;j<3;i=j++)
	{
		Edge*	e(m_edgesTable.find(Edge(t->m_vertices[i],t->m_vertices[j])));
		if(!(--e->m_refs))					m_edgesTable.remove(*e);
		if(!(--t->m_vertices[i]->m_refs))	m_verticesPool.release(t->m_vertices[i]);
	}
	m_trianglesPool.release(t);
}

//
TESSELATOR_HDR
inline typename TESSELATOR_TYP::Triangle*	TESSELATOR_TYP::createTriangle(Vertex* a,Vertex* b,Vertex* c,const Triangle* parent)
{
	Triangle* triangle = m_trianglesPool.allocate();
	triangle->m_needUpdate	=	1;
	triangle->m_vertices[0]	=	a; ++a->m_refs;
	triangle->m_vertices[1]	=	b; ++b->m_refs;
	triangle->m_vertices[2]	=	c; ++c->m_refs;
	++getEdge(a,b)->m_refs;
	++getEdge(b,c)->m_refs;
	++getEdge(c,a)->m_refs;
	if(parent)
	{
		triangle->m_depth	=	parent->m_depth+1;
		triangle->m_data	=	parent->m_data;
	}
	else
	{
		triangle->m_depth	=	0;
	}
	m_triangles.pushBack(triangle);
	m_metric->onCreate(triangle);
	return triangle;
}

//
TESSELATOR_HDR
inline int									TESSELATOR_TYP::createQuad(Vertex* m0, Vertex* m1, Vertex* v0, Vertex* v1, const Triangle* parent)
{
	if(v0->m_uid < v1->m_uid)
	{
		createTriangle(m0,m1,v0,parent);
		createTriangle(m0,v0,v1,parent);
		return 0;
	}
	else
	{
		createTriangle(m0,m1,v1,parent);
		createTriangle(m1,v0,v1,parent);
		return 1;
	}
}

//
TESSELATOR_HDR
inline void									TESSELATOR_TYP::splitTriangleEdges(const Triangle* triangle,Edge** edges,int edgesToSplit)
{
	Vertex*	midPoints[3];
	for(int i=0;i<3;++i)
	{
		if(edges[i] && edges[i]->m_midPoint==HK_NULL && (edgesToSplit&(1<<i)))
		{
			hkVector4	midPoint;	midPoint.setInterpolate4(edges[i]->m_vertices[0]->m_position,edges[i]->m_vertices[1]->m_position,edges[i]->m_fraction);
			edges[i]->m_midPoint	=	createVertex(midPoint);
		}
		midPoints[i] = edges[i]->m_midPoint;
	}
	switch(edgesToSplit&7)
	{
	case	1:	createTriangle(midPoints[0],triangle->m_vertices[1],triangle->m_vertices[2],triangle);
				createTriangle(midPoints[0],triangle->m_vertices[2],triangle->m_vertices[0],triangle);
		break;

	case	2:	createTriangle(midPoints[1],triangle->m_vertices[2],triangle->m_vertices[0],triangle);
				createTriangle(midPoints[1],triangle->m_vertices[0],triangle->m_vertices[1],triangle);
		break;

	case	3:	createTriangle(midPoints[1],midPoints[0],triangle->m_vertices[1],triangle);
				createQuad(midPoints[0],midPoints[1],triangle->m_vertices[2],triangle->m_vertices[0],triangle);
		break;

	case	4:	createTriangle(midPoints[2],triangle->m_vertices[0],triangle->m_vertices[1],triangle);
				createTriangle(midPoints[2],triangle->m_vertices[1],triangle->m_vertices[2],triangle);
		break;

	case	5:	createTriangle(midPoints[0],midPoints[2],triangle->m_vertices[0],triangle);
				createQuad(midPoints[2],midPoints[0],triangle->m_vertices[1],triangle->m_vertices[2],triangle);
		break;

	case	6:	createTriangle(midPoints[2],midPoints[1],triangle->m_vertices[2],triangle);
				createQuad(midPoints[1],midPoints[2],triangle->m_vertices[0],triangle->m_vertices[1],triangle);
		break;

	case	7:	createTriangle(triangle->m_vertices[0],midPoints[0],midPoints[2],triangle);
				createTriangle(triangle->m_vertices[1],midPoints[1],midPoints[0],triangle);
				createTriangle(triangle->m_vertices[2],midPoints[2],midPoints[1],triangle);
				createTriangle(midPoints[0],midPoints[1],midPoints[2],triangle);
		break;
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
