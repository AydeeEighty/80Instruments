#include "plugin.hpp"

float TRIG=0.25;

struct Klok2 : Module {
	enum ParamId {
		RESET_PARAM,
		RUN_PARAM,
		BPM_PARAM,
		MAINP_PARAM,
		MULT2P_PARAM,
		MULT4P_PARAM,
		MULT8P_PARAM,
		MULT16P_PARAM,
		DIV2P_PARAM,
		DIV4P_PARAM,
		DIV8P_PARAM,
		DIV16P_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		RESET_OUTPUT,
		MAIN_OUTPUT,
		MAINP_OUTPUT,
		MULT2_OUTPUT,
		MULT2P_OUTPUT,
		MULT4_OUTPUT,
		MULT4P_OUTPUT,
		MULT8_OUTPUT,
		MULT8P_OUTPUT,
		MULT16_OUTPUT,
		MULT16P_OUTPUT,
		DIV2_OUTPUT,
		DIV2P_OUTPUT,
		DIV4_OUTPUT,
		DIV4P_OUTPUT,
		DIV8_OUTPUT,
		DIV8P_OUTPUT,
		DIV16_OUTPUT,
		DIV16P_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		RUN_LIGHT,
		RESET_LIGHT,
		LIGHTS_LEN
	};

	dsp::PulseGenerator pgen;
	dsp::PulseGenerator pgenp;
	dsp::PulseGenerator pgenDiv2;
	dsp::PulseGenerator pgenDiv2p;
	dsp::PulseGenerator pgenDiv4;
	dsp::PulseGenerator pgenDiv4p;
	dsp::PulseGenerator pgenDiv8;
	dsp::PulseGenerator pgenDiv8p;
	dsp::PulseGenerator pgenDiv16;
	dsp::PulseGenerator pgenDiv16p;
	dsp::PulseGenerator pgenMult2;
	dsp::PulseGenerator pgenMult2p;
	dsp::PulseGenerator pgenMult4;
	dsp::PulseGenerator pgenMult4p;
	dsp::PulseGenerator pgenMult8;
	dsp::PulseGenerator pgenMult8p;
	dsp::PulseGenerator pgenMult16;
	dsp::PulseGenerator pgenMult16p;
	dsp::BooleanTrigger runButtonTrigger;
	dsp::BooleanTrigger resetButtonTrigger;
	dsp::PulseGenerator resetPulse;
	bool runit=false;
	bool initialHit=true;

	float counter,counterDiv2,counterDiv4,counterDiv8,counterDiv16; 
	float counterMult2,counterMult4,counterMult8,counterMult16;
	float period,periodDiv2,periodDiv4,periodDiv8,periodDiv16;
	float periodMult2,periodMult4,periodMult8,periodMult16;
	float out,outDiv2,outDiv4,outDiv8,outDiv16;
	float outMult2,outMult4,outMult8,outMult16;
	float randProb;
	float outp,outDiv2p,outDiv4p,outDiv8p,outDiv16p;
	float outMult2p,outMult4p,outMult8p,outMult16p;



