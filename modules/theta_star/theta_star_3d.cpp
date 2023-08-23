#include "theta_star_3d.h"


namespace ThetaStar {


void ThetaStar3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_point", "position", "weight_scale"), &ThetaStar3D::add_point);
    ClassDB::bind_method(D_METHOD("connect_points", "from", "to", "bidirectional"), &ThetaStar3D::add_point);
    ClassDB::bind_method(D_METHOD("get_points"), &ThetaStar3D::get_points);
    ClassDB::bind_method(D_METHOD("get_id_path", "from", "to"), &ThetaStar3D::get_id_path);
    ClassDB::bind_method(D_METHOD("_hash_position", "position"), &ThetaStar3D::_hash_position);
}


bool ThetaStar3D::add_point(const Vector3i position, const real_t weight_scale) {
    bool result = false;
    Vector3i min = Vector3i(0, 0, 0);
    Vector3i max = _size - Vector3i(1, 1, 1);

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

    Point<Vector3i>* from_point = nullptr;
    Point<Vector3i>* to_point = nullptr;

    bool from_exists = _points.lookup(from_id, from_point);
    bool to_exists = _points.lookup(to_id, to_point);

    ERR_FAIL_COND_V_MSG(!from_exists, from_exists, vformat("Point 'from' doesn't exist. id = %d, position = %v", from_id, from));
    ERR_FAIL_COND_V_MSG(!to_exists, to_exists, vformat("Point 'to' doesn't exist. id = %d, position = %v", to_id, to));

    result = true;

    from_point->neighbors.insert(to_id, to_point);

    if (bidirectional)
    {
        to_point->neighbors.insert(from_id, from_point);
    }
    
    return result;
}


PackedVector3Array ThetaStar3D::get_points() const {
    PackedVector3Array vectors;

    for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = _points.iter(); it.valid; it = _points.next_iter(it)) {
		vectors.push_back((*(it.value))->position);
	}
    
    return vectors;
}

PackedInt64Array ThetaStar3D::get_id_path(const Vector3i from, const Vector3i to) const {
    return PackedInt64Array();
}


ThetaStar3D::ThetaStar3D(const Vector3i in_size) {
    _size = in_size;
}


ThetaStar3D::~ThetaStar3D() {
    _clear();
}


int64_t ThetaStar3D::_hash_position(const Vector3i position) const {
    int64_t result = 0;

    result = position.x + (_size.x * position.y) + (_size.x * _size.y * position.z);

    return result;
}


void ThetaStar3D::_clear() {
    for (OAHashMap<int64_t, Point<Vector3i>*>::Iterator it = _points.iter(); it.valid; it = _points.next_iter(it)) {
		memdelete(*(it.value));
	}
}

};