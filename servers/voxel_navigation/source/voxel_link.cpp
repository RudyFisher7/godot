//
// Created by rfish on 3/15/2024.
//

#include "../include/voxel_navigation/voxel_link.h"

namespace VoxelNavigation {

int VoxelLink::get_layer_index() const {
	return static_cast<int>(_raw_data & 0xf);
}


int VoxelLink::get_node_index() const {
	return static_cast<int>(_raw_data & 0x3fffff0);
}


int VoxelLink::get_subnode_index() const {
	return static_cast<int>(_raw_data & 0xfc000000);
}

} //namespace VoxelNavigation