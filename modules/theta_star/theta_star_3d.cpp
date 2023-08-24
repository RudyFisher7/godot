#include "theta_star_3d.h"


namespace ThetaStar {


void ThetaStar3D::_bind_methods() {
    ClassDB::bind_static_method("ThetaStar3D", D_METHOD("create", "in_size"), &ThetaStar3D::create);
    ClassDB::bind_method(D_METHOD("get_size"), &ThetaStar3D::get_size);
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


TypedArray<Vector3i> ThetaStar3D::get_points() const {
    TypedArray<Vector3i> vectors;

    for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = _points.iter(); it.valid; it = _points.next_iter(it)) {
		vectors.push_back((*(it.value))->position);
	}
    
    return vectors;
}

PackedInt64Array ThetaStar3D::get_id_path(const Vector3i from, const Vector3i to) {
    return PackedInt64Array();
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
    Vector3i min = Vector3i(0, 0, 0);
    Vector3i max = size - Vector3i(1, 1, 1);

    ERR_FAIL_COND_V_MSG(size <= min, result, vformat("Size must be set to a Vector with all positive values. size = %v", size));
    
    Vector3i clamped_position = position.clamp(min, max);

    ERR_FAIL_COND_V_MSG(position != clamped_position, result, vformat("Point's position must be non-negative and within size. position = %v", position));
	ERR_FAIL_COND_V_MSG(weight_scale < 0.0, result, vformat("Point's weight_scale must be non-negative. weight_scale = %f", weight_scale));

    int64_t id = _hash_position(position);

    result = true;

    Point<Vector3i>* point = memnew(Point<Vector3i>);

    point->id = id;
    point->position = position;
    point->weight_scale = weight_scale;

    _points.set(id, point);

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


void ThetaStar3D::_clear() {
    for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = _points.iter(); it.valid; it = _points.next_iter(it)) {
		memdelete(*(it.value));
	}
}

};