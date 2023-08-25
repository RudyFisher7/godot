#include "theta_star_3d.h"


namespace ThetaStar {


void ThetaStar3D::_bind_methods() {
    ClassDB::bind_static_method("ThetaStar3D", D_METHOD("create", "in_size"), &ThetaStar3D::create);
    ClassDB::bind_method(D_METHOD("get_size"), &ThetaStar3D::get_size);
    ClassDB::bind_method(D_METHOD("get_point_id", "position"), &ThetaStar3D::get_point_id);
    ClassDB::bind_method(D_METHOD("get_point_position", "id"), &ThetaStar3D::get_point_position);
    ClassDB::bind_method(D_METHOD("get_point_hash", "position"), &ThetaStar3D::get_point_hash);
    ClassDB::bind_method(D_METHOD("get_points"), &ThetaStar3D::get_points);
    ClassDB::bind_method(D_METHOD("get_id_path", "from", "to"), &ThetaStar3D::get_id_path);
    ClassDB::bind_method(D_METHOD("get_point_path", "from", "to"), &ThetaStar3D::get_point_path);
    ClassDB::bind_method(D_METHOD("build_bidirectional_grid", "in_neighbors"), &ThetaStar3D::build_bidirectional_grid, DEFVAL(TypedArray<Vector3i>()));
    ClassDB::bind_method(D_METHOD("add_point", "position", "weight_scale"), &ThetaStar3D::add_point);
    ClassDB::bind_method(D_METHOD("connect_points", "from", "to", "bidirectional"), &ThetaStar3D::connect_points);

    ClassDB::bind_method(D_METHOD("_hash_position", "position"), &ThetaStar3D::_hash_position);


    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3I, "size"), "", "get_size");
}


Ref<ThetaStar3D> ThetaStar3D::create(const Vector3i in_size) {
    Ref<ThetaStar3D> result = memnew(ThetaStar3D(in_size));
    return result;
}


Vector3i ThetaStar3D::get_size() const {
    return size;
}


int64_t ThetaStar3D::get_point_id(const Vector3i position) const {
    int64_t result = -1;
    int64_t id = -1;
    
    id = get_point_hash(position);

    if (_points.has(id)) {
        result = id;
    }
    
    return result;
}


Vector3i ThetaStar3D::get_point_position(const int64_t id) const {
    Vector3i result;
    Point<Vector3i>* point = nullptr;

    if (_points.has(id)) {
        _points.lookup(id, point);

        result = point->position;
    } else {
        result = Vector3i(-1, -1, -1);
    }

    return result;
}


int64_t ThetaStar3D::get_point_hash(const Vector3i position) const {
    int64_t result = -1;
    bool is_position_valid = false;
    int64_t id = -1;
    
    is_position_valid = _is_position_valid(position);
    if (is_position_valid) {
        result = _hash_position(position);
    }
    
    return result;
}


TypedArray<Vector3i> ThetaStar3D::get_points() const {
    TypedArray<Vector3i> vectors;

    for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = _points.iter(); it.valid; it = _points.next_iter(it)) {
		vectors.push_back((*(it.value))->position);
	}
    
    return vectors;
}

PackedInt64Array ThetaStar3D::get_id_path(const Vector3i from, const Vector3i to) {
    PackedInt64Array result;
    bool is_from_valid = false;
    bool is_to_valid = false;
    int64_t from_id = 0;
    int64_t to_id = 0;
    Point<Vector3i>* from_point = nullptr;
    Point<Vector3i>* to_point = nullptr;

    is_from_valid = _is_position_valid(from);
    is_to_valid = _is_position_valid(to);

    if (is_from_valid && is_to_valid) {
        from_id = _hash_position(from);
        to_id = _hash_position(to);
    }

    if (is_from_valid && is_to_valid) {
        is_from_valid = _points.has(from_id);
        is_to_valid = _points.has(to_id);
    }

    if (is_from_valid && is_to_valid) {
        _points.lookup(from_id, from_point);
        _points.lookup(to_id, to_point);

        result = _get_id_path(from_point, to_point);
    }

    return result;
}


