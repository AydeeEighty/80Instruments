#include "plugin.hpp"


struct AngryArp : Module {
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
		STEPS_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		GATE_INPUT,
		VOCT_INPUT,
		CLOCK_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		GATEOUT_OUTPUT,
		OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHT1_LIGHT,
		LIGHT2_LIGHT,
		LIGHT3_LIGHT,
		LIGHT4_LIGHT,
		LIGHT5_LIGHT,
		LIGHT6_LIGHT,
		LIGHT7_LIGHT,
		LIGHT8_LIGHT,
		LIGHT9_LIGHT,
		LIGHT10_LIGHT,
		LIGHT11_LIGHT,
		LIGHT12_LIGHT,
		LIGHT13_LIGHT,
		LIGHT14_LIGHT,
		LIGHT15_LIGHT,
		LIGHT16_LIGHT,
		LIGHTS_LEN
	};
	dsp::SchmittTrigger edgeDetector;
	int stepNr=0;

	AngryArp() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		for (int i=0;i<16; i++){
			configParam(STEP1_PARAM+i,-2.0,2.0,0.0,"Octave");
			paramQuantities[STEP1_PARAM+i]->snapEnabled = true;
		};
		configParam(STEPS_PARAM, 1.0f, 16.0f, 16.0f, "Steps");
		paramQuantities[STEPS_PARAM]->snapEnabled = true;
		configInput(GATE_INPUT, "Gate");
		configInput(VOCT_INPUT, "V/Oct");
		configInput(CLOCK_INPUT, "Clock");
		configOutput(GATEOUT_OUTPUT, "Gate");
		configOutput(OUT_OUTPUT, "Main");
	}

	void process(const ProcessArgs& args) override {
		float pitch = inputs[VOCT_INPUT].getVoltage();
		
		float octave =params[stepNr].getValue();
		float arp = octave*12*0.08333 ;


		int pulses = (int)fmaxf(params[STEPS_PARAM].getValue(), 1.0f); 
		float gate_on = inputs[GATE_INPUT].getVoltage();

		if (gate_on>1){
		if (edgeDetector.process(inputs[CLOCK_INPUT].getVoltage())) {
			stepNr=(stepNr+1) % pulses;
	      }
		}
		else{
			stepNr=0;
			arp=0;
			
		}
		if (inputs[GATE_INPUT].getVoltage()>1){
			outputs [GATEOUT_OUTPUT].setVoltage(10.0f);
		}
		else {
			outputs [GATEOUT_OUTPUT].setVoltage(0.0f);
		}
		for (int l=0; l<LIGHTS_LEN; l++){
			lights[l].setSmoothBrightness (l==stepNr, 5e-6f);
	          }
	    outputs [OUT_OUTPUT].setVoltage (pitch+arp);
	}
};


struct AngryArpWidget : ModuleWidget {
	AngryArpWidget(AngryArp* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/AngryArp.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(20.433, 33.4)), module, AngryArp::STEP1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(40.494, 33.4)), module, AngryArp::STEP2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(60.556, 33.4)), module, AngryArp::STEP3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(80.617, 33.4)), module, AngryArp::STEP4_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(20.433, 53.2)), module, AngryArp::STEP5_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(40.494, 53.2)), module, AngryArp::STEP6_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(60.556, 53.2)), module, AngryArp::STEP7_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(80.617, 53.2)), module, AngryArp::STEP8_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(20.433, 72.999)), module, AngryArp::STEP9_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(40.494, 72.999)), module, AngryArp::STEP10_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(60.556, 72.999)), module, AngryArp::STEP11_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(80.617, 72.999)), module, AngryArp::STEP12_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(20.433, 92.798)), module, AngryArp::STEP13_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(40.494, 92.798)), module, AngryArp::STEP14_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(60.556, 92.798)), module, AngryArp::STEP15_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(80.617, 92.798)), module, AngryArp::STEP16_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(63.898, 119.419)), module, AngryArp::STEPS_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(11.526, 119.256)), module, AngryArp::GATE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(29.078, 119.256)), module, AngryArp::VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(46.284, 119.419)), module, AngryArp::CLOCK_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(76.942, 119.419)), module, AngryArp::GATEOUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(87.817, 119.581)), module, AngryArp::OUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(20.433, 24.148)), module, AngryArp::LIGHT1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(40.494, 24.148)), module, AngryArp::LIGHT2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(60.556, 24.148)), module, AngryArp::LIGHT3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(80.617, 24.148)), module, AngryArp::LIGHT4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(20.433, 43.947)), module, AngryArp::LIGHT5_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(40.494, 43.947)), module, AngryArp::LIGHT6_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(60.556, 43.947)), module, AngryArp::LIGHT7_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(80.617, 43.947)), module, AngryArp::LIGHT8_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(20.433, 63.747)), module, AngryArp::LIGHT9_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(40.494, 63.747)), module, AngryArp::LIGHT10_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(60.556, 63.747)), module, AngryArp::LIGHT11_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(80.617, 63.747)), module, AngryArp::LIGHT12_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(20.433, 83.546)), module, AngryArp::LIGHT13_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(40.494, 83.546)), module, AngryArp::LIGHT14_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(60.556, 83.546)), module, AngryArp::LIGHT15_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(80.617, 83.546)), module, AngryArp::LIGHT16_LIGHT));
	}
};


Model* modelAngryArp = createModel<AngryArp, AngryArpWidget>("AngryArp");