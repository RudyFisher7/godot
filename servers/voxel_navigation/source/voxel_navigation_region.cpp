//
// Created by rfish on 3/8/2024.
//

#include "../include/voxel_navigation/voxel_navigation_region.h"

#include "core/math/aabb.h"
#include "scene/3d/mesh_instance_3d.h"

namespace VoxelNavigation {

void VoxelNavigationRegion::_bind_methods() {
	ClassDB::bind_method(D_METHOD("bake_grid"), &VoxelNavigationRegion::bake_grid);
}


void VoxelNavigationRegion::bake_grid() {
	ERR_FAIL_COND_MSG(!Thread::is_main_thread(), "The SceneTree can only be parsed on the main thread. Call this function from the main thread or use call_deferred().");
	ERR_FAIL_COND(!is_inside_tree());

	if (get_child_count() > 0) {
		MeshInstance3D *mesh_instance = Object::cast_to<MeshInstance3D>(get_child(0));
		if (mesh_instance) {
			Ref<Mesh> mesh = mesh_instance->get_mesh();
			if (mesh.is_valid()) {
				const Transform3D xform = mesh_instance->get_transform();
				source_geometry_data.add_mesh(mesh, xform);
			}
		}
	}

	//rudy todo:: pickup here

	// this Node3D's global position is considered 0,0,0 for now
	// apply global position during runtime queries
	AABB aabb = AABB();
}


void VoxelNavigationRegion::init(Vector3i in_dimensions) {
	_resolution = in_dimensions.clamp(Vector3i(0, 0, 0), Vector3i(4, 4, 4));
	_voxels.clear();
	_voxels.resize(_resolution.x * _resolution.y * _resolution.z);
}

} //namespace VoxelNavigation