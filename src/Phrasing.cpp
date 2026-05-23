#include "plugin.hpp"
#include "CoreModules/CoreProcessor.hh"
#include "CoreModules/register_module.hh"
#include "rack.hpp"
#include <cmath>
#include <algorithm>

using namespace rack;

struct Phrasing : CoreProcessor {
    Phrasing() = default;

    void update() override {
        // Your main logic will go here
    }

    void set_samplerate(float sr) override {
    }

    void set_param(int param_id, float val) override {
    }

    void set_input(int input_id, float val) override {
    }

    float get_output(int output_id) const override {
        return 0.0f;
    }
};

// ==================== REGISTRATION ====================
MetaModule::ModuleInfoView info{
    .description = "Phrasing and presence controller",
    .width_hp = 12,
};

void init(rack::Plugin* p) {
    MetaModule::register_module<Phrasing>("THEREELPEET", "phrasingmm", info, "res/Phrasing.png");
}