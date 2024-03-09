//
// Created by rfish on 3/6/2024.
//


#include "register_types.h"

#include "core/object/class_db.h"
#include "core/config/engine.h"

#include "include/voxel_navigation/voxel_grid.h"


void register_voxel_navigation_types() {
	ClassDB::register_class<VoxelNavigation::VoxelGrid>();
}

void unregister_voxel_navigation_types() {

}
