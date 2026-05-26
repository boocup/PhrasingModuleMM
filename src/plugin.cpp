#include "plugin.hpp"

extern Plugin* pluginInstance;

void init_PhrasingModuleMM(Plugin* p) {
    pluginInstance = p;
    p->addModel(modelPhrasingMM);
}