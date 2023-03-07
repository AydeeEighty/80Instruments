#include "plugin.hpp"

float p=0.f;
float square=0.f;


struct Sqr : Module {
	enum ParamId {
		PITCH_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		PITCH_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SQUARE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	

	Sqr() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
		configInput(PITCH_INPUT, "");
		configOutput(SQUARE_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		// Compute the frequency from the pitch parameter and input
		float pitch = params[PITCH_PARAM].getValue();
		pitch += inputs[PITCH_INPUT].getVoltage();
		pitch = clamp(pitch, -4.f, 4.f);
		// The default pitch is C4 = 261.6256f
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);
		p += freq * args.sampleTime;
		if (p >= 0.5f)
			p-= 1.f;

		square= sin(p/freq * 2.0 * M_PI)>=0.0 ? 1.0:-1.0;
		outputs[SQUARE_OUTPUT].setVoltage(5.f*square);

	}
};


struct SqrWidget : ModuleWidget {
	SqrWidget(Sqr* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Sqr.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(5.08, 54.657)), module, Sqr::PITCH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 78.853)), module, Sqr::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.08, 104.615)), module, Sqr::SQUARE_OUTPUT));
	}
};


Model* modelSqr = createModel<Sqr, SqrWidget>("Sqr");