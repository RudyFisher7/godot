#include "register_types.h"

#include "core/object/class_db.h"
#include "theta_star_3d.h"
#include "utheta_star_3d.h"

void initialize_theta_star_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }

    ClassDB::register_class<ThetaStar::ThetaStar3D>();
    ClassDB::register_class<ThetaStar::UThetaStar3D>();
}

void uninitialize_theta_star_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
   // Nothing to do here in this example.
}