#include "register_types.h"

#include "core/object/class_db.h"
#include "summator.h"
#include "theta_star_3d.h"

void initialize_navigation_volume_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }

    ClassDB::register_class<Summator>();
    ClassDB::register_class<ThetaStar::ThetaStar3D>();
}

void uninitialize_navigation_volume_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
   // Nothing to do here in this example.
}