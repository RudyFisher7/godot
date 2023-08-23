#ifndef TEST_THETA_STAR_3D_H
#define TEST_THETA_STAR_3D_H


#include "tests/test_macros.h"

#include "modules/theta_star/theta_star_3d.h"


namespace TestThetaStar {

    TEST_CASE("[Modules][ThetaStar] Constructor") {
        Ref<ThetaStar::ThetaStar3D> t = memnew(ThetaStar::ThetaStar3D);

        REQUIRE(t != nullptr);
    }

};


#endif // TEST_THETA_STAR_3D_H