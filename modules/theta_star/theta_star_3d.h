#ifndef THETA_STAR_3D_H
#define THETA_STAR_3D_H


#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"
#include "core/object/script_language.h"

#include "utils.h"

#include "core/templates/oa_hash_map.h"


namespace ThetaStar {

class ThetaStar3D: public RefCounted {
    GDCLASS(ThetaStar3D, RefCounted);

protected:
    static void _bind_methods();


    Vector3i dimensions = Vector3i(1, 1, 1);
    OAHashMap<int64_t, Point<Vector3i>*> _points;
    uint8_t closed_counter = 0u;


public:
    static Ref<ThetaStar3D> create(const Vector3i in_dimensions, const bool reserve = false);
    
    Vector3i get_dimensions() const;
    int64_t get_size() const;
    int64_t get_capacity() const;
    int64_t get_point_id(const Vector3i position);
    Vector3i get_point_position(const int64_t id) const;
    int64_t get_point_hash(const Vector3i position);
    bool is_point_valid_for_hashing(const Vector3i position) const;
    TypedArray<Vector3i> get_points() const;
    TypedArray<Vector3i> get_point_connections(const Vector3i position);
    void reserve(const uint32_t new_capacity);
    PackedInt64Array get_id_path(const Vector3i from, const Vector3i to);
    TypedArray<Vector3i> get_point_path(const Vector3i from, const Vector3i to);

    void build_bidirectional_grid(TypedArray<Vector3i> in_neighbors = TypedArray<Vector3i>());

    bool add_point(const Vector3i position, const real_t weight_scale = 1.0);
    bool remove_point(const Vector3i position); //todo:: unit test for sure

    bool connect_points(const Vector3i from, const Vector3i to, const bool bidirectional = false);
    bool disconnect_points(const Vector3i from, const Vector3i to, const bool bidirectional = false); //todo:: implement and unit test

    ThetaStar3D();
    ThetaStar3D(const Vector3i in_dimensions, const bool reserve = false);
    virtual ~ThetaStar3D();

protected:
    static bool _are_dimensions_valid(const Vector3i& in_dimensions);
    bool _connect_points(const int64_t from_id, const int64_t to_id, const bool bidirectional = false);
    void _connect_bidirectional_neighbors_in_grid(Point<Vector3i>* const from_point, const TypedArray<Vector3i>& in_neighbors);

    PackedInt64Array _get_id_path(Point<Vector3i>* const from, Point<Vector3i>* const to);
    TypedArray<Vector3i> _get_position_path(const int64_t from, const int64_t to);
    void _expand_point(Point<Vector3i>* const point, const Point<Vector3i>* const to, LocalVector<Point<Vector3i>*>& open, SortArray<Point<Vector3i>*, Point<Vector3i>::Comparator>& sorter);

    virtual int64_t _hash_position(Vector3i position);
    virtual real_t _compute_edge_cost(int64_t from, int64_t to);
    virtual real_t _estimate_edge_cost(int64_t from, int64_t to);

    GDVIRTUAL1RC(int64_t, _hash_position, Vector3i)
    GDVIRTUAL2RC(real_t, _compute_edge_cost, int64_t, int64_t)
	GDVIRTUAL2RC(real_t, _estimate_edge_cost, int64_t, int64_t)

    virtual void _clear();
    
    bool _is_position_valid(const Vector3i position, bool warn = false) const;

    void _build_default_neighbors(TypedArray<Vector3i>& out_neighbors) const;
};

};


#endif // THETA_STAR_3D_H
