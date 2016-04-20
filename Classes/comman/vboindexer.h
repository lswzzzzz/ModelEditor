#pragma once
#include "cocos2d.h"
USING_NS_CC;

struct PackedVertex{
	Vec3 position;
	Vec2 uv;
	Vec3 normal;
	bool operator<(const PackedVertex that) const{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex( 
	Vec3 & in_vertex, 
	Vec2 & in_uv, 
	Vec3 & in_normal, 
	std::vector<Vec3> & out_vertices,
	std::vector<Vec2> & out_uvs,
	std::vector<Vec3> & out_normals,
	unsigned short & result);

bool getSimilarVertexIndex_fast( 
	PackedVertex & packed, 
	std::map<PackedVertex,unsigned short> & VertexToOutIndex,
	unsigned short & result);

void indexVBO_slow(
	std::vector<Vec3> & in_vertices,
	std::vector<Vec2> & in_uvs,
	std::vector<Vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<Vec3> & out_vertices,
	std::vector<Vec2> & out_uvs,
	std::vector<Vec3> & out_normals
);

void indexVBO(
	std::vector<Vec3> & in_vertices,
	std::vector<Vec2> & in_uvs,
	std::vector<Vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<Vec3> & out_vertices,
	std::vector<Vec2> & out_uvs,
	std::vector<Vec3> & out_normals);

void indexVBO_TBN(
	std::vector<Vec3> & in_vertices,
	std::vector<Vec2> & in_uvs,
	std::vector<Vec3> & in_normals,
	std::vector<Vec3> & in_tangents,
	std::vector<Vec3> & in_bitangents,

	std::vector<unsigned short> & out_indices,
	std::vector<Vec3> & out_vertices,
	std::vector<Vec2> & out_uvs,
	std::vector<Vec3> & out_normals,
	std::vector<Vec3> & out_tangents,
	std::vector<Vec3> & out_bitangents);