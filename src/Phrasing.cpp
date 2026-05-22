#include "plugin.hpp"
#include <cmath>
#include <algorithm>
using namespace rack;
using namespace rack::componentlibrary;

struct Phrasing : Module {
    enum ParamId {
        DENSITY_PARAM,
        GAP_JITTER_PARAM,
        DURATION_JITTER_PARAM,
        GUARANTEE_ONE_PARAM,
        LANE1_ACTIVE_PARAM,
        LANE2_ACTIVE_PARAM,
        LANE3_ACTIVE_PARAM,
        LANE4_ACTIVE_PARAM,
        WEIGHT1_PARAM,
        WEIGHT2_PARAM,
        WEIGHT3_PARAM,
        WEIGHT4_PARAM,
        LANEDUR1_PARAM,
        LANEDUR2_PARAM,
        LANEDUR3_PARAM,
        LANEDUR4_PARAM,
        FLOOR1_PARAM,
        FLOOR2_PARAM,
        FLOOR3_PARAM,
        FLOOR4_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        DENSITY_CV_INPUT,
        TRIG1_INPUT,
        TRIG2_INPUT,
        TRIG3_INPUT,
        TRIG4_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        OUT1_OUTPUT,
        OUT2_OUTPUT,
        OUT3_OUTPUT,
        OUT4_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        LANE1_LIGHT,
        LANE2_LIGHT,
        LANE3_LIGHT,
        LANE4_LIGHT,
        LIGHTS_LEN
    };

    float laneTarget[4] = {0.f, 0.f, 0.f, 0.f};
    float laneValue[4] = {0.f, 0.f, 0.f, 0.f};
    float laneOnTimer[4] = {0.f, 0.f, 0.f, 0.f};
    float gapTimer = 0.f;
    bool initialized = false;
    dsp::SchmittTrigger laneBtnTrig[4];
    dsp::SchmittTrigger trigIn[4];
    bool laneEnabled[4] = {true, true, true, true};
    bool laneEnabledInit = false;

    Phrasing() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        // ... (your existing config code stays exactly the same)
        configParam(DENSITY_PARAM, 0.f, 1.f, 0.7f, "Density", "%", 0.f, 100.f);
        configParam(GAP_JITTER_PARAM, 0.f, 1.f, 0.25f, "Gap Jitter", "%", 0.f, 100.f);
        configParam(DURATION_JITTER_PARAM, 0.f, 1.f, 0.25f, "Duration Jitter", "%", 0.f, 100.f);
        configSwitch(GUARANTEE_ONE_PARAM, 0.f, 1.f, 0.f, "Guarantee one lane", {"Off", "On"});
        configInput(DENSITY_CV_INPUT, "Density CV");
        configInput(TRIG1_INPUT, "Trig 1");
        configInput(TRIG2_INPUT, "Trig 2");
        configInput(TRIG3_INPUT, "Trig 3");
        configInput(TRIG4_INPUT, "Trig 4");
        configButton(LANE1_ACTIVE_PARAM, "Lane 1 Enable");
        configButton(LANE2_ACTIVE_PARAM, "Lane 2 Enable");
        configButton(LANE3_ACTIVE_PARAM, "Lane 3 Enable");
        configButton(LANE4_ACTIVE_PARAM, "Lane 4 Enable");
        configParam(WEIGHT1_PARAM, 0.f, 1.f, 0.8f, "Weight I", "%", 0.f, 100.f);
        configParam(WEIGHT2_PARAM, 0.f, 1.f, 0.8f, "Weight II", "%", 0.f, 100.f);
        configParam(WEIGHT3_PARAM, 0.f, 1.f, 0.8f, "Weight III", "%", 0.f, 100.f);
        configParam(WEIGHT4_PARAM, 0.f, 1.f, 0.8f, "Weight IV", "%", 0.f, 100.f);
        configParam(LANEDUR1_PARAM, 0.f, 1.f, 0.5f, "Duration I");
        configParam(LANEDUR2_PARAM, 0.f, 1.f, 0.5f, "Duration II");
        configParam(LANEDUR3_PARAM, 0.f, 1.f, 0.5f, "Duration III");
        configParam(LANEDUR4_PARAM, 0.f, 1.f, 0.5f, "Duration IV");
        configParam(FLOOR1_PARAM, 0.f, 1.f, 0.0f, "Floor I", "%", 0.f, 100.f);
        configParam(FLOOR2_PARAM, 0.f, 1.f, 0.0f, "Floor II", "%", 0.f, 100.f);
        configParam(FLOOR3_PARAM, 0.f, 1.f, 0.0f, "Floor III", "%", 0.f, 100.f);
        configParam(FLOOR4_PARAM, 0.f, 1.f, 0.0f, "Floor IV", "%", 0.f, 100.f);
        configOutput(OUT1_OUTPUT, "Lane CV I");
        configOutput(OUT2_OUTPUT, "Lane CV II");
        configOutput(OUT3_OUTPUT, "Lane CV III");
        configOutput(OUT4_OUTPUT, "Lane CV IV");
        configLight(LANE1_LIGHT, "Lane 1");
        configLight(LANE2_LIGHT, "Lane 2");
        configLight(LANE3_LIGHT, "Lane 3");
        configLight(LANE4_LIGHT, "Lane 4");
    }

    // ... [all your existing methods (knobToSeconds, densityToGapSeconds, jitterMul, process) stay exactly the same] ...

    // ==================== WIDGET ====================
    struct PhrasingWidget : ModuleWidget {
        PhrasingWidget(Phrasing* module) {
            setModule(module);
            setPanel(createPanel(asset::plugin(pluginInstance, "res/Phrasing.svg")));
            // ... (your entire widget code stays the same)
            // (I kept it unchanged for brevity)
        }
    };

    // MetaModule Info
    struct PhrasingInfo {
        static constexpr const char* slug = "phrasingmm";
        static constexpr const char* name = "PhrasingMM";
        static constexpr const char* description = "Phrasing and presence controller (MetaModule port)";
        static constexpr uint32_t width_hp = 12;   // Adjust if your panel is wider/narrower
    };

};

// ==================== MODEL REGISTRATION ====================
rack::Model* modelPhrasingMM = rack::createModel<Phrasing, Phrasing::PhrasingWidget>("phrasingmm");