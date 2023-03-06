#include "plugin.hpp"
float TRIG_TIME=1e-3f;

struct Klok : Module {
	enum ParamId {
		RESET_PARAM,
		RUN_PARAM,
		BPM_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		MAIN_OUTPUT,
		RESET_OUTPUT,
		_2X_OUTPUT,
		_4X_OUTPUT,
		_8X_OUTPUT,
		_16X_OUTPUT,
		DIV2_OUTPUT,
		DIV4_OUTPUT,
		DIV8_OUTPUT,
		DIV16_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		RUN_LIGHT,
		RESET_LIGHT,
		LIGHTS_LEN
	};

	dsp::PulseGenerator pgen;
	dsp::PulseGenerator pgenDiv2;
	dsp::PulseGenerator pgenDiv4;
	dsp::PulseGenerator pgenDiv8;
	dsp::PulseGenerator pgenDiv16;
	dsp::PulseGenerator pgenMult2;
	dsp::PulseGenerator pgenMult4;
	dsp::PulseGenerator pgenMult8;
	dsp::PulseGenerator pgenMult16;
	dsp::BooleanTrigger runButtonTrigger;
	dsp::BooleanTrigger resetButtonTrigger;
	dsp::PulseGenerator resetPulse;
	bool runit=false;
	
	float counter,counterDiv2,counterDiv4,counterDiv8,counterDiv16; 
	float counterMult2,counterMult4,counterMult8,counterMult16;
	float period,periodDiv2,periodDiv4,periodDiv8,periodDiv16;
	float periodMult2,periodMult4,periodMult8,periodMult16;