TypedArray<Vector3i> ThetaStar3D::get_point_path(const Vector3i from, const Vector3i to) {
    return TypedArray<Vector3i>();
}


void ThetaStar3D::build_bidirectional_grid(TypedArray<Vector3i> in_neighbors) {
    for (int32_t x = 0; x < size.x; x++) {
        for (int32_t y = 0; y < size.y; y++) {
            for (int32_t z = 0; z < size.z; z++) {
                add_point(Vector3i(x, y, z));
            }
        }
    }
    
    for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = _points.iter(); it.valid; it = _points.next_iter(it)) {
		const int64_t id = *it.key;
        Point<Vector3i>* point = *it.value;

        if (in_neighbors.is_empty()) {
            in_neighbors.push_back(Vector3i(0, 0, 1));
            in_neighbors.push_back(Vector3i(0, 0, -1));
            in_neighbors.push_back(Vector3i(1, 0, 0));
            in_neighbors.push_back(Vector3i(-1, 0, 0));
            in_neighbors.push_back(Vector3i(0, 1, 0));
            in_neighbors.push_back(Vector3i(0, -1, 0));
        }
        
        _connect_bidirectional_neighbors_in_grid(id, point, in_neighbors);
	}
}


bool ThetaStar3D::add_point(const Vector3i position, const real_t weight_scale) {
    bool result = false;
    
    result = _is_position_valid(position);

    if (result) {
        result = weight_scale >= 0.0;
        ERR_FAIL_COND_V_MSG(!result, result, vformat("Point's weight_scale must be non-negative. weight_scale = %f", weight_scale));

        int64_t id = _hash_position(position);

        Point<Vector3i>* point = memnew(Point<Vector3i>);

        point->id = id;
        point->position = position;
        point->weight_scale = weight_scale;

        _points.set(id, point);
    }

    return result;
}


bool ThetaStar3D::connect_points(const Vector3i from, const Vector3i to, const bool bidirectional) {
    bool result = false;

    int64_t from_id = _hash_position(from);
    int64_t to_id = _hash_position(to);

    result = _connect_points(from_id, to_id, bidirectional);
    
    return result;
}


ThetaStar3D::ThetaStar3D() {
    ERR_FAIL_MSG("ThetaStar3D can't be created directly. Use create() method.");
}


ThetaStar3D::ThetaStar3D(const Vector3i in_size) {
    ERR_FAIL_MSG(vformat("ThetaStar3D's size must be set to a Vector with all positive values. size = %v", in_size));

    size = in_size;
    _points.reserve(size.x * size.y * size.z);
}


ThetaStar3D::~ThetaStar3D() {
    _clear();
}


int64_t ThetaStar3D::_hash_position(const Vector3i position) const {
    int64_t result = 0;

    result = position.x + (size.x * position.y) + (size.x * size.y * position.z);

    return result;
}


bool ThetaStar3D::_connect_points(const int64_t from_id, const int64_t to_id, const bool bidirectional) {
    bool result = false;

    Point<Vector3i>* from_point = nullptr;
    Point<Vector3i>* to_point = nullptr;

    bool from_exists = _points.lookup(from_id, from_point);
    bool to_exists = _points.lookup(to_id, to_point);

    ERR_FAIL_COND_V_MSG(!from_exists, from_exists, vformat("Point 'from' doesn't exist. id = %d", from_id));
    ERR_FAIL_COND_V_MSG(!to_exists, to_exists, vformat("Point 'to' doesn't exist. id = %d", to_id));

    result = true;

    from_point->neighbors.insert(to_id, to_point);

    if (bidirectional)
    {
        to_point->neighbors.insert(from_id, from_point);
    }
    
    return result;
}


