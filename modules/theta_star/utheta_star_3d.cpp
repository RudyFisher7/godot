#include "modules/theta_star/utheta_star_3d.h"

#include "core/templates/oa_hash_map.h"


namespace ThetaStar {


Ref<UThetaStar3D> UThetaStar3D::create(const Vector3i in_dimensions, const bool reserve) {
    Ref<UThetaStar3D> result = memnew(UThetaStar3D(in_dimensions, reserve));
    return result;
}


UThetaStar3D::UThetaStar3D() { // todo:: inheritance stuff
    ERR_FAIL_MSG("UThetaStar3D can't be created directly. Use create() method.");
}


UThetaStar3D::UThetaStar3D(const Vector3i in_dimensions, const bool reserve) {
    ERR_FAIL_COND_MSG(!UThetaStar3D::_are_dimensions_valid(in_dimensions), vformat("UThetaStar3D's _dimensions must be set to a Vector with all positive values. _dimensions = %v", in_dimensions));
    
    _dimensions = in_dimensions;

    if (reserve) {
        uint32_t capacity = _dimensions.x * _dimensions.y * _dimensions.z;

        if (capacity > _points.get_capacity()) {
            _points.reserve(capacity);
        }
    }
}


UThetaStar3D::~UThetaStar3D() {
    clear();
}


void UThetaStar3D::build_bidirectional_grid(TypedArray<Vector3i> in_neighbors) {
    for (int32_t x = 0; x < _dimensions.x; x++) {
        for (int32_t y = 0; y < _dimensions.y; y++) {
            for (int32_t z = 0; z < _dimensions.z; z++) {
                add_point(Vector3i(x, y, z));
            }
        }
    }

    if (in_neighbors.is_empty()) {
        _build_default_neighbors(in_neighbors);
    }
    
    for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = _points.iter(); it.valid; it = _points.next_iter(it)) {
        Point<Vector3i>* point = *it.value;
        
        _connect_bidirectional_neighbors_in_grid(point, in_neighbors);
	}
}


void UThetaStar3D::_bind_methods() {
    ClassDB::bind_static_method("UThetaStar3D", D_METHOD("create", "in_dimensions", "reserve"), &UThetaStar3D::create, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("build_bidirectional_grid", "in_neighbors"), &UThetaStar3D::build_bidirectional_grid, DEFVAL(TypedArray<Vector3i>()));
}


void UThetaStar3D::_build_default_neighbors(TypedArray<Vector3i>& out_neighbors) const {
    out_neighbors.push_back(Vector3i(0, 0, 1));
    out_neighbors.push_back(Vector3i(0, 0, -1));
    out_neighbors.push_back(Vector3i(1, 0, 0));
    out_neighbors.push_back(Vector3i(-1, 0, 0));
    out_neighbors.push_back(Vector3i(0, 1, 0));
    out_neighbors.push_back(Vector3i(0, -1, 0));
}


void UThetaStar3D::_connect_bidirectional_neighbors_in_grid(Point<Vector3i>* const from_point, const TypedArray<Vector3i>& in_neighbors) {
    for (size_t i = 0; i < in_neighbors.size(); i++) {
        const Vector3i neighbor_position = from_point->position + in_neighbors[i];

        bool is_neighbor_position_valid = _is_position_valid(neighbor_position);

        if (is_neighbor_position_valid) {
            const int64_t neighbor_id = _hash_position(neighbor_position);
            Point<Vector3i>* neighbor_point = nullptr;

            if (_points.lookup(neighbor_id, neighbor_point)) {
                Vector3 p = Vector3(from_point->position.x, from_point->position.y, from_point->position.z);
                Vector3 n = Vector3(neighbor_point->position.x, neighbor_point->position.y, neighbor_point->position.z);
                if (p.distance_to(n) > 1.1) {
                    print_line(vformat("from = %v, to = %v, neighbor = %v", from_point->position, neighbor_point->position, in_neighbors[i]));
                }

                from_point->neighbors.set(neighbor_id, neighbor_point);
                neighbor_point->neighbors.set(from_point->id, from_point);
            }
        }
    }
}


int64_t UThetaStar3D::_hash_position(Vector3i position) {
    int64_t result = 0;

    result = position.x + (_dimensions.x * position.y) + (_dimensions.x * _dimensions.y * position.z);

    return result;
}


Vector3i UThetaStar3D::_get_minimum_dimensions() const {
    return Vector3i(0, 0, 0);
}

};