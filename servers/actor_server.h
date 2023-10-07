#ifndef ACTOR_SERVER_H
#define ACTOR_SERVER_H


#include "core/object/object.h"
#include "core/os/thread.h"
#include "core/os/mutex.h"


class ActorServer : public Object {
    GDCLASS(ActorServer, Object);

private:
    static ActorServer* singleton;
    static void thread_function(ActorServer* instance);
private:
    bool thread_exited = false;
    mutable bool exit_thread = false;
    Thread* thread;
    Mutex* mutex;
public:
    ActorServer();
    ~ActorServer();
};



#endif // ACTOR_SERVER_H