#include "plugin.hpp"


struct Maniac : Module {
	enum ParamId {
		STEP1_PARAM,
		STEP2_PARAM,
		STEP3_PARAM,
		STEP4_PARAM,
		STEP5_PARAM,
		STEP6_PARAM,
		STEP7_PARAM,
		STEP8_PARAM,
		STEP9_PARAM,
		STEP10_PARAM,
		STEP11_PARAM,
		STEP12_PARAM,
		STEP13_PARAM,
		STEP14_PARAM,
		STEP15_PARAM,
		STEP16_PARAM,
		SWITCH_PARAM,
		STEPS_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		IN_INPUT,
		RESET_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHT_STEP_1_LIGHT,
		LIGHT_STEP_2_LIGHT,
		LIGHT_STEP_3_LIGHT,
		LIGHT_STEP_4_LIGHT,
		LIGHT_STEP_5_LIGHT,
		LIGHT_STEP_6_LIGHT,
		LIGHT_STEP_7_LIGHT,
		LIGHT_STEP_8_LIGHT,
		LIGHT_STEP_9_LIGHT,
		LIGHT_STEP_10_LIGHT,
		LIGHT_STEP_11_LIGHT,
		LIGHT_STEP_12_LIGHT,
		LIGHT_STEP_13_LIGHT,
		LIGHT_STEP_14_LIGHT,
		LIGHT_STEP_15_LIGHT,
		LIGHT_STEP_16_LIGHT,
		LIGHTS_LEN
	};

	
	dsp::SchmittTrigger edgeDetector;
	int stepNr=-1;
	bool initialHit=true;


	Maniac() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configSwitch(SWITCH_PARAM, 0.0f, 1.0f, 1.0f, "Mode", {"Seq.", "Random"});
		for (int i=0; i< Maniac::LIGHTS_LEN; i++){
			configParam(STEP1_PARAM+i,0.0,1.0,0.5);
		};
		configParam(STEPS_PARAM, 1.0f, 16.0f, 16.0f, "Steps");
		paramQuantities[STEPS_PARAM]->snapEnabled = true;
		initialHit=true;
		stepNr=-1;
	}

	void process(const ProcessArgs& args) override {
	int pulses = (int)fmaxf(params[STEPS_PARAM].getValue(), 1.0f); 
	if(initialHit){
		 stepNr=-1;
		initialHit=false;
	
	}
	if (stepNr>0){
    outputs [OUT_OUTPUT].setVoltage (params[stepNr].getValue());
	}
	else{
	outputs [OUT_OUTPUT].setVoltage (params[0].getValue());
	}
	if (params[SWITCH_PARAM].getValue()==0){
	if (edgeDetector.process(inputs[IN_INPUT].getVoltage())) {
			stepNr=(stepNr+1) % pulses;
	}
	}
	else {
	if (edgeDetector.process(inputs[IN_INPUT].getVoltage())){
			stepNr= rand() % pulses;
	}
	}
	if (inputs[RESET_INPUT].isConnected()){
		if (inputs[RESET_INPUT].getVoltage() >0){
			stepNr=-1;
			initialHit=true;
		}
	}
	for (int l=0; l<LIGHTS_LEN; l++){
		lights[l].setSmoothBrightness (l==stepNr, 5e-6f);
	}
	
	
	}
};


struct ManiacWidget : ModuleWidget {
	ManiacWidget(Maniac* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Maniac.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(19.715, 31.055)), module, Maniac::STEP1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(38.189, 31.055)), module, Maniac::STEP2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(56.664, 31.055)), module, Maniac::STEP3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.138, 31.055)), module, Maniac::STEP4_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(19.715, 48.928)), module, Maniac::STEP5_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(38.189, 48.928)), module, Maniac::STEP6_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(56.664, 48.928)), module, Maniac::STEP7_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.138, 48.928)), module, Maniac::STEP8_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(19.715, 66.8)), module, Maniac::STEP9_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(38.189, 66.8)), module, Maniac::STEP10_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(56.664, 66.8)), module, Maniac::STEP11_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.138, 66.8)), module, Maniac::STEP12_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(19.715, 84.672)), module, Maniac::STEP13_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(38.189, 84.672)), module, Maniac::STEP14_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(56.664, 84.672)), module, Maniac::STEP15_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(75.138, 84.672)), module, Maniac::STEP16_PARAM));
		addParam(createParamCentered<CKSS>(mm2px(Vec(50.672, 107.47)), module, Maniac::SWITCH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(88.021, 109.545)), module, Maniac::STEPS_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.412, 107.604)), module, Maniac::IN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(33.344, 107.418)), module, Maniac::RESET_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(69.101, 108.777)), module, Maniac::OUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.6, 31.651)), module, Maniac::LIGHT_STEP_1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(47.409, 31.651)), module, Maniac::LIGHT_STEP_2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(66.217, 31.651)), module, Maniac::LIGHT_STEP_3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(85.025, 31.651)), module, Maniac::LIGHT_STEP_4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.6, 49.136)), module, Maniac::LIGHT_STEP_5_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(47.409, 49.136)), module, Maniac::LIGHT_STEP_6_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(66.217, 49.136)), module, Maniac::LIGHT_STEP_7_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(85.025, 49.136)), module, Maniac::LIGHT_STEP_8_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.6, 66.622)), module, Maniac::LIGHT_STEP_9_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(47.409, 66.622)), module, Maniac::LIGHT_STEP_10_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(66.217, 66.622)), module, Maniac::LIGHT_STEP_11_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(85.025, 66.622)), module, Maniac::LIGHT_STEP_12_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(28.6, 84.107)), module, Maniac::LIGHT_STEP_13_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(47.409, 84.107)), module, Maniac::LIGHT_STEP_14_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(66.217, 84.107)), module, Maniac::LIGHT_STEP_15_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(85.025, 84.107)), module, Maniac::LIGHT_STEP_16_LIGHT));
	}
};


Model* modelManiac = createModel<Maniac, ManiacWidget>("Maniac");