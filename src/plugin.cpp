#include "plugin.hpp"

#ifdef METAMODULE_BUILTIN
extern Plugin* pluginInstance;
#else
Plugin* pluginInstance;
#endif

#ifdef METAMODULE_BUILTIN
void init_PhrasingModuleMM(Plugin* p) {
#else
void init(Plugin* p) {
#endif
    pluginInstance = p;
    p->addModel(modelPhrasingMM);
}
