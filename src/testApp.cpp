#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    cout << "Screen size: " << ofGetWidth() << " x " << ofGetHeight() << std::endl;
    ofHideCursor();
    desdeElPrincipio = false;
    //
    //kinect.init();
    isKinect = (kinect.numTotalDevices() > 0);
    cout << (isKinect?"Kinect!":":( no Kinect") << std::endl;
    if (isKinect) {
        kinect.init();
        //kinect.init(true); // shows infrared instead of RGB video image
        //kinect.init(false, false); // disable video image (faster fps)
        kinect.open();		// opens first available kinect
        camHeight = kinect.height;
        camWidth = kinect.width;
    } else {
        vidGrabber.listDevices();
        camWidth 		= 1024;	// try to grab at this size.
        camHeight 		= 768;
	
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(camWidth,camHeight);
        
        camWidth = vidGrabber.getWidth();
        camHeight = vidGrabber.getHeight();
    }
    
    preview0.setWidth(1024/2);
    preview0.setHeight(768/2);
    preview1.setWidth(1024/2);
    preview1.setHeight(768/2);
    
    
	cout << "W:" << camWidth << " H:" << camHeight <<std::endl;
    
	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_RGB);	
	
	currentImg = 0;
	direccion = 1;
	maxImg = -1;
	captureIdx = -1;
	nextFrame = 0;
	modo = 0;
	fullscreen = false;
	rotation=0; //PI/2;
	ghostMode = true;
	_alphaValue = 255;
	logos = false;
	ultimoGrafo = 0;
	nextSaveIdx = 0;
	showAnimacion = true;	
	showCaptura = true;
    justPreview = true;
    reportFrames = false;
    
	
	myFont.loadFont("Chalet Tokyo.otf", 12);

	ofBackground(0,0,0);
	ofEnableAlphaBlending();
	dibujarCartelon = false;
	cartelon.loadImage("CartelonMaisanto.jpg");
	ofSetWindowPosition(1260,-160);
    //ofSetWindow
	//ofSetFullscreen(true);
	showHelp = false;
    
    setFrameSize();
}

//--------------------------------------------------------------
void testApp::draw(){
	if (dibujarCartelon) {
		if (cartelon.bAllocated()) cartelon.draw(0,0);
        else ofBackground(0,0,0);
		return;
	}
    
    if (justPreview) {
        dibujarCaptura(-1);
    } else {
        //ofTranslate(0,20);

        if (modo == 3) dibujarAnimacionFrameAFrame(0);
        else dibujarAnimacion(0);
        if (showCaptura) {
            dibujarCaptura(1);
            //dibujarCaptura(2);
        }
    }


	if (!fullscreen) {
		char reportStr[512];
        if (reportFrames) {
            ofSetColor(128+64);
            sprintf(reportStr, "Capturados %i frames. Mostrando el frame %i. ", (int)grafos.size(), currentImg+1);
            myFont.drawString(reportStr, 20, ofGetHeight()-10);
        }
	
		if (logos) {
			//fill(255);
			ofSetColor(255);
			sprintf(reportStr, "http://96frames.ludic.cc/");
			myFont.drawString(reportStr, ofGetWidth() - 210, ofGetHeight()-10);
		}
        
        if (desdeElPrincipio) {
            ofSetColor(255,0,0);
            sprintf(reportStr, "DP");
            myFont.drawString(reportStr, 500, ofGetHeight()-70);
        }
        ofSetColor(255);
        if (showHelp) {
            ofSetColor(255);
            sprintf(reportStr, "'h': esta ayuda\n' ' : capturar un frame\n'b' : empezar desde 0\n'p' : dibujar cartelon\n'd' : borrar la œltima captura\nLEFT: ida y vuelta\nRIGHT: ida\n'0' : solo el PREVIEW.\n'1' : modo 2\n'2' : modo 3\n's' : swap screens\n'c' : show capture\n'f' : fullscreen\n'r' : rotar captura\n'g' : toggle ghost mode\na/s : alpha -/+\n'l' : show logos\n'P' : show animacion\n'm' : desde el principio toggle");
            myFont.drawString(reportStr, 300, 400);
        }
	}
}
void testApp::dibujarAnimacionFrameAFrame(int pantalla) {
	if (grafos.size() > 0) {
		try {
			ofPushMatrix();
			ofTranslate((ofGetWidth()/2)*pantalla-camWidth/20, frameHeight/2-camHeight/20);
			ofRotateZ(rotation);
            ofTranslate(camWidth/20, camHeight/20);
			for (int i = 0; i < currentImg; i++) {
				grafos[i].draw(-frameWidth/2+(i%10)*camWidth/10, -frameHeight/2+((int)i/10)*camHeight/10, camWidth/10, camHeight/10);
			}
			ofPopMatrix();
			
		} 
		catch (...) {
			cout << "Ups! " << std::endl; //+ e.toString());
			currentImg = 0;
		}
    }
}
void testApp::dibujarAnimacion(int pantalla) {
	if (grafos.size() > 0) {
		try {			
            ofPushMatrix();
            
            if (!fullscreen) {
                ofTranslate(ofGetWidth()/4*3-frameWidth/2, ofGetHeight()/2-frameHeight/2);
            } else {
                ofTranslate(ofGetWidth()/2-frameWidth/2, ofGetHeight()/2-frameHeight/2);
            }
        
            ofTranslate(frameWidth/2, frameHeight/2);
            ofRotateZ(rotation);
            
            ofTranslate(-frameWidth/2, -frameHeight/2);
            
            ofSetHexColor(0xffffff);
            grafos[currentImg].draw(0, 0, frameWidth, frameHeight);
            ofPopMatrix();
			
			
		} 
		catch (...) {
			cout << "Ups! " << std::endl; //+ e.toString());
			currentImg = 0;
		}
    }
}

