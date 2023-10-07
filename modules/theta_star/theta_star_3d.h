#ifndef THETA_STAR_3D_H
#define THETA_STAR_3D_H


#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"
#include "core/object/script_language.h"

#include "utils.h"

#include "core/templates/oa_hash_map.h"

// final todos:: 
// - document how it is easy to do cell size in gdscript - just multiply each Vector3i by the cell size.
// - document how to use for good line of sight checks:
//      - use an external cell size and offset and have this graph stay at a cell size of 1,1,1
//      - simulating a cell size directly will mean that larger cells' borders are just ignored if should be disabled, it will be consider a sparse graph instead with free space
namespace ThetaStar {

class ThetaStar3D: public RefCounted {
    GDCLASS(ThetaStar3D, RefCounted);


protected:
    Vector3i _dimensions = Vector3i(1, 1, 1);
    OAHashMap<int64_t, Point<Vector3i>*> _points;
    int64_t number_of_disabled_points = 0;
    uint8_t closed_counter = 0u;


public:
    ThetaStar3D();
    virtual ~ThetaStar3D();
    
    void reserve(const uint32_t new_capacity);
    void clear();

    Vector3i get_dimensions() const;
    int64_t get_size() const;
    int64_t get_point_count() const;
    int64_t get_disabled_point_count() const;
    bool is_empty() const;
    int64_t get_capacity() const;
    int64_t get_point_id(const Vector3i position);
    Vector3i get_point_position(const int64_t id) const;
    int64_t get_point_hash(const Vector3i position);
    bool is_position_valid_for_hashing(const Vector3i position) const;
    bool is_id_disabled(const int64_t id) const;
    bool is_position_disabled(const Vector3i position);
    bool has_id(const int64_t id);
    bool has_point(const Vector3i position);

    TypedArray<Vector3i> get_points() const;
    TypedArray<Vector3i> get_point_connections(const Vector3i position);

    // todo:: solve issue when 'to' is not reachable from 'from'
    PackedInt64Array get_id_path_from_positions(const Vector3i from, const Vector3i to);
    TypedArray<Vector3i> get_point_path_from_positions(const Vector3i from, const Vector3i to);
    PackedInt64Array get_id_path_from_ids(const int64_t from, const int64_t to);
    TypedArray<Vector3i> get_point_path_from_ids(const int64_t from, const int64_t to);
    TypedArray<Vector3> get_point_path_from_off_graph_positions(const Vector3 from, const Vector3 to);

    bool add_point(const Vector3i position);
    bool remove_point(const Vector3i position); //todo:: unit test for sure

    bool disable_point_by_position(const Vector3i position, const bool disable = true);
    bool disable_point_by_id(const int64_t id, const bool disable = true);

    bool connect_points(const Vector3i from, const Vector3i to, const bool bidirectional = false);
    bool disconnect_points(const Vector3i from, const Vector3i to, const bool bidirectional = false); //todo:: unit test


protected:
    static void _bind_methods();
    static bool _are_dimensions_valid(const Vector3i& in_dimensions);

    bool _connect_points(const int64_t from_id, const int64_t to_id, const bool bidirectional = false);
    Point<Vector3i>* _get_closest_point_toward(const Vector3 from, const Vector3 to);
    Vector3i _get_closest_position_toward(const Vector3 from, const Vector3 to) const;

    void _get_point_path(Point<Vector3i>* const from, Point<Vector3i>* const to, LocalVector<const Point<Vector3i>*>& outPath);
    void _expand_point(Point<Vector3i>* const point, const Point<Vector3i>* const to, LocalVector<Point<Vector3i>*>& open, SortArray<Point<Vector3i>*, Point<Vector3i>::Comparator>& sorter);
    void _expand_point_helper(Point<Vector3i>* const previous_point, Point<Vector3i>* const neighbor, const Point<Vector3i>* const to, LocalVector<Point<Vector3i>*>& open, SortArray<Point<Vector3i>*, Point<Vector3i>::Comparator>& sorter);
    bool _has_line_of_sight_helper(LineOfSightArguments& args); //todo:: unit test for sure
    
    virtual bool _has_line_of_sight(Vector3i from, Vector3i to);
    virtual int64_t _hash_position(Vector3i position);
    virtual real_t _compute_edge_cost(int64_t from, int64_t to);
    virtual real_t _estimate_edge_cost(int64_t from, int64_t to);

    GDVIRTUAL2RC(bool, _has_line_of_sight, Vector3i, Vector3i)
    GDVIRTUAL1RC(int64_t, _hash_position, Vector3i)
    GDVIRTUAL2RC(real_t, _compute_edge_cost, int64_t, int64_t)
	GDVIRTUAL2RC(real_t, _estimate_edge_cost, int64_t, int64_t)

    bool _is_position_valid(const Vector3i position, const bool warn = false) const;
    virtual Vector3i _get_minimum_dimensions() const;
};

};


#endif // THETA_STAR_3D_H
