#ifndef NAV_VOLUME_UTILS_H
#define NAV_VOLUME_UTILS_H


#include "core/object/script_language.h"
#include "core/templates/oa_hash_map.h"


namespace ThetaStar {


template<class VectorType>
struct Point {
    struct Comparator {
        _FORCE_INLINE_ bool operator()(const Point<VectorType>* const a, const Point<VectorType>* const b) const {
            bool result = false;

            result = a->get_total_cost() > b->get_total_cost();

            if (!result) {
                if (a->get_total_cost() == a->get_total_cost()) {
                    result = a->cost_to_target < b->cost_to_target;
                }
            }

            return result;
        }
    };

    int64_t id = 0;
    VectorType position;
    real_t weight_scale = 0;
    bool enabled = true;

    OAHashMap<int64_t, Point<VectorType>*> neighbors = 4u;

    // These variables are used for pathfinding and may be outdated if trying
    // to use between path solving queries.
    Point<VectorType>* prevous_point = nullptr;
    real_t cost_from_start = 0; // g cost
    real_t cost_to_target = 0; // h cost
    uint8_t opened_counter = 0u;
    uint8_t closed_counter = 0u;

    Point<VectorType>() {}
    ~Point<VectorType>() {}

    real_t get_total_cost() const { // f cost
        return cost_from_start + cost_to_target;
    }
};

};


#endif // NAV_VOLUME_UTILS_H