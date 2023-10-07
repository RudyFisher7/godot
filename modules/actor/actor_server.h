#ifndef ACTOR_SERVER_H
#define ACTOR_SERVER_H


#include "core/object/object.h"
#include "core/os/thread.h"
#include "core/os/mutex.h"
#include "core/templates/rid.h"
#include "core/variant/variant.h"


class ActorServer : public Object {
    GDCLASS(ActorServer, Object);

private:
    static ActorServer* singleton;

    bool thread_exited = false;
    mutable bool exit_thread = false;
    Thread* thread = nullptr;
    Mutex* mutex = nullptr;


public:
    ActorServer();
    ~ActorServer();

    static ActorServer* get_singleton();

    Error init();
    void lock();
    void unlock();
    void finish();


protected:
    static void _bind_methods();


private:
    static void thread_function(void* instance);

};



#endif // ACTOR_SERVER_H