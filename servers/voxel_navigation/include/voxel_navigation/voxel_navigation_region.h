//
// Created by rfish on 3/8/2024.
//

#ifndef VOXEL_NAVIGATION_REGION_H
#define VOXEL_NAVIGATION_REGION_H


#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/math/vector3.h"
#include "core/math/vector3i.h"
#include "core/templates/local_vector.h"
#include "scene/3d/node_3d.h"
#include "scene/resources/navigation_mesh_source_geometry_data_3d.h"


namespace VoxelNavigation {

class VoxelNavigationRegion : public Node3D {
	GDCLASS(VoxelNavigationRegion, Node3D);


protected:
	NavigationMeshSourceGeometryData3D source_geometry_data;
	Vector3i _resolution;
	LocalVector<uint64_t> _voxels;//rudy todo:: may want to use Cow Vector for passing around or store in server

	static void _bind_methods();


public:
	VoxelNavigationRegion() = default;
	~VoxelNavigationRegion() = default;

	//fixme:: implement inside server and its helpers instead
	void bake_grid();

	void init(Vector3i in_dimensions);
};

} // namespace VoxelNavigation

#endif // VOXEL_NAVIGATION_REGION_H
