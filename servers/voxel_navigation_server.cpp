//
// Created by rfish on 3/6/2024.
//

#include "servers/voxel_navigation_server.h"


VoxelNavigationServer *VoxelNavigationServer::singleton = nullptr;


Error VoxelNavigationServer::init() {
	return Error::OK;
}

VoxelNavigationServer *VoxelNavigationServer::get_singleton() {
	return singleton;
}


VoxelNavigationServer::VoxelNavigationServer() {
	singleton = this;
}


VoxelNavigationServer::~VoxelNavigationServer() {
}


void VoxelNavigationServer::finish() {
}


void VoxelNavigationServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("tmp", "val"), &VoxelNavigationServer::tmp);
}
void VoxelNavigationServer::tmp(int val) {
}