	Klok() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configButton(RUN_PARAM, "Run");
		configButton(RESET_PARAM,"Reset");
		configParam(BPM_PARAM, 30.0, 240.0, 120.0, "Tempo","BPM");
		configOutput(MAIN_OUTPUT, "");
		configOutput(_2X_OUTPUT, "");
		configOutput(_4X_OUTPUT, "");
		configOutput(_8X_OUTPUT, "");
		configOutput(_16X_OUTPUT, "");
		configOutput(DIV2_OUTPUT, "");
		configOutput(DIV4_OUTPUT, "");
		configOutput(DIV8_OUTPUT, "");
		configOutput(DIV16_OUTPUT, "");
		counter = period = 0.f;
		counterDiv2= periodDiv2 =0.f;
		counterDiv4= periodDiv4 =0.f;
		counterDiv8= periodDiv8 =0.f;
		counterDiv16= periodDiv16 =0.f;
		counterMult2 = periodMult2 =0.f;
		counterMult4 = periodMult4 =0.f;
		counterMult8 = periodMult8 =0.f;
		counterMult16 = periodMult16 =0.f;
	}

	void process(const ProcessArgs& args) override {

		if (runButtonTrigger.process(params[RUN_PARAM].getValue())) {
			runit^= true;
			
		}
		if (resetButtonTrigger.process(params[RESET_PARAM].getValue())){
			counter = period = 0.f;
			counterDiv2= periodDiv2 =0.f;
			counterDiv4= periodDiv4 =0.f;
			counterDiv8= periodDiv8 =0.f;
			counterDiv16= periodDiv16 =0.f;
			counterMult2 = periodMult2 =0.f;
			counterMult4 = periodMult4 =0.f;
			counterMult8 = periodMult8 =0.f;
			counterMult16 = periodMult16 =0.f;
			resetPulse.trigger(TRIG_TIME);
			
		}
	bool resetGate = resetPulse.process(args.sampleTime);
	
	
	if (runit){
		float out = pgen.process(args.sampleTime);
		float outDiv2 = pgenDiv2.process(args.sampleTime);
		float outDiv4 = pgenDiv4.process(args.sampleTime);
		float outDiv8 = pgenDiv8.process(args.sampleTime);
		float outDiv16 = pgenDiv16.process(args.sampleTime);
		float outMult2 = pgenMult2.process(args.sampleTime);
		float outMult4 = pgenMult4.process(args.sampleTime);
		float outMult8 = pgenMult8.process(args.sampleTime);
		float outMult16 = pgenMult16.process(args.sampleTime);
		outputs[MAIN_OUTPUT].setVoltage(10.f*out);
		outputs[DIV2_OUTPUT].setVoltage(10.f*outDiv2);
		outputs[DIV4_OUTPUT].setVoltage(10.f*outDiv4);
		outputs[DIV8_OUTPUT].setVoltage(10.f*outDiv8);
		outputs[DIV16_OUTPUT].setVoltage(10.f*outDiv16);
		outputs[_2X_OUTPUT].setVoltage(10.f*outMult2);
		outputs[_4X_OUTPUT].setVoltage(10.f*outMult4);
		outputs[_8X_OUTPUT].setVoltage(10.f*outMult8);
		outputs[_16X_OUTPUT].setVoltage(10.f*outMult16);
		
		float BPM = params[BPM_PARAM].getValue();
		period = 60.f * args.sampleRate/BPM;
		periodDiv2 = (60.f * args.sampleRate/BPM)*2;
		periodDiv4 = (60.f * args.sampleRate/BPM)*4;
		periodDiv8 = (60.f * args.sampleRate/BPM)*8;
		periodDiv16 = (60.f * args.sampleRate/BPM)*16;
		periodMult2 = (60.f * args.sampleRate/BPM)/2;
		periodMult4 = (60.f * args.sampleRate/BPM)/4;
		periodMult8 = (60.f * args.sampleRate/BPM)/8;
		periodMult16 = (60.f * args.sampleRate/BPM)/16;


		if (counter>period){
			pgen.trigger(TRIG_TIME);
			counter -= period;	
		}

		if (counterDiv2>periodDiv2){
			pgenDiv2.trigger(TRIG_TIME);
			counterDiv2-= periodDiv2;
		}

		if (counterDiv4>periodDiv4){
			pgenDiv4.trigger(TRIG_TIME);
			counterDiv4-= periodDiv4;
		}

		if (counterDiv8>periodDiv8){
			pgenDiv8.trigger(TRIG_TIME);
			counterDiv8-= periodDiv8;
		}

		if (counterDiv16>periodDiv16){
			pgenDiv16.trigger(TRIG_TIME);
			counterDiv16-= periodDiv16;
		}

		if (counterMult2>periodMult2){
			pgenMult2.trigger(TRIG_TIME);
			counterMult2-= periodMult2;
		}
		if (counterMult4>periodMult4){
			pgenMult4.trigger(TRIG_TIME);
			counterMult4-= periodMult4;
		}

		if (counterMult8>periodMult8){
			pgenMult8.trigger(TRIG_TIME);
			counterMult8-= periodMult8;
		}

		if (counterMult16>periodMult16){
			pgenMult16.trigger(TRIG_TIME);
			counterMult16-= periodMult16;
		}
		counter++;
		counterDiv2++;
		counterDiv4++;
		counterDiv8++;
		counterDiv16++;
		counterMult2++;
		counterMult4++;
		counterMult8++;
		counterMult16++;


		
		lights[RUN_LIGHT].setBrightness(1.0);
	}
	else {
		
		lights[RUN_LIGHT].setBrightness(0);
	}
	outputs[RESET_OUTPUT].setVoltage(resetGate ? 10.f : 0.f);
	lights[RESET_LIGHT].setSmoothBrightness(resetGate, args.sampleTime);
	}
};


struct KlokWidget : ModuleWidget {
	KlokWidget(Klok* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Klok.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createLightParamCentered<VCVLightButton<MediumSimpleLight<WhiteLight>>>(mm2px(Vec(6.324, 26.452)), module, Klok::RESET_PARAM, Klok::RESET_LIGHT));
		addParam(createLightParamCentered<VCVLightButton<MediumSimpleLight<WhiteLight>>>(mm2px(Vec(19.076, 26.346)), module, Klok::RUN_PARAM, Klok::RUN_LIGHT));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.7, 42.072)), module, Klok::BPM_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.237, 58.333)), module, Klok::MAIN_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(18.119, 58.439)), module, Klok::RESET_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.149, 72.057)), module, Klok::_2X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(17.888, 72.057)), module, Klok::_4X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.149, 85.621)), module, Klok::_8X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(17.888, 85.621)), module, Klok::_16X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.202, 104.0)), module, Klok::DIV2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(17.942, 104.0)), module, Klok::DIV4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.096, 116.927)), module, Klok::DIV8_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(17.835, 116.927)), module, Klok::DIV16_OUTPUT));
	}
};


Model* modelKlok = createModel<Klok, KlokWidget>("Klok");