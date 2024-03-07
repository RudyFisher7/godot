//
// Created by rfish on 3/6/2024.
//


#include "register_types.h"

#include "core/object/class_db.h"
#include "core/config/engine.h"

#include "servers/voxel_navigation_server.h"
#include "servers/voxel_navigation_server_dummy.h"

static VoxelNavigationServer *voxel_navigation_server = nullptr;
static VoxelNavigationServerDummy *_voxel_navigation_server = nullptr;

void register_voxel_navigation_server_types() {
	voxel_navigation_server = memnew(VoxelNavigationServer);
	voxel_navigation_server->init();
	_voxel_navigation_server = memnew(VoxelNavigationServerDummy);
	ClassDB::register_class<VoxelNavigationServerDummy>();
	Engine::get_singleton()->add_singleton(Engine::Singleton("VoxelNavigationServer", VoxelNavigationServerDummy::get_singleton()));
}

void unregister_voxel_navigation_server_types() {
	if (voxel_navigation_server) {
		voxel_navigation_server->finish();
		memdelete(voxel_navigation_server);
	}

	if (_voxel_navigation_server) {
		memdelete(_voxel_navigation_server);
	}
}
