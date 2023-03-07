//This code is originally taken from the VCV Rack Plugin Development Tutorial

#include "plugin.hpp"


struct Sin : Module {
	float phase = 0.f;
	float blinkPhase = 0.f;
	enum ParamId {
		PITCH_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		PITCH_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SINE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Sin() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
		configInput(PITCH_INPUT, "");
		configOutput(SINE_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		// Compute the frequency from the pitch parameter and input
		float pitch = params[PITCH_PARAM].getValue();
		pitch += inputs[PITCH_INPUT].getVoltage();
		pitch = clamp(pitch, -4.f, 4.f);
		// The default pitch is C4 = 261.6256f
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

		// Accumulate the phase
		phase += freq * args.sampleTime;
		if (phase >= 0.5f)
			phase -= 1.f;

		// Compute the sine output
		float sine = std::sin(2.f * M_PI * phase);
		// Audio signals are typically +/-5V
		// https://vcvrack.com/manual/VoltageStandards

		
		outputs[SINE_OUTPUT].setVoltage(5.f * sine);

	}
};


struct SinWidget : ModuleWidget {
	SinWidget(Sin* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Sin.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(5.396, 54.657)), module, Sin::PITCH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.222, 79.027)), module, Sin::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.222, 106.356)), module, Sin::SINE_OUTPUT));

		
	}
};


Model* modelSin = createModel<Sin, SinWidget>("Sin");