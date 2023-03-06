#include "plugin.hpp"


struct Kaos : Module {
	enum ParamId {
		SWITCH_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		GATE_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};
	dsp::SchmittTrigger edgeDetector;

	Kaos() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configSwitch(SWITCH_PARAM, 0.0, 2.0, 2.0, "Output",{"1V","5V","10V"});
		configInput(GATE_INPUT, "");
		configOutput(OUT_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		int getswitch =params[SWITCH_PARAM].getValue();
		int voltage;
		if (getswitch==0){
			voltage=1;
		}
		if (getswitch==1){
			voltage=5;
		}
		if (getswitch==2){
			voltage=10;
		}
		float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/voltage));
		if (edgeDetector.process(inputs[GATE_INPUT].getVoltage())) {
			outputs [OUT_OUTPUT].setVoltage (r2);
		}
	}
};


struct KaosWidget : ModuleWidget {
	KaosWidget(Kaos* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Kaos.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<CKSSThreeHorizontal>(mm2px(Vec(7.62, 31.478)), module, Kaos::SWITCH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 49.074)), module, Kaos::GATE_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.62, 74.896)), module, Kaos::OUT_OUTPUT));
	}
};


Model* modelKaos = createModel<Kaos, KaosWidget>("Kaos");