#ifndef NAV_VOLUME_UTILS_H
#define NAV_VOLUME_UTILS_H


#include "core/object/script_language.h"
#include "core/templates/oa_hash_map.h"


namespace ThetaStar {


template<class VectorType>
struct Point {
    int64_t id = 0;
    VectorType position;
    real_t weight_scale = 0;
    bool enabled = true;

    OAHashMap<int64_t, Point<VectorType>*> neighbors = 4u;
    OAHashMap<int64_t, Point<VectorType>*> unlinked_neighbors = 4u;

    Point<VectorType>* prevous_point_3d = nullptr;
    real_t cost_from_start = 0;
    real_t cost_from_target = 0;
};

};


#endif // NAV_VOLUME_UTILS_H