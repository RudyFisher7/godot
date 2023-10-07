#include "actor_server.h"


ActorServer* ActorServer::singleton = nullptr;


ActorServer::ActorServer() {
    //
}


ActorServer::~ActorServer() {
    //
}


ActorServer* ActorServer::get_singleton() {
    return nullptr;
}


Error ActorServer::init() {
    Error result = Error::OK;

    return result;
}


void ActorServer::lock() {
    //
}


void ActorServer::unlock() {
    //
}


void ActorServer::finish() {
    //
}


void ActorServer::_bind_methods() {
    //
}


void ActorServer::thread_function(void* instance) {
    //
}
