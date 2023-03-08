#include "plugin.hpp"


struct Volta : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		CH1_INPUT,
		CH2_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUTS_LEN
	};
	enum LightId {
		CH1_LIGHT1_LIGHT,
		CH2_LIGHT1_LIGHT,
		CH1_LIGHT2_LIGHT,
		CH2_LIGHT2_LIGHT,
		CH1_LIGHT3_LIGHT,
		CH2_LIGHT3_LIGHT,
		CH1_LIGHT4_LIGHT,
		CH2_LIGHT4_LIGHT,
		CH1_LIGHT5_LIGHT,
		CH2_LIGHT5_LIGHT,
		CH1_LIGHT6_LIGHT,
		CH2_LIGHT6_LIGHT,
		CH1_LIGHT7_LIGHT,
		CH2_LIGHT7_LIGHT,
		CH1_LIGHT8_LIGHT,
		CH2_LIGHT8_LIGHT,
		CH1_LIGHT9_LIGHT,
		CH2_LIGHT9_LIGHT,
		CH1_LIGHT10_LIGHT,
		CH2_LIGHT10_LIGHT,
		LIGHTS_LEN
	};

	Volta() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(CH1_INPUT, "Channel 1");
		configInput(CH2_INPUT, "Channel 2");
	}

	void process(const ProcessArgs& args) override {
		float ch1 = inputs[CH1_INPUT].getVoltage();
		float ch2 = inputs[CH2_INPUT].getVoltage();
		if (ch1>0) lights[CH1_LIGHT10_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=1) lights[CH1_LIGHT9_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=2) lights[CH1_LIGHT8_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=3) lights[CH1_LIGHT7_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=4) lights[CH1_LIGHT6_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=5) lights[CH1_LIGHT5_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=6) lights[CH1_LIGHT4_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=7) lights[CH1_LIGHT3_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=8) lights[CH1_LIGHT2_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch1>=9) lights[CH1_LIGHT1_LIGHT].setSmoothBrightness (1.f,5e-6f);

		if (ch2>0) lights[CH2_LIGHT10_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=1) lights[CH2_LIGHT9_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=2) lights[CH2_LIGHT8_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=3) lights[CH2_LIGHT7_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=4) lights[CH2_LIGHT6_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=5) lights[CH2_LIGHT5_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=6) lights[CH2_LIGHT4_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=7) lights[CH2_LIGHT3_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=8) lights[CH2_LIGHT2_LIGHT].setSmoothBrightness (1.f,5e-6f);
		if (ch2>=9) lights[CH2_LIGHT1_LIGHT].setSmoothBrightness (1.f,5e-6f);

		for (int l=0; l<LIGHTS_LEN; l++){
			lights[l].setSmoothBrightness (0.f, 5e-6f);
	          }
		
	}
};


struct VoltaWidget : ModuleWidget {
	VoltaWidget(Volta* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Volta.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.519, 111.422)), module, Volta::CH1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.121, 111.422)), module, Volta::CH2_INPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 19.274)), module, Volta::CH1_LIGHT1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 19.274)), module, Volta::CH2_LIGHT1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 28.488)), module, Volta::CH1_LIGHT2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 28.488)), module, Volta::CH2_LIGHT2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 37.703)), module, Volta::CH1_LIGHT3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 37.703)), module, Volta::CH2_LIGHT3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 46.918)), module, Volta::CH1_LIGHT4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 46.918)), module, Volta::CH2_LIGHT4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 56.133)), module, Volta::CH1_LIGHT5_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 56.133)), module, Volta::CH2_LIGHT5_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 65.348)), module, Volta::CH1_LIGHT6_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 65.348)), module, Volta::CH2_LIGHT6_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 74.562)), module, Volta::CH1_LIGHT7_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 74.562)), module, Volta::CH2_LIGHT7_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 83.777)), module, Volta::CH1_LIGHT8_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 83.777)), module, Volta::CH2_LIGHT8_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 92.992)), module, Volta::CH1_LIGHT9_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 92.992)), module, Volta::CH2_LIGHT9_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(12.519, 102.207)), module, Volta::CH1_LIGHT10_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.121, 102.207)), module, Volta::CH2_LIGHT10_LIGHT));
	}
};


Model* modelVolta = createModel<Volta, VoltaWidget>("Volta");