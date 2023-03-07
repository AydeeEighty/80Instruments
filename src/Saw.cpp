#include "plugin.hpp"

float phase = 0.f;
float saw=0.f;

struct Saw : Module {
	enum ParamId {
		PITCH_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		PITCH_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SAW_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Saw() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
		configInput(PITCH_INPUT, "");
		configOutput(SAW_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		float deltaTime = 1.0f / args.sampleRate;
		float pitch = params[PITCH_PARAM].getValue();
		pitch += inputs[PITCH_INPUT].getVoltage();
		pitch = clamp(pitch, -4.f, 4.f);
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);
		// Accumulate the phase
		phase += freq * deltaTime;
		if (phase >= 1.0f)
			phase -= 1.0f;
		float pinput = 0.5f;
        saw = cos(exp(pinput * M_PI * phase));
		outputs[SAW_OUTPUT].setVoltage(5.f * saw);
	}
};


struct SawWidget : ModuleWidget {
	SawWidget(Saw* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Saw.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(5.08, 54.657)), module, Saw::PITCH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 78.853)), module, Saw::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.08, 104.615)), module, Saw::SAW_OUTPUT));
	}
};


Model* modelSaw = createModel<Saw, SawWidget>("Saw");