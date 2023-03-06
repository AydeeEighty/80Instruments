#include "plugin.hpp"


struct Maniac3 : Module {
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
		OCTAVE1_PARAM,
		OCTAVE2_PARAM,
		OCTAVE3_PARAM,
		OCTAVE4_PARAM,
		OCTAVE5_PARAM,
		OCTAVE6_PARAM,
		OCTAVE7_PARAM,
		OCTAVE8_PARAM,
		OCTAVE9_PARAM,
		OCTAVE10_PARAM,
		OCTAVE11_PARAM,
		OCTAVE12_PARAM,
		OCTAVE13_PARAM,
		OCTAVE14_PARAM,
		OCTAVE15_PARAM,
		OCTAVE16_PARAM,
		DRIFT1_PARAM,
		DRIFT2_PARAM,
		DRIFT3_PARAM,
		DRIFT4_PARAM,
		DRIFT5_PARAM,
		DRIFT6_PARAM,
		DRIFT7_PARAM,
		DRIFT8_PARAM,
		DRIFT9_PARAM,
		DRIFT10_PARAM,
		DRIFT11_PARAM,
		DRIFT12_PARAM,
		DRIFT13_PARAM,
		DRIFT14_PARAM,
		DRIFT15_PARAM,
		DRIFT16_PARAM,
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
	float drift=0.0;
	int driftInt=0;
	float semitoneDrift=0.0;
    int randomDrift=0;

struct paramnote : ParamQuantity {
	int semitone;
	std::string getDisplayValueString() override {
	if (getValue()==0){
		return "C";
	}
	else if (getValue()>0 && getValue()<=1.000){
		return "C#";
	}
	else if (getValue()>1 && getValue()<=2.000){
		return "D";
	}
	else if (getValue()>2 && getValue()<=3.000){
		return "D#";
	}
	else if (getValue()>3 && getValue()<=4.000){
		return "E";
	}
	else if (getValue()>4 && getValue()<=5.000){
		return "F";
	}
	else if (getValue()>5 && getValue()<=6.000){
		return "F#";
	}
	else if (getValue()>6 && getValue()<=7.000){
		return "G";
	}
	else if (getValue()>7 && getValue()<=8.000){
		return "G#";
	}
	else if (getValue()>8 && getValue()<=9.000){
		return "A";
	}
	else if (getValue()>9 && getValue()<=10.000){
		return "A#";
	}
	else if (getValue()>10 && getValue()<=11.000){
		return "B";
	}
	else if (getValue()>11 && getValue()<=12.000){
		return "C";
	}
	else {
		return "N";
	}
	}
};


	Maniac3() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configSwitch(SWITCH_PARAM, 0.0f, 1.0f, 1.0f, "Mode", {"Seq.", "Random"});
		for (int i=0; i< Maniac3::LIGHTS_LEN; i++){
			configParam<paramnote>(STEP1_PARAM+i,0.0f,12.0f,0.0f,"Note");
			paramQuantities[STEP1_PARAM+i]->snapEnabled = true;
			configParam(DRIFT1_PARAM+i,0.0f,12.0f,0.0f,"Drift");
			paramQuantities[DRIFT1_PARAM+i]->snapEnabled = true;
			configParam(OCTAVE1_PARAM+i,-3.0f,3.0f,0.0f,"Octave Change");
			paramQuantities[OCTAVE1_PARAM+i]->snapEnabled=true;
			
		};
		configParam(STEPS_PARAM, 1.0f, 16.0f, 16.0f, "Steps");
		paramQuantities[STEPS_PARAM]->snapEnabled = true;
	}

	void process(const ProcessArgs& args) override {

	int pulses = (int)fmaxf(params[STEPS_PARAM].getValue(), 1.0f);
	float octave = params[OCTAVE1_PARAM+stepNr].getValue();
	float octaveChange = octave*12*0.08333;
	if (params[SWITCH_PARAM].getValue()==0){
	if (edgeDetector.process(inputs[IN_INPUT].getVoltage())) {
			stepNr=(stepNr+1) % pulses;
			drift=(params[DRIFT1_PARAM+stepNr].getValue());
			driftInt=static_cast<int>(drift);
			if (driftInt!=0){
			randomDrift = std::rand() % (driftInt*2) - driftInt;
			semitoneDrift = static_cast<float>(randomDrift) * 0.08333;	
			}
			else {
				semitoneDrift=0.0;
			}
			
	}
	}
	else {
	if (edgeDetector.process(inputs[IN_INPUT].getVoltage())){
			stepNr= rand() % pulses;
			drift=(params[DRIFT1_PARAM+stepNr].getValue());
			driftInt=static_cast<int>(drift);
			if (driftInt!=0)
			{
			randomDrift = std::rand() % (driftInt*2) - driftInt;
			semitoneDrift = static_cast<float>(randomDrift) * 0.08333;
			}
			else {
				semitoneDrift=0.0;
			}	
	}
	}
	if (inputs[RESET_INPUT].isConnected()){
		if (inputs[RESET_INPUT].getVoltage() >0){
			stepNr=0;
		}
	}
	for (int l=0; l<LIGHTS_LEN; l++){
		lights[l].setSmoothBrightness (l==stepNr, 5e-6f);
	}
	

	outputs [OUT_OUTPUT].setVoltage ((params[STEP1_PARAM+stepNr].getValue()/12)+semitoneDrift+octaveChange);






	}
};


