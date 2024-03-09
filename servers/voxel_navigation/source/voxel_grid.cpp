//
// Created by rfish on 3/8/2024.
//

#include "../include/voxel_navigation/voxel_grid.h"

namespace VoxelNavigation {

void VoxelGrid::_bind_methods() {
	ClassDB::bind_method(D_METHOD("tmp", "val"), &VoxelGrid::tmp);
}


void VoxelGrid::tmp(int val) {
}


void VoxelGrid::init(Vector3i in_dimensions) {
	_resolution = in_dimensions.clamp(Vector3i(0, 0, 0), Vector3i(4, 4, 4));
	_voxels.clear();
	_voxels.resize(_resolution.x * _resolution.y * _resolution.z);
}

} //namespace VoxelNavigation