	Klok2() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configButton(RUN_PARAM, "Run");
		configButton(RESET_PARAM,"Reset");
		configParam(BPM_PARAM, 30.0, 240.0, 120.0, "Tempo","BPM");
		configParam(MAINP_PARAM, 0.f, 1.f, 0.5f, "1x Prob");
		configParam(MULT2P_PARAM, 0.f, 1.f, 0.5f, "");
		configParam(MULT4P_PARAM, 0.f, 1.f, 0.5f, "");
		configParam(MULT8P_PARAM, 0.f, 1.f, 0.5f, "");
		configParam(MULT16P_PARAM, 0.f, 1.f, 0.5f, "");
		configParam(DIV2P_PARAM, 0.f, 1.f, 0.5f, "");
		configParam(DIV4P_PARAM, 0.f, 1.f, 0.5f, "");
		configParam(DIV8P_PARAM, 0.f, 1.f, 0.5f, "");
		configParam(DIV16P_PARAM, 0.f, 1.f, 0.5f, "");
		configOutput(RESET_OUTPUT, "");
		configOutput(MAIN_OUTPUT, "");
		configOutput(MAINP_OUTPUT, "");
		configOutput(MULT2_OUTPUT, "");
		configOutput(MULT2P_OUTPUT, "");
		configOutput(MULT4_OUTPUT, "");
		configOutput(MULT4P_OUTPUT, "");
		configOutput(MULT8_OUTPUT, "");
		configOutput(MULT8P_OUTPUT, "");
		configOutput(MULT16_OUTPUT, "");
		configOutput(MULT16P_OUTPUT, "");
		configOutput(DIV2_OUTPUT, "");
		configOutput(DIV2P_OUTPUT, "");
		configOutput(DIV4_OUTPUT, "");
		configOutput(DIV4P_OUTPUT, "");
		configOutput(DIV8_OUTPUT, "");
		configOutput(DIV8P_OUTPUT, "");
		configOutput(DIV16_OUTPUT, "");
		configOutput(DIV16P_OUTPUT, "");
		counter = 0.f;
		period = 0.f;
		counterDiv2= 0.f;
		periodDiv2 =0.f;
		counterDiv4= 0.f;
		periodDiv4 =0.f;
		counterDiv8= 0.f;
		periodDiv8 =0.f;
		counterDiv16= 0.f;
		periodDiv16 =0.f;
		counterMult2 = 0.f;
		periodMult2 =0.f;
		counterMult4 = 0.f;
		periodMult4 =0.f;
		counterMult8 = 0.f;
		periodMult8 =-0.f;
		counterMult16 = 0.f; 
		periodMult16 =0.f;
		initialHit=true;
	}

	void process(const ProcessArgs& args) override {

		if (runButtonTrigger.process(params[RUN_PARAM].getValue())) {
			runit^= true;
			}
		if (resetButtonTrigger.process(params[RESET_PARAM].getValue())){
			counter = 0.f;
			period = 0.f;
			counterDiv2= 0.f;
			periodDiv2 =0.f;
			counterDiv4= 0.f;
			periodDiv4 =0.f;
			counterDiv8= 0.f;
			periodDiv8 =0.f;
			counterDiv16= 0.f;
			periodDiv16 =0.f;
			counterMult2 = 0.f;
			periodMult2 =0.f;
			counterMult4 = 0.f;
			periodMult4 =0.f;
			counterMult8 = 0.f;
			periodMult8 =0.f;
			counterMult16 = 0.f; 
			periodMult16 =0.f;
			resetPulse.trigger(1e-3f);
			initialHit=true;
		}
	bool resetGate = resetPulse.process(args.sampleTime);
	
	
	if (runit){
		if(initialHit){
			pgen.trigger(TRIG);
			pgenDiv2.trigger(TRIG*2);
			pgenDiv4.trigger(TRIG*4);
			pgenDiv8.trigger(TRIG*8);
			pgenDiv16.trigger(TRIG*16);
			pgenMult2.trigger(TRIG/2);
			pgenMult4.trigger(TRIG/4);
			pgenMult8.trigger(TRIG/8);
			pgenMult16.trigger(TRIG/16);
			out = pgen.process(args.sampleTime);
			outDiv2 = pgenDiv2.process(args.sampleTime);
			outDiv4 = pgenDiv4.process(args.sampleTime);
			outDiv8 = pgenDiv8.process(args.sampleTime);
			outDiv16 = pgenDiv16.process(args.sampleTime);
			outMult2 = pgenMult2.process(args.sampleTime);
			outMult4 = pgenMult4.process(args.sampleTime);
			outMult8 = pgenMult8.process(args.sampleTime);
			outMult16 = pgenMult16.process(args.sampleTime);
			outputs[MAIN_OUTPUT].setVoltage(10.f*out);
			outputs[DIV2_OUTPUT].setVoltage(10.f*outDiv2);
			outputs[DIV4_OUTPUT].setVoltage(10.f*outDiv4);
			outputs[DIV8_OUTPUT].setVoltage(10.f*outDiv8);
			outputs[DIV16_OUTPUT].setVoltage(10.f*outDiv16);
			outputs[MULT2_OUTPUT].setVoltage(10.f*outMult2);
			outputs[MULT4_OUTPUT].setVoltage(10.f*outMult4);
			outputs[MULT8_OUTPUT].setVoltage(10.f*outMult8);
			outputs[MULT16_OUTPUT].setVoltage(10.f*outMult16);
			initialHit=false;
		}	
		
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
			pgen.trigger(TRIG);
			counter -= period;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[MAINP_PARAM].getValue()){
			pgenp.trigger(TRIG);	
			}

		}

		if (counterDiv2>periodDiv2){
			pgenDiv2.trigger(TRIG*2);
			counterDiv2-= periodDiv2;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[DIV2P_PARAM].getValue()){
			pgenDiv2p.trigger(TRIG*2);	
			}
		}

		if (counterDiv4>periodDiv4){
			pgenDiv4.trigger(TRIG*4);
			counterDiv4-= periodDiv4;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[DIV4P_PARAM].getValue()){
			pgenDiv4p.trigger(TRIG*4);	
			}
		}

		if (counterDiv8>periodDiv8){
			pgenDiv8.trigger(TRIG*8);
			counterDiv8-= periodDiv8;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[DIV8P_PARAM].getValue()){
			pgenDiv8p.trigger(TRIG*8);	
			}
		}

		if (counterDiv16>periodDiv16){
			pgenDiv16.trigger(TRIG*16);
			counterDiv16-= periodDiv16;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[DIV16P_PARAM].getValue()){
			pgenDiv16p.trigger(TRIG*16);	
			}
		}

		if (counterMult2>periodMult2){
			pgenMult2.trigger(TRIG/2);
			counterMult2-= periodMult2;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[MULT2P_PARAM].getValue()){
			pgenMult2p.trigger(TRIG/2);	
			}
		}
		if (counterMult4>periodMult4){
			pgenMult4.trigger(TRIG/4);
			counterMult4-= periodMult4;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[MULT4P_PARAM].getValue()){
			pgenMult4p.trigger(TRIG/4);	
			}
		}

		if (counterMult8>periodMult8){
			pgenMult8.trigger(TRIG/8);
			counterMult8-= periodMult8;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[MULT8P_PARAM].getValue()){
			pgenMult8p.trigger(TRIG/8);	
			}
		}

		if (counterMult16>periodMult16){
			pgenMult16.trigger(TRIG/16);
			counterMult16-= periodMult16;
			randProb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));	
			if (randProb<=params[MULT16P_PARAM].getValue()){
			pgenMult16p.trigger(TRIG/16);	
			}
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
		
		out = pgen.process(args.sampleTime);
		outp = pgenp.process(args.sampleTime);
		outDiv2 = pgenDiv2.process(args.sampleTime);
		outDiv2p = pgenDiv2p.process(args.sampleTime);
		outDiv4 = pgenDiv4.process(args.sampleTime);
		outDiv4p = pgenDiv4p.process(args.sampleTime);
		outDiv8 = pgenDiv8.process(args.sampleTime);
		outDiv8p = pgenDiv8p.process(args.sampleTime);
		outDiv16 = pgenDiv16.process(args.sampleTime);
		outDiv16p = pgenDiv16p.process(args.sampleTime);
		outMult2 = pgenMult2.process(args.sampleTime);
		outMult2p = pgenMult2p.process(args.sampleTime);
		outMult4 = pgenMult4.process(args.sampleTime);
		outMult4p = pgenMult4p.process(args.sampleTime);
		outMult8 = pgenMult8.process(args.sampleTime);
		outMult8p = pgenMult8p.process(args.sampleTime);
		outMult16 = pgenMult16.process(args.sampleTime);
		outMult16p = pgenMult16p.process(args.sampleTime);
		outputs[MAIN_OUTPUT].setVoltage(10.f*out);
		outputs[DIV2_OUTPUT].setVoltage(10.f*outDiv2);
		outputs[DIV4_OUTPUT].setVoltage(10.f*outDiv4);
		outputs[DIV8_OUTPUT].setVoltage(10.f*outDiv8);
		outputs[DIV16_OUTPUT].setVoltage(10.f*outDiv16);
		outputs[MULT2_OUTPUT].setVoltage(10.f*outMult2);
		outputs[MULT4_OUTPUT].setVoltage(10.f*outMult4);
		outputs[MULT8_OUTPUT].setVoltage(10.f*outMult8);
		outputs[MULT16_OUTPUT].setVoltage(10.f*outMult16);
		outputs[MAINP_OUTPUT].setVoltage(10.f*outp);
		outputs[MULT2P_OUTPUT].setVoltage(10.f*outMult2p);
		outputs[MULT4P_OUTPUT].setVoltage(10.f*outMult4p);
		outputs[MULT8P_OUTPUT].setVoltage(10.f*outMult8p);
		outputs[MULT16P_OUTPUT].setVoltage(10.f*outMult16p);
		outputs[DIV2P_OUTPUT].setVoltage(10.f*outDiv2p);
		outputs[DIV4P_OUTPUT].setVoltage(10.f*outDiv4p);
		outputs[DIV8P_OUTPUT].setVoltage(10.f*outDiv8p);
		outputs[DIV16P_OUTPUT].setVoltage(10.f*outDiv16p);
		
		
		
		
		lights[RUN_LIGHT].setBrightness(1.0);
	}
	else {
		
		lights[RUN_LIGHT].setBrightness(0);
	}
	outputs[RESET_OUTPUT].setVoltage(resetGate ? 10.f : 0.f);
	lights[RESET_LIGHT].setSmoothBrightness(resetGate, args.sampleTime);


	}
};


