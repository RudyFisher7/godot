//
// Created by rfish on 3/6/2024.
//

#ifndef SERVERS_VOXEL_NAVIGATION_SERVER_DUMMY_H
#define SERVERS_VOXEL_NAVIGATION_SERVER_DUMMY_H


#include "voxel_navigation_server.h"

#include "core/object/object.h"
#include "core/object/class_db.h"


class VoxelNavigationServerDummy : public Object {
	GDCLASS(VoxelNavigationServerDummy, Object);
	friend class VoxelNavigationServer;
	static VoxelNavigationServerDummy *singleton;

protected:
	static void _bind_methods();
public:
	static VoxelNavigationServerDummy *get_singleton();
	VoxelNavigationServerDummy();
	~VoxelNavigationServerDummy() override = default;
	Error init();
	void finish();
	void tmp(int val);
};

#endif //SERVERS_VOXEL_NAVIGATION_SERVER_DUMMY_H