void testApp::dibujarCaptura(int pantalla) {
    if (fullscreen) return;

    if (pantalla == -1) {
        ofPushMatrix();
        ofTranslate(frameWidth/2, frameHeight/2);
        ofRotateZ(rotation);
        ofTranslate(-frameWidth/2, -frameHeight/2);

        ofSetHexColor(0xffffff);
        //ofRect(-1,-1, frameWidth+2, frameHeight+2);
                
        if (isKinect) kinect.draw(0,0, frameWidth, frameHeight);
        else vidGrabber.draw(0, 0, frameWidth, frameHeight);

        ofPopMatrix();
    } else {
    
        ofPushMatrix();
        ofTranslate(ofGetWidth()/4-frameWidth/2, ofGetHeight()/2-frameHeight/2);
        ofTranslate(frameWidth/2, frameHeight/2);
        
        ofRotateZ(rotation);
        
        ofTranslate(-frameWidth/2, -frameHeight/2);

        ofSetHexColor(0xffffff);
        //ofRect(-1,-1, frameWidth+2, frameHeight+2);

        if (isKinect) kinect.draw(0,0, frameWidth, frameHeight);
        else vidGrabber.draw(0, 0, frameWidth, frameHeight);
        
        if (ghostMode && grafos.size() > 0) {
            ofSetColor(255, 255, 255, 80);
            grafos[grafos.size()-1].draw(0,0, frameWidth, frameHeight);
        }
        ofPopMatrix();
    }
	
}

void testApp::prepararProximoFrame()
{
	if (grafos.size() == 0) return;
    
    int bottom = (desdeElPrincipio)?0:MAX(0,grafos.size()-LOOP_FRAMES-1);
    
	switch(modo) {
		case 3:
		case 0:
			currentImg+= direccion;
			if (currentImg > grafos.size()-1 || currentImg < bottom) {
                if (currentImg < bottom) currentImg = bottom-1;
				direccion *= -1;
				currentImg += direccion;
			}
			break;
			
		case 1:
			currentImg+= direccion;
			if (currentImg > grafos.size()-1) {
				currentImg = bottom;
			}
			break;
			
		case 2:
			currentImg = (int)ofRandom(0, grafos.size());
			cout << "currentImg=" << currentImg << std::endl;
			break;
	}
}

void testApp::setFrameSize() {
	
	if (rotation == 90 || rotation == 270) {
		frameHeight = (fullscreen || justPreview)?ofGetHeight():min(ofGetHeight(), camHeight);
		frameWidth = frameHeight*camWidth/camHeight;
        if (frameWidth > ofGetWidth()) {
            frameWidth = (fullscreen || justPreview)?ofGetWidth():min(ofGetWidth()/2, camWidth);
            frameHeight = camHeight*frameWidth/camWidth;
        }
	}
	else {
		frameWidth = (fullscreen || justPreview)?ofGetWidth():min(ofGetWidth()/2, camWidth);
		frameHeight = camHeight*frameWidth/camWidth;
        if (frameHeight > ofGetHeight()) {
            frameHeight = (fullscreen || justPreview)?ofGetHeight():min(ofGetHeight(), camHeight);
            frameWidth = frameHeight*camWidth/camHeight;
        }
	}
    cout << "El tama–o de los frames es de " << frameWidth << " x " << frameHeight << std::endl;
}
//--------------------------------------------------------------
void testApp::update(){
	
	//ofBackground(100,100,100);
	
    bool newFrame = false;
    unsigned char * pixels;
    
	if (isKinect) {
        kinect.update();
        newFrame = kinect.isFrameNew();
        pixels = kinect.getPixels();
    } else {
        vidGrabber.grabFrame();
        newFrame = vidGrabber.isFrameNew();
        pixels = vidGrabber.getPixels();
    }
	
	if (vidGrabber.isFrameNew()){
		int totalPixels = camWidth*camHeight*3;
		for (int i = 0; i < totalPixels; i++){
			videoInverted[i] = 255 - pixels[i];
		}
		videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
	}
	
	if (nextFrame < ofGetElapsedTimeMillis() && showAnimacion) {
		nextFrame = ofGetElapsedTimeMillis()+1000/4; //MIN(8, grafos.size()+1);
		
		//dibujarAnimacion(2);
		prepararProximoFrame();
	}
    
    
}




