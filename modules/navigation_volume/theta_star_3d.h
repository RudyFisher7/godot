#ifndef THETA_STAR_3D_H
#define THETA_STAR_3D_H


#include "core/object/ref_counted.h"


namespace ThetaStar {

class ThetaStar3D: public RefCounted {
    GDCLASS(ThetaStar3D, RefCounted);

protected:
    static void _bind_methods();


public:
    ThetaStar3D();

};

};


#endif // THETA_STAR_3D_H