struct Maniac3Widget : ModuleWidget {
	Maniac3Widget(Maniac3* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Maniac3.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.847, 39.522)), module, Maniac3::STEP1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.223, 39.522)), module, Maniac3::STEP2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(38.599, 39.522)), module, Maniac3::STEP3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(53.975, 39.522)), module, Maniac3::STEP4_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(69.352, 39.522)), module, Maniac3::STEP5_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(84.728, 39.522)), module, Maniac3::STEP6_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(100.104, 39.522)), module, Maniac3::STEP7_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(115.48, 39.522)), module, Maniac3::STEP8_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(7.847, 49.607)), module, Maniac3::OCTAVE1_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(23.223, 49.607)), module, Maniac3::OCTAVE2_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(38.599, 49.607)), module, Maniac3::OCTAVE3_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(53.975, 49.607)), module, Maniac3::OCTAVE4_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(69.352, 49.607)), module, Maniac3::OCTAVE5_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(84.728, 49.607)), module, Maniac3::OCTAVE6_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(100.104, 49.607)), module, Maniac3::OCTAVE7_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(115.48, 49.607)), module, Maniac3::OCTAVE8_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(7.847, 59.691)), module, Maniac3::DRIFT1_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(23.223, 59.691)), module, Maniac3::DRIFT2_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(38.599, 59.691)), module, Maniac3::DRIFT3_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(53.975, 59.691)), module, Maniac3::DRIFT4_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(69.352, 59.691)), module, Maniac3::DRIFT5_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(84.728, 59.691)), module, Maniac3::DRIFT6_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(100.104, 59.691)), module, Maniac3::DRIFT7_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(115.48, 59.691)), module, Maniac3::DRIFT8_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.847, 82.399)), module, Maniac3::STEP9_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(23.223, 82.399)), module, Maniac3::STEP10_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(38.599, 82.399)), module, Maniac3::STEP11_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(53.975, 82.399)), module, Maniac3::STEP12_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(69.352, 82.399)), module, Maniac3::STEP13_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(84.728, 82.399)), module, Maniac3::STEP14_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(100.104, 82.399)), module, Maniac3::STEP15_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(115.48, 82.399)), module, Maniac3::STEP16_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(7.847, 92.483)), module, Maniac3::OCTAVE9_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(23.223, 92.483)), module, Maniac3::OCTAVE10_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(38.599, 92.483)), module, Maniac3::OCTAVE11_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(53.975, 92.483)), module, Maniac3::OCTAVE12_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(69.352, 92.483)), module, Maniac3::OCTAVE13_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(84.728, 92.483)), module, Maniac3::OCTAVE14_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(100.104, 92.483)), module, Maniac3::OCTAVE15_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(115.48, 92.483)), module, Maniac3::OCTAVE16_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(7.847, 102.568)), module, Maniac3::DRIFT9_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(23.223, 102.568)), module, Maniac3::DRIFT10_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(38.599, 102.568)), module, Maniac3::DRIFT11_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(53.975, 102.568)), module, Maniac3::DRIFT12_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(69.352, 102.568)), module, Maniac3::DRIFT13_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(84.728, 102.568)), module, Maniac3::DRIFT14_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(100.104, 102.568)), module, Maniac3::DRIFT15_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(115.48, 102.568)), module, Maniac3::DRIFT16_PARAM));
		addParam(createParamCentered<CKSS>(mm2px(Vec(60.836, 121.924)), module, Maniac3::SWITCH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(98.185, 121.924)), module, Maniac3::STEPS_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(24.575, 121.67)), module, Maniac3::IN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.508, 121.67)), module, Maniac3::RESET_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(79.265, 121.67)), module, Maniac3::OUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(7.847, 31.711)), module, Maniac3::LIGHT1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(23.223, 31.711)), module, Maniac3::LIGHT2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(38.599, 31.711)), module, Maniac3::LIGHT3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(53.975, 31.711)), module, Maniac3::LIGHT4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(69.352, 31.711)), module, Maniac3::LIGHT5_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(84.728, 31.711)), module, Maniac3::LIGHT6_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(100.104, 31.711)), module, Maniac3::LIGHT7_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(115.48, 31.711)), module, Maniac3::LIGHT8_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(7.847, 74.263)), module, Maniac3::LIGHT9_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(23.223, 74.263)), module, Maniac3::LIGHT10_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(38.599, 74.263)), module, Maniac3::LIGHT11_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(53.975, 74.263)), module, Maniac3::LIGHT12_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(69.352, 74.263)), module, Maniac3::LIGHT13_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(84.728, 74.263)), module, Maniac3::LIGHT14_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(100.104, 74.263)), module, Maniac3::LIGHT15_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(115.48, 74.263)), module, Maniac3::LIGHT16_LIGHT));
	}
};


Model* modelManiac3 = createModel<Maniac3, Maniac3Widget>("Maniac3");