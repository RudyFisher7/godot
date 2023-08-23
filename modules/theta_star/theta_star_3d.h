#ifndef THETA_STAR_3D_H
#define THETA_STAR_3D_H


#include "core/object/ref_counted.h"

#include "utils.h"

#include "core/templates/oa_hash_map.h"


namespace ThetaStar {

class ThetaStar3D: public RefCounted {
    GDCLASS(ThetaStar3D, RefCounted);

protected:
    static void _bind_methods();


    Vector3i _size;
    OAHashMap<int64_t, Point<Vector3i>*> _points;


public:
    bool add_point(const Vector3i position, const real_t weight_scale = 1.0);
    bool connect_points(const Vector3i from, const Vector3i to, const bool bidirectional = false);
    PackedVector3Array get_points() const;

    PackedInt64Array get_id_path(const Vector3i from, const Vector3i to) const;

    ThetaStar3D(const Vector3i in_size = Vector3i(8, 8, 8));
    virtual ~ThetaStar3D();

protected:
    int64_t _hash_position(const Vector3i position) const;

    void _clear();
};

};


#endif // THETA_STAR_3D_H