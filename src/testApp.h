#pragma once

//#define OFX_MAX_RESOLUTION 1

#include <time.h>
#include "ofMain.h"
#include "ofxKinect.h"

#define MAX_FRAMES 96
#define LOOP_FRAMES 7

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
	
	void dibujarCaptura(int pantalla);
	void dibujarAnimacion(int pantalla);
	void dibujarAnimacionFrameAFrame(int pantalla);
	void prepararProximoFrame();
	void swapScreenPosition() {};
    void setFrameSize();
		
	ofVideoGrabber 		vidGrabber;
	unsigned char * 	videoInverted;
	ofTexture			videoTexture;
	int 				camWidth;
	int 				camHeight;
    int                 frameWidth;
    int                 frameHeight;
    
    ofxKinect kinect;
    bool isKinect;
    
    bool showHelp;
    
    
	ofTrueTypeFont myFont;
		
	/*** Espec’ficos del 96 frames ***/
	// Lo siguiente es la cantidad de frames en el buffer y sobre lo que va a hacer el loop
	vector <ofTexture> grafos;

    ofImage cartelon;

	double nextFrame;

	int     currentImg;
	int     direccion;
	int     maxImg;
	int     captureIdx;
	int     ultimoGrafo;
	int     nextSaveIdx;

    ofRectangle preview0, preview1;
    
	int     modo;
	float   rotation; //PI/2;
	float   _alphaValue;
    
	bool    logos;
	bool    fullscreen;
    bool    justPreview;
    bool    projectionMode;
	bool    ghostMode;
    bool    desdeElPrincipio;
	bool    showAnimacion;
	bool    showCaptura;
	bool    dibujarCartelon;
    bool    reportFrames;
};
