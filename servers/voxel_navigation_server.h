//
// Created by rfish on 3/6/2024.
//

#ifndef SERVERS_VOXEL_NAVIGATION_SERVER_H
#define SERVERS_VOXEL_NAVIGATION_SERVER_H


#include "core/object/object.h"
#include "core/object/class_db.h"

class VoxelNavigationServer : public Object {
	GDCLASS(VoxelNavigationServer, Object);

	static VoxelNavigationServer *singleton;

public:
	VoxelNavigationServer();
	~VoxelNavigationServer() override;
	virtual Error init();

	static VoxelNavigationServer *get_singleton();

	virtual void finish();

	virtual void tmp(int val);

protected:
	static void _bind_methods();
};


#endif //SERVERS_VOXEL_NAVIGATION_SERVER_H
