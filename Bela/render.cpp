/*
 ____  _____ _        _
| __ )| ____| |      / \
|  _ \|  _| | |     / _ \
| |_) | |___| |___ / ___ \
|____/|_____|_____/_/   \_\
http://bela.io
*/
/**
\CSD2C_Eindopdracht

MadPanner
-------------------------

This sketch reads the analog inputs which are connected to 2 joysticks and 2 touchpads (Bela Trill Squares).
The data is sent via OSC to the laptop connected to the Bela running a C++ program.
*/

#include <Bela.h>
#include <cmath>
#include <libraries/Scope/Scope.h>
#include <libraries/OscSender/OscSender.h>
#include <libraries/Trill/Trill.h>
#include <iostream>
#include <pthread.h>

pthread_mutex_t oscMutex;

OscSender oscSender;
Trill touchPad1;
Trill touchPad2;

int gAudioFramesPerAnalogFrame = 0;

// Set the analog channels to read from
int joystickX = 0;
int joystickY = 1;
int joystick2X = 2;
int joystick2Y = 3;


//create the sensor variables
float xPos = 0.0f;
float yPos = 0.0f;
float xPos2 = 0.0f;
float yPos2 = 0.0f;

float touchPad1Xpos;
float touchPad1Ypos;
float touchPad2Xpos;
float touchPad2Ypos;


unsigned int auxTaskSleepTime = 20000;

const char* remoteIP = "192.168.6.1"; // IP address of the laptop that bela is connected to
int remotePort = 7563; // port to be sending OSC messages to


// Function to be run in separate thread that sends OSC messages to a receiver
void sendOSC(void*)
{
	while(!Bela_stopRequested()) //checks if the prorgram should end
	{
		touchPad1.readI2C();
		if (touchPad1.getNumTouches()) {
			touchPad1Xpos = touchPad1.compoundTouchHorizontalLocation() * 2.0f - 1.0f;
			touchPad1Ypos = touchPad1.compoundTouchLocation() * 2.0f - 1.0f;
			oscSender.newMessage("/touchpad1").add(touchPad1Xpos).add(touchPad1Ypos).send();

		}
		touchPad2.readI2C();
		if(touchPad2.getNumTouches()) {
			touchPad2Xpos = touchPad2.compoundTouchHorizontalLocation() * 2.0f - 1.0f;
			touchPad2Ypos = touchPad2.compoundTouchLocation() * 2.0f - 1.0f;
			oscSender.newMessage("/touchpad2").add(touchPad2Xpos).add(touchPad2Ypos).send();
		}
		
		// Since reading I2C and sending OSC takes some time, let the program sleep for x amount of time
		usleep(auxTaskSleepTime); 
		
		// Joystick value at center posititon is { 0.5, 0.5 }, scale this so the center becomes { 0.0, 0.0 }
		__wrap_pthread_mutex_lock(&oscMutex);
		xPos = xPos * 2.0f - 0.984f;
		yPos = yPos * 2.0f - 1.0f;

		xPos2 = xPos2 * 2.0f - 0.984f;
		yPos2 = yPos2 * 2.0f - 1.0f;

		// xPos = 0.0f;
		// yPos = 0.0f;
		// xPos2 = 0.0f;
		// yPos2 = 0.0f;

		// std::cout << "xPos: " << xPos2 << " " << "yPos: " << yPos2 << std::endl;
		// Send the joystick values via OSC
		oscSender.newMessage("/joystick1").add(xPos).add(yPos).send();
		oscSender.newMessage("/joystick2").add(xPos2).add(yPos2).send();
		__wrap_pthread_mutex_unlock(&oscMutex);

	}


}

bool setup(BelaContext *context, void *userData)
{

	// Check if analog channels are enabled
	if(context->analogFrames == 0 || context->analogFrames > context->audioFrames) {
		rt_printf("Error: this example needs analog enabled, with 4 or 8 channels\n");
		return false;
	}
	
	// Check if touchpads are connected and adressed correctly
	if(touchPad1.setup(1, Trill::SQUARE) != 0) {
		fprintf(stderr, "Unable to initialise Trill Square\n");
		return false;
	}
	
	if(touchPad2.setup(1, Trill::SQUARE, 41) != 0) {
		fprintf(stderr, "Unable to initialise Trill Square2\n");
		return false;
	}
	
	touchPad1.printDetails();
	touchPad2.printDetails();
	touchPad2.setNoiseThreshold(0.02);
	
	oscSender.setup(remotePort, remoteIP);
	oscSender.newMessage("/osc-connection").add("hello").send();
	
	// Send the OSC messages in a separate thread so it won't interfere with the audio thread
	Bela_runAuxiliaryTask(sendOSC);

	// Calculate amount of audio frames per analog frame
	gAudioFramesPerAnalogFrame = context->audioFrames / context->analogFrames;
	
	if (__wrap_pthread_mutex_init(&oscMutex, NULL))
		printf("unable to initialize mutex");
	
	
	
	
	return true;
}

void render(BelaContext *context, void *userData)
{

	for(unsigned int n = 0; n < context->audioFrames; n++) {
		if(gAudioFramesPerAnalogFrame && !(n % gAudioFramesPerAnalogFrame)) {
			// read analog inputs and update frequency and amplitude
			// Depending on the sampling rate of the analog inputs, this will
			// happen every audio frame (if it is 44100)
			// or every two audio frames (if it is 22050)
			__wrap_pthread_mutex_lock(&oscMutex);
			yPos = map(analogRead(context, n/gAudioFramesPerAnalogFrame, joystickX), 0.0f, 0.824f, 0.0f, 1.0f);
			xPos = map(analogRead(context, n/gAudioFramesPerAnalogFrame, joystickY), 0.0f, 0.824f, 0.0f, 1.0f);
			yPos2 = map(analogRead(context, n/gAudioFramesPerAnalogFrame, joystick2X), 0.0f, 0.824f, 0.0f, 1.0f);
			xPos2 = map(analogRead(context, n/gAudioFramesPerAnalogFrame, joystick2Y), 0.0f, 0.824f, 0.0f, 1.0f);
			__wrap_pthread_mutex_unlock(&oscMutex);
		}
	}
}	

void cleanup(BelaContext *context, void *userData)
{
	__wrap_pthread_mutex_destroy(&oscMutex);
}
