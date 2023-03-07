#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	// p->addModel(modelMyModule);
    p->addModel(modelManiac);
	p->addModel(modelSin);
	p->addModel(modelKaos);
	p->addModel(modelKlok);
	p->addModel(modelManiac2);
	p->addModel(modelAngryArp);
	p->addModel(modelManiac3);
	p->addModel(modelSaw);
	
	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
