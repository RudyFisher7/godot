#ifndef UTHETA_STAR_3D_H
#define UTHETA_STAR_3D_H


#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"
#include "core/object/script_language.h"

#include "modules/theta_star/theta_star_3d.h"
#include "utils.h"


// final todos:: allow cell height width depth scaling?
namespace ThetaStar {

class UThetaStar3D: public ThetaStar3D {
    GDCLASS(UThetaStar3D, ThetaStar3D);


public:
    static Ref<UThetaStar3D> create(const Vector3i in_dimensions, const bool reserve = false);

    UThetaStar3D();
    UThetaStar3D(const Vector3i in_dimensions, const bool reserve = false);
    virtual ~UThetaStar3D();

    void build_bidirectional_grid(TypedArray<Vector3i> in_neighbors = TypedArray<Vector3i>());


protected:
    static void _bind_methods();

    void _build_default_neighbors(TypedArray<Vector3i>& out_neighbors) const;
    void _connect_bidirectional_neighbors_in_grid(Point<Vector3i>* const from_point, const TypedArray<Vector3i>& in_neighbors);
    int64_t _hash_position(Vector3i position) override;
    Vector3i _get_minimum_dimensions() const override;
};

};


#endif // UTHETA_STAR_3D_H
