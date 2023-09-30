#ifndef TEST_THETA_STAR_H
#define TEST_THETA_STAR_H


#include "tests/test_macros.h"

#include "modules/theta_star/theta_star_3d.h"


namespace TestThetaStar {

    TEST_SUITE_BEGIN("ThetaStar");

    TEST_CASE("default id hashing uniqueness") {
        Vector3i size_for_hash_tests = Vector3i(1, 1, 1);
        int32_t count = 0xFFFFFFFE;

        for (int32_t x = 1; x < count; x++) {
            size_for_hash_tests.x = x;

            for (int32_t y = 1; y < count; y++) {
                size_for_hash_tests.y = y;

                for (int32_t z = 1; z < count; z++) {
                    size_for_hash_tests.z = z;

                    Ref<ThetaStar::ThetaStar3D> t = ThetaStar::ThetaStar3D::create(size_for_hash_tests);

                    Vector3i size = t->get_dimensions();
                    int record_count = static_cast<int>(size.x * size.y * size.z);
                    Vector<uint8_t> record_of_hashes = Vector<uint8_t>();
                    
                    record_of_hashes.resize(record_count);
                    record_of_hashes.fill(0u);

                    for (int32_t point_x = 0; point_x < size.x; point_x++) {
                        for (int32_t point_y = 0; point_y < size.y; point_y++) {
                            for (int32_t point_z = 0; point_z < size.z; point_z++) {
                                Vector3i point_position = Vector3i(point_x, point_y, point_z);
                                CHECK(t->is_position_valid_for_hashing(point_position));

                                int64_t id = t->get_point_hash(point_position);
                                record_of_hashes.set(id, record_of_hashes.get(id) + 1u);
                                CHECK(record_of_hashes[id] == 1u);
                            }
                        }
                    }

                    for (int i = 0; i < record_of_hashes.size(); i++)
                    {
                        CHECK(record_of_hashes.get(i) == 1u);
                    }

                    memdelete<Ref<ThetaStar::ThetaStar3D>>(&t);
                }
            }
        }
    }

    TEST_CASE("static create") {

        SUBCASE("Invalid") {
            Ref<ThetaStar::ThetaStar3D> t = ThetaStar::ThetaStar3D::create(Vector3i(0, 0, 0));

            CHECK(t != nullptr);
            CHECK(t->get_dimensions() == Vector3i(1, 1, 1));

            memdelete<Ref<ThetaStar::ThetaStar3D>>(&t);
        }

        SUBCASE("valid") {
            Ref<ThetaStar::ThetaStar3D> t = ThetaStar::ThetaStar3D::create(Vector3i(1, 1, 1));

            CHECK(t != nullptr);

            memdelete<Ref<ThetaStar::ThetaStar3D>>(&t);
        }

        SUBCASE("valid with size = (1000, 1000, 1000)") {
            int32_t val = 1000;
            Ref<ThetaStar::ThetaStar3D> t = ThetaStar::ThetaStar3D::create(Vector3i(val, val, val));

            CHECK(t != nullptr);
            CHECK(t->get_capacity() == static_cast<int64_t>(val * val * val));

            memdelete<Ref<ThetaStar::ThetaStar3D>>(&t);
        }
    }

    TEST_CASE("constructor") {
        Ref<ThetaStar::ThetaStar3D> t = memnew(ThetaStar::ThetaStar3D);

        CHECK(t != nullptr);
        CHECK(t->get_dimensions() == Vector3i(1, 1, 1));

        memdelete<Ref<ThetaStar::ThetaStar3D>>(&t);
    }

    TEST_SUITE_END(); // ThetaStar

} // namespace TestThetaStar


#endif // TEST_THETA_STAR_H
