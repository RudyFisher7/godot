#ifndef THETA_STAR_UTILS_H
#define THETA_STAR_UTILS_H


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
    bool enabled = true;

    OAHashMap<int64_t, Point<VectorType>*> neighbors = 4u;

    // These variables are used for pathfinding and may be outdated if trying
    // to use between path solving queries.
    Point<VectorType>* previous_point = nullptr;
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


struct LineOfSightArguments {
public:
    int64_t balance = 0;
    int32_t small_axis_from = 0;

    Vector3i voxel_to_check_1;
    Vector3i voxel_to_check_2;
    Vector3i voxel_to_check_3;

private:
    int32_t big_axis_distance = 0;
    int32_t small_axis_distance = 0;
    int8_t small_axis_sign = 1;

public:
    LineOfSightArguments(
            const int32_t in_small_axis_from,
            const int32_t in_big_axis_distance,
            const int32_t in_small_axis_distance,
            const int8_t in_small_axis_sign
    ) {
        small_axis_from = in_small_axis_from;
        big_axis_distance = in_big_axis_distance;
        small_axis_distance = in_small_axis_distance;
        small_axis_sign = in_small_axis_sign;
    }

    const int32_t& get_big_axis_distance() const { return big_axis_distance; }
    const int32_t& get_small_axis_distance() const { return small_axis_distance; }
    const int8_t& get_small_axis_sign() const { return small_axis_sign; }
};

};


#endif // THETA_STAR_UTILS_H