//
// Created by rfish on 3/6/2024.
//


#include "voxel_navigation_server_dummy.h"


VoxelNavigationServerDummy *VoxelNavigationServerDummy::singleton = nullptr;


VoxelNavigationServerDummy *VoxelNavigationServerDummy::get_singleton() {
	return singleton;
}


void VoxelNavigationServerDummy::_bind_methods() {
	ClassDB::bind_method(D_METHOD("tmp", "val"), &VoxelNavigationServerDummy::tmp);
}


VoxelNavigationServerDummy::VoxelNavigationServerDummy() {
	singleton = this;
}


Error VoxelNavigationServerDummy::init() { return VoxelNavigationServer::get_singleton()->init(); }
void VoxelNavigationServerDummy::finish() { VoxelNavigationServer::get_singleton()->finish(); }
void VoxelNavigationServerDummy::tmp(int val) { VoxelNavigationServer::get_singleton()->tmp(val); }