struct Klok2Widget : ModuleWidget {
	Klok2Widget(Klok2* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Klok2.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createLightParamCentered<VCVLightButton<MediumSimpleLight<WhiteLight>>>(mm2px(Vec(24.997, 27.503)), module, Klok2::RESET_PARAM, Klok2::RESET_LIGHT));
		addParam(createLightParamCentered<VCVLightButton<MediumSimpleLight<WhiteLight>>>(mm2px(Vec(35.963, 27.503)), module, Klok2::RUN_PARAM, Klok2::RUN_LIGHT));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(46.929, 27.503)), module, Klok2::BPM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(40.123, 42.995)), module, Klok2::MAINP_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(24.997, 58.139)), module, Klok2::MULT2P_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(55.25, 58.139)), module, Klok2::MULT4P_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(24.997, 71.925)), module, Klok2::MULT8P_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(55.25, 71.925)), module, Klok2::MULT16P_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(24.997, 85.711)), module, Klok2::DIV2P_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(55.25, 85.711)), module, Klok2::DIV4P_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(24.997, 99.497)), module, Klok2::DIV8P_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(55.25, 99.497)), module, Klok2::DIV16P_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.031, 27.503)), module, Klok2::RESET_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.837, 42.995)), module, Klok2::MAIN_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(29.157, 42.995)), module, Klok2::MAINP_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.71, 58.139)), module, Klok2::MULT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.031, 58.139)), module, Klok2::MULT2P_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.963, 58.139)), module, Klok2::MULT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(44.284, 58.139)), module, Klok2::MULT4P_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.71, 71.925)), module, Klok2::MULT8_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.031, 71.925)), module, Klok2::MULT8P_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.963, 71.925)), module, Klok2::MULT16_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(44.284, 71.925)), module, Klok2::MULT16P_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.71, 85.711)), module, Klok2::DIV2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.031, 85.711)), module, Klok2::DIV2P_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.963, 85.711)), module, Klok2::DIV4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(44.284, 85.711)), module, Klok2::DIV4P_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.71, 99.497)), module, Klok2::DIV8_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.031, 99.497)), module, Klok2::DIV8P_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.963, 99.497)), module, Klok2::DIV16_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(44.284, 99.497)), module, Klok2::DIV16P_OUTPUT));
	}
};


Model* modelKlok2 = createModel<Klok2, Klok2Widget>("Klok2");