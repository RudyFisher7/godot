//
// Created by rfish on 3/15/2024.
//

#ifndef VOXEL_LINK_H
#define VOXEL_LINK_H


#include <stdint.h>


namespace VoxelNavigation {

class VoxelLink {
private:
	uint32_t _raw_data = 0u;

public:
	VoxelLink() = default;
	~VoxelLink() = default;

	//rudy todo:: copy and move stuff


	int get_layer_index() const;
	int get_node_index() const;
	int get_subnode_index() const;
};

} // namespace VoxelNavigation

#endif // VOXEL_LINK_H