void ThetaStar3D::_connect_bidirectional_neighbors_in_grid(const int64_t from_id, Point<Vector3i>* from_point, const TypedArray<Vector3i> in_neighbors) {
    for (size_t i = 0; i < in_neighbors.size(); i++) {
        const int64_t neighbor_id = _hash_position(from_point->position + in_neighbors[i]);

        if (_points.has(neighbor_id)) {
            Point<Vector3i>* neighbor_point = nullptr;

            _points.lookup(neighbor_id, neighbor_point);
            
            if (!from_point->neighbors.has(neighbor_id)) {
                from_point->neighbors.insert(neighbor_id, neighbor_point);
            }

            if (!neighbor_point->neighbors.has(from_id)) {
                neighbor_point->neighbors.insert(from_id, from_point);
            }
        }
    }
}


PackedInt64Array ThetaStar3D::_get_id_path(Point<Vector3i>* const from, Point<Vector3i>* const to) {
    PackedInt64Array result;
    LocalVector<Point<Vector3i>*> open;
    SortArray<Point<Vector3i>*, Point<Vector3i>::Comparator> sorter;

    closed_counter++;

    from->cost_from_start = 0;
    from->cost_to_target = _estimate_edge_cost(from, to);

    open.push_back(from);

    while (!open.is_empty()) {
        Point<Vector3i>* current = open[0];

        if (current == to) {
            //then done
        } else {
            sorter.pop_heap(0, open.size(), open.ptr());
            open.remove_at(open.size() - 1);

            current->closed_counter = closed_counter;

            for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = current->neighbors.iter(); it.valid; it = current->neighbors.next_iter(it)) {
                Point<Vector3i>* const current_neighbor = *it.value;

                real_t current_cost_from_start = current->cost_from_start + _estimate_edge_cost(current, current_neighbor);

                if (
                        current_neighbor->enabled 
                        && (current_neighbor->opened_counter != closed_counter
                        || current_cost_from_start < current_neighbor->cost_from_start)
                ) {
                    current_neighbor->cost_from_start = current_cost_from_start;
                    current_neighbor->prevous_point = current;

                    if (current_neighbor->opened_counter != closed_counter) {
                        open.push_back(current_neighbor);
                        current_neighbor->opened_counter = closed_counter;

                        sorter.push_heap(0, open.size() - 1, 0, current_neighbor, open.ptr());
                    } else {
                        sorter.push_heap(0, open.find(current_neighbor), 0, current_neighbor, open.ptr());
                    }
                }
            }
        }
    }

    const Point<Vector3i>* point = to;
    while (point != from) {
        result.push_back(point->id);
        point = point->prevous_point;
    }

    result.push_back(point->id);

    result.reverse();

    return result;
}


TypedArray<Vector3i> ThetaStar3D::_get_position_path(const int64_t from, const int64_t to) {
    return TypedArray<Vector3i>();
}


real_t ThetaStar3D::_estimate_edge_cost(const Point<Vector3i>* const from, const Point<Vector3i>* const to) const {
    real_t result = 0.0;

    Vector3 from_position = Vector3(from->position);
    Vector3 to_position = Vector3(to->position);

    result = from_position.distance_to(to_position) * to->weight_scale;

    return result;
}


void ThetaStar3D::_clear() {
    for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = _points.iter(); it.valid; it = _points.next_iter(it)) {
		memdelete(*(it.value));
	}
}


bool ThetaStar3D::_is_position_valid(const Vector3i position) const {
    bool result = false;
    Vector3i min = Vector3i(0, 0, 0);
    Vector3i max = size - Vector3i(1, 1, 1);
    Vector3i clamped_position = position.clamp(min, max);

    ERR_FAIL_COND_V_MSG(position != clamped_position, result, vformat("Point's position must be non-negative and within size. position = %v", position));

    result = true;

    return result;
}

};