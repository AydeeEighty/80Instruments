#include "plugin.hpp"


struct Bullet : Module {
	enum ParamId {
		INITY_PARAM,
		VEL_PARAM,
		GRAV_PARAM,
		ANGLE_PARAM,
		DRAG_PARAM,
		TRIG_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		VEL_INPUT,
		TRIG_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		EOC_OUTPUT,
		INVY_OUTPUT,
		Y_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		TRIG_LIGHT,
		LIGHTS_LEN
	};

	dsp::PulseGenerator EOCPulse;
	dsp::BooleanTrigger trigButtonTrigger;
	bool isRunning = false;
	float yInitPos=0.0;
	float yPos=0.0;
	float t=0.0;
	float grav=0.0;
	float theta=0.0;
	float velocity=0.0;
	float PI=3.1415926;
	float drag=0.5;
	float out=0.0;

	Bullet() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(INITY_PARAM, -5.f, 5.f, 0.f, "Ground Position");
		configParam(VEL_PARAM, 0.1f, 10.f, 5.f, "Velocity");
		configParam(GRAV_PARAM, 0.5f, 9.f, 4.5f, "Gravity");
		configParam(ANGLE_PARAM, 20.f, 90.f, 45.f, "Angle");
		configButton(TRIG_PARAM,"Trigger");
		configInput(TRIG_INPUT, "Trigger");
		configOutput(Y_OUTPUT, "Trajectory");
		configParam(DRAG_PARAM, 0.0001f, 0.01f, 0.005f, "Drag");
		configInput(VEL_INPUT, "Velocity CV");
		configOutput(EOC_OUTPUT, "End of Cycle");
		configOutput(INVY_OUTPUT, "Inverted Trajectory");
	}

	void process(const ProcessArgs& args) override {

		if ((inputs[TRIG_INPUT].getVoltage()>0) & (!isRunning)){
			yInitPos=params[INITY_PARAM].getValue();
			if (inputs[VEL_INPUT].isConnected()){
				velocity=inputs[VEL_INPUT].getVoltage();
			}
			else {
				velocity=params[VEL_PARAM].getValue();
			}
			
			grav=params[GRAV_PARAM].getValue();
			theta=params[ANGLE_PARAM].getValue();
			drag=params[DRAG_PARAM].getValue();
			t=0.0;
			yPos=yInitPos;
			isRunning=true;
		}
		if (trigButtonTrigger.process(params[TRIG_PARAM].getValue()) & !isRunning){
			yInitPos=params[INITY_PARAM].getValue();
			if (inputs[VEL_INPUT].isConnected()){
				velocity=inputs[VEL_INPUT].getVoltage();
			}
			else {
				velocity=params[VEL_PARAM].getValue();
			}
			grav=params[GRAV_PARAM].getValue();
			theta=params[ANGLE_PARAM].getValue();
			drag=params[DRAG_PARAM].getValue();
			t=0.0;
			yPos=yInitPos;
			isRunning=true;
		}
		if (isRunning){
			t+=args.sampleTime;
			velocity = velocity - (drag)*t;
			yPos=(velocity*t*std::sin( theta * PI / 180.0)-(0.5*grav*(t*t)))+yInitPos;
			
			if ((yPos>5) | (yPos<yInitPos)){
				isRunning=false;
				EOCPulse.trigger(1e-3f);
				out = EOCPulse.process(args.sampleTime); 
				outputs[EOC_OUTPUT].setVoltage(10.f*out);
				}
			if (outputs[Y_OUTPUT].isConnected()) {
			outputs[Y_OUTPUT].setVoltage(yPos);
			 }
			if (outputs[INVY_OUTPUT].isConnected()){
			outputs[INVY_OUTPUT].setVoltage(-1*yPos);
			}
			lights[TRIG_LIGHT].setBrightness(1.0);	
			
			

		}
		else{

			lights[TRIG_LIGHT].setBrightness(0.0);
			outputs[Y_OUTPUT].setVoltage(yInitPos);
			outputs[INVY_OUTPUT].setVoltage(yInitPos);
			outputs[EOC_OUTPUT].setVoltage(0.0);
		}
	

	}
};


struct BulletWidget : ModuleWidget {
	BulletWidget(Bullet* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Bullet.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.48, 28.489)), module, Bullet::INITY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(22.374, 52.174)), module, Bullet::VEL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(38.516, 52.174)), module, Bullet::GRAV_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(22.702, 73.556)), module, Bullet::ANGLE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(38.783, 73.556)), module, Bullet::DRAG_PARAM));
		addParam(createLightParamCentered<VCVLightButton<MediumSimpleLight<WhiteLight>>>(mm2px(Vec(30.48, 93.468)), module, Bullet::TRIG_PARAM, Bullet::TRIG_LIGHT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.412, 51.975)), module, Bullet::VEL_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.221, 109.748)), module, Bullet::TRIG_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.48, 110.145)), module, Bullet::EOC_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(40.765, 110.343)), module, Bullet::INVY_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(51.684, 110.343)), module, Bullet::Y_OUTPUT));
	}
};


Model* modelBullet = createModel<Bullet, BulletWidget>("Bullet");