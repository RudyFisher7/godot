//
// Created by rfish on 3/8/2024.
//

#ifndef VOXEL_NAVIGATION_VOXEL_GRID_H
#define VOXEL_NAVIGATION_VOXEL_GRID_H


#include "scene/main/node.h"
#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/math/vector3.h"
#include "core/math/vector3i.h"
#include "core/templates/local_vector.h"


namespace VoxelNavigation {

class VoxelGrid : public Node {
	GDCLASS(VoxelGrid, Node);


protected:
	Vector3i _resolution;
	LocalVector<uint64_t> _voxels;

	static void _bind_methods();


public:
	VoxelGrid() = default;
	~VoxelGrid() = default;

	void tmp(int val);

	void init(Vector3i in_dimensions);
};

} //namespace VoxelNavigation

#endif //VOXEL_NAVIGATION_VOXEL_GRID_H
