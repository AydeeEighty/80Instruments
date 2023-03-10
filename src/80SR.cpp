#include "plugin.hpp"

float EPSILON=1e-9f;
bool isAtk, isRunning;
float env;
float sus;
float Astep,Dstep,Rstep;

struct _80SR : Module {
	enum ParamId {
		ATTACK_PARAM,
		DECAY_PARAM,
		SUSTAIN_PARAM,
		RELEASE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		ATTACK_INPUT,
		DECAY_INPUT,
		SUSTAIN_INPUT,
		RELEASE_INPUT,
		GATE_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		ENV_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	dsp::SchmittTrigger gateDetect;

	_80SR() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(ATTACK_PARAM, 0.f,5.f, 0.5f, "Attack"," s");
		configParam(DECAY_PARAM, 0.f, 5.f, 0.5f, "Decay"," s");
		configParam(SUSTAIN_PARAM, 0.f, 1.f, 0.5f, "Sustain");
		configParam(RELEASE_PARAM, 0.f, 5.f, 0.5f, "Release"," s");
		configInput(ATTACK_INPUT, "Attack CV");
		configInput(DECAY_INPUT, "Decay CV");
		configInput(SUSTAIN_INPUT, "Sustain CV");
		configInput(RELEASE_INPUT, "Release CV");
		configInput(GATE_INPUT, "Gate in");
		configOutput(ENV_OUTPUT, "Envelope out");
	}

	void process(const ProcessArgs& args) override {
	if (inputs[SUSTAIN_INPUT].isConnected()){
		sus=inputs[SUSTAIN_INPUT].getVoltage();
	}
	else{
	sus = params[SUSTAIN_PARAM].getValue();
	}
	if (inputs[ATTACK_INPUT].isConnected()){
	Astep = 1.f / (EPSILON + args.sampleRate * inputs[ATTACK_INPUT].getVoltage());
	}
	else{
	Astep = 1.f / (EPSILON + args.sampleRate * params[ATTACK_PARAM].getValue());
	}
	if (inputs[DECAY_INPUT].isConnected()){
	Dstep = (sus - 1.0) / (EPSILON + args.sampleRate * inputs[DECAY_INPUT].getVoltage());
	}
	else{
	Dstep = (sus - 1.0) / (EPSILON + args.sampleRate * params[DECAY_PARAM].getValue());
	}
	if (inputs[RELEASE_INPUT].isConnected()){
	Rstep = -(sus + EPSILON) / (EPSILON + args.sampleRate * inputs[RELEASE_INPUT].getVoltage());
	}
	else{
	Rstep = -(sus + EPSILON) / (EPSILON + args.sampleRate * params[RELEASE_PARAM].getValue());
	}
	Astep = clamp(Astep, EPSILON, 0.5);

	Dstep = std::max(Dstep, -0.5f);
	Rstep = std::max(Rstep, -1.f);

	bool gate = inputs[GATE_INPUT].getVoltage() >= 1.0;
	if (gateDetect.process(gate)) {
		isAtk = true;
		isRunning = true;
		
	}


	if (isRunning) {
		if (gate) {
			// ATK
			if (isAtk) {
				env += Astep;
				if (env >= 1.0)
					isAtk = false;
			}
			else {
				// DEC
				if (env <= sus + 0.001) {
					env = sus;
				}
				else {
					env += Dstep;
				}
			}
		} else {
			// REL
			env += Rstep;
			if (env <= Rstep)
				isRunning = false;
		}
	}
	 else {
		env = 0.0;
	}
	if (env>1){
		env=1.0;
	}
	if (env<0)
	{
		env=0.0;
	}
	if (outputs[ENV_OUTPUT].isConnected()) {
		outputs[ENV_OUTPUT].setVoltage(10.f * env);
	}
	}
};


struct _80SRWidget : ModuleWidget {
	_80SRWidget(_80SR* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/80SR.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(19.564, 27.576)), module, _80SR::ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(19.564, 44.327)), module, _80SR::DECAY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(19.564, 61.078)), module, _80SR::SUSTAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(19.564, 77.828)), module, _80SR::RELEASE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.292, 27.576)), module, _80SR::ATTACK_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.292, 44.327)), module, _80SR::DECAY_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.292, 61.078)), module, _80SR::SUSTAIN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.292, 77.828)), module, _80SR::RELEASE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.683, 111.237)), module, _80SR::GATE_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(18.279, 111.237)), module, _80SR::ENV_OUTPUT));
	}
};


Model* model_80SR = createModel<_80SR, _80SRWidget>("80SR");