//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	// in fullscreen mode, on a pc at least, the 
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...
    if (key == 'o') {
        projectionMode++;
        if (projectionMode > 3) projectionMode = 0;
    }
	if (key == 'p') {
		dibujarCartelon = !dibujarCartelon;
	} else if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
	} else if (key == ' ') {
		if (grafos.size() == MAX_FRAMES) {
			grafos.erase(grafos.begin());
		}
		ofTexture newImg;
		newImg.allocate(camWidth,camHeight, GL_RGB);
        if (isKinect) newImg.loadData(kinect.getPixels(), camWidth, camHeight, GL_RGB);
        else newImg.loadData(vidGrabber.getPixels(), camWidth, camHeight, GL_RGB);
		grafos.push_back(newImg);
		
		//grafos.add(img2);
		//player.trigger();
		//println("added...");
        
		char file_name[256];
		
		time_t rawtime;
		struct tm * timeinfo;
		
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		
		sprintf(file_name, "capturas/%i-%02i-%02i_%02i-%02i-%02i_%lu.tif", timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, ofGetElapsedTimeMillis());
		cout << "Up to save to " << file_name ;
		//if (isKinect) ofSaveImage(kinect.getPixelsRef(), file_name, OF_IMAGE_QUALITY_BEST);
        //else  ofSaveImage(vidGrabber.getPixelsRef(), file_name, OF_IMAGE_QUALITY_BEST);
        ofPixels hiResPixels;
        newImg.readToPixels(hiResPixels);
        ofSaveImage(hiResPixels, file_name);
		cout << "... done" << std::endl;
        
	} 
    else if (key == 'h') showHelp = !showHelp;
	else if (key == 'b') {
		grafos.clear();
		currentImg = 0;
		ofBackground(0,0,0);
	} 
	else if (key == 'd') {
		if (grafos.size()>0) grafos.pop_back();
		if (grafos.size() == 0) ofBackground(0,0,0);
	} 
	else if (key == OF_KEY_LEFT) {
		modo = 0;
	} 
	else if (key == OF_KEY_RIGHT) {
		modo = 1;
		direccion = 1;
        currentImg = grafos.size()-1;
	}
	else if (key == '0') {
        //justPreview = !justPreview;
        justPreview = true;
        setFrameSize();
        //fullscreen = true;
        //showAnimacion = false;
	} else if (key == '9') {
        justPreview = false;
        fullscreen = false;
        showCaptura = true;
        showAnimacion = true;
        reportFrames = false;
        modo = 0;
        setFrameSize();
    } else if (key == '1') {
		modo = 2;
	} else if (key == '2') {
		modo = 3;
	} else if (key == 's') {
		swapScreenPosition();
	} else if (key == 'c') {
		showCaptura = !showCaptura;
	}
	else if (key == 'f') {
		fullscreen = !fullscreen;
		ofBackground(0,0,0);
        setFrameSize();
	} else if (key == 'F') {
		ofToggleFullscreen();
	} else if (key == 'r') {
		ofBackground(0,0,0);
		if (rotation < 270) rotation += 90;
		else rotation = 0;
        setFrameSize();
	}
	else if (key == 'R') reportFrames = !reportFrames;
	else if (key == 'g') ghostMode = !ghostMode;
	else if (key == 'a') {
		_alphaValue = MAX(0, _alphaValue-1);
	} 
	else if (key == 'A') {
		_alphaValue = MAX(0, _alphaValue-10);
	} 
	else if (key == 's') {
		_alphaValue = MIN(255, _alphaValue+1);
	}
	else if (key == 'S') {
		_alphaValue = MIN(255, _alphaValue+10);
	} 
	else if (key == 'l') {
		logos = !logos;
	} else if (key == 'P') showAnimacion = !showAnimacion;
    else if (key =='m') desdeElPrincipio = !desdeElPrincipio;
	if (!logos) {
		ofSetColor(0,0,0);
		ofRect(0, ofGetHeight()-40, ofGetWidth(), 40);
	}
	cout << "Frames: " << (grafos.size()+1) << " / " << (MAX_FRAMES+1) << " " << "_alphaValue:" << _alphaValue <<std::endl;	
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
