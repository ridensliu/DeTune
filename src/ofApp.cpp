#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //设置GUI
    gui.setup();
    gui.add(loadButton.setup("Load Image"));
    gui.add(intSlider.setup("Vert Num Reset", 30, 3, 99));
    gui.add(button.setup("<- Reset the Delaunay"));
    gui.add(floatSliderBlue.setup("blue color", 255., 0., 255.));
    gui.add(floatSliderRed.setup("red color", 255., 0., 255.));
    gui.add(floatSliderGreen.setup("green color", 255., 0., 255.));
    gui.add(floatSlider1.setup("noise", 1., 0., 5.));
    gui.add(floatSlider2.setup("wire", 1., 0., 1.));
    
    
    upd_num = 0;
    // 设置德劳内三角
//    vertCount = 700;
    vertCount = intSlider;
    // Adding POINTS To Generate Delaunay Triangles
    for (int i=0; i<vertCount; i++)
    {
        float x = ofRandom(1024);
        float y = ofRandom(1024);
        float z = ofRandom(200) + 700;
        ofPoint randomPoint(x, y, z);
        delaunay.addPoint(randomPoint);
    }
    delaunay.triangulate();
    
    
    // Sphere
//    int dim = 20;
//    float spacing = (M_PI * 2)/dim;
//    int numPoints = dim * dim;
//    int size = 200;
//
//    for(int i = 0; i < dim; i++){
//        float z = size * cos(spacing / 2 * i);
//        float s = size * sin(spacing / 2 * i);
//
//        for(int j = 0; j < dim; j++){
//            ofPoint randomPoint(cos(spacing * j) * s, sin(spacing * j) * s, z);
//            delaunay.addPoint(randomPoint);
//        }
//        delaunay.triangulate();
//    }
    
    
    // Light
    light.enable();
    light.setPosition(ofVec3f(100,100,200));
    light.lookAt(ofVec3f(0,0,0));
    
    // Texture
    ofDisableArbTex();
    ofLoadImage(mTex,"noiseTexture.png");
    img.load("sand.png");
    mTex.setTextureWrap(GL_REPEAT, GL_REPEAT);
    mTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    
    shaderColor.load("shader3/shader");
    shaderWire.load("shader3/shader.vert", "shader3/shader_wire.frag");
    shaderBG.load("shader3/shader_bg.vert", "shader3/shader.frag");
    
    // Sound
    //waveIndex = 0;
    
    clock.setTempo(180);
    clock.setTicksPerBeat(2);
    
    int sampleRate = 44100;
    int bufferSize = 512;
    tickTime = 1 / (double) sampleRate;
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    
    font.load("Inter-Bold.ttf", 10);
    
    loadDButton.setup(20, ofGetWindowHeight() - 60, 200, 40, "CHOOSE IMAGE");
    loadDButton.font = &font;
    
    wireSlider.setup(230, ofGetWindowHeight() - 60, 200, 40, "WIRE OPACITY");
    wireSlider.font = &font;
    wireSlider.setValue(1);
    wireSlider.hide = true;
    
    volumeSlider.setup(440, ofGetWindowHeight() - 60, 200, 40, "VOLUME");
    volumeSlider.font = &font;
    volumeSlider.setValue(1);
    volumeSlider.hide = true;
    
    countSlider.setup(650, ofGetWindowHeight() - 60, ofGetWindowWidth() - 650 - 20, 40, "VERTEX COUNT");
    countSlider.font = &font;
    countSlider.min = 20;
    countSlider.max = 60;
    countSlider.step = 4;
    countSlider.setValue(lastCount);
    countSlider.hide = true;
    
    logo.load("detune_logo.png");
}

void ofApp::setupImage(std::string imgPath) {
    img.load(imgPath);
    
    float width = img.getWidth();
    float height = img.getHeight();
    
    // Analyse the image
    warmness = 0.0;
    ofMesh &mesh = delaunay.triangleMesh;
    for (int i = 0; i < mesh.getNumVertices(); i++) {
        ofVec3f vertex = mesh.getVertex(i);
        vertex.x /= 1024.0;
        vertex.y /= 1024.0;
        
        ofColor col = img.getColor(floor(vertex.x * width), floor(vertex.y * height));
        warmness += getColorWarmness(col);
        
        mesh.setColor(i, col);
    }
    warmness /= mesh.getNumVertices();
    clock.setTempo(ofLerp(30, 220, warmness));
    float beatLength = 60.0 * 1000.0 / clock.bpm;
    if (warmness > 0.5) {
        envelope.setAttack(10);
        envelope.setDecay(beatLength * 0.3);
        envelope.setSustain(0.8);
        envelope.setRelease(beatLength * 0.5);
    } else {
        float p = 1 - warmness / 0.5;
        envelope.setAttack(beatLength * 0.35);
        envelope.setDecay(beatLength * 0.3);
        envelope.setSustain(0.65);
        envelope.setRelease(beatLength * 0.35);
    }
    
    imgLoaded = true;
    
    wireSlider.hide = false;
    volumeSlider.hide = false;
    countSlider.hide = false;
    
    loadDButton.setLabel("REPLACE IMAGE");
}

// 0 for coldest, 1 for most warm
float ofApp::getColorWarmness(ofColor color) {
    return abs(161 - color.getHue()) / 128.0;
}

//--------------------------------------------------------------
void ofApp::update(){
//    if(upd_num <= 700){
//    vertCount = 3;
//    // Adding POINTS To Generate Delaunay Triangles
//    for (int i=0; i<vertCount; i++)
//    {
//        float x = ofRandom(1024);
//        float y = ofRandom(1024);
//        float z = ofRandom(200) + 700;
//        ofPoint randomPoint(x, y, z);
//        delaunay.addPoint(randomPoint);
//    }
//    delaunay.triangulate();
//    upd_num ++;
//    }
    wireSlider.handleMove(ofGetMouseX(), ofGetMouseY());
    volumeSlider.handleMove(ofGetMouseX(), ofGetMouseY());
    countSlider.handleMove(ofGetMouseX(), ofGetMouseY());
    
    int countVal = (int) countSlider.getValue();
    if(countVal != lastCount){
        delaunay.reset();
        upd_num = 0;
        vertCount = intSlider;
        // Adding POINTS To Generate Delaunay Triangles
        for (int i=0; i< countVal; i++)
        {
            float x = ofRandom(1024);
            float y = ofRandom(1024);
            float z = ofRandom(200) + 700;
            ofPoint randomPoint(x, y, z);
            delaunay.addPoint(randomPoint);
        }
        delaunay.triangulate();
        
        if (imgPath != "") setupImage(imgPath);
        
        lastCount = countVal;
    }
    
    if (loadButton || loadDButton.getClick()) {
        ofFileDialogResult result = ofSystemLoadDialog("Load image file");
        
        if (result.bSuccess) {
            string path = result.getPath();
            string ext = ofFilePath::getFileExt(path);
            if (ext == "jpg" || ext == "png" || ext == "jpeg") {
                setupImage(path);
                imgPath = path;
            } else {
                ofSystemAlertDialog("Do not support ." + ext + " file extension");
            }
        }
        
        float w = ofGetWindowWidth();
        float h = ofGetWindowHeight();
        for(int x = 0; x < 8; x++){
            for(int y = 0; y < 8; y++){
                ofPoint pos(floor(ofRandom(w + 400) - 200), floor(ofRandom(h + 400) - 200), 0);
                delaunay2d.addPoint(pos);
            }
        }
        
        delaunay2d.triangulate();
        
        ofMesh &mesh = delaunay2d.triangleMesh;
        for (int i = 0; i < mesh.getNumVertices(); i++) {
            ofVec3f vertex = mesh.getVertex(i);
            float px = ofClamp(vertex.x / w, 0, 0.999);
            float py = ofClamp(vertex.y / h, 0, 0.999);
            ofColor c = img.getColor(floor(px * img.getWidth()), floor(py * img.getHeight()));
            mesh.setColor(i, c);
        }
    }
    
    loadDButton.update();
    wireSlider.update();
    volumeSlider.update();
    countSlider.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    // wireframe动态颜色
//    for (int i = 0; i < vertCount; i++) {
//        ofVec3f pos = delaunay.triangleMesh.getVertex(i);
//        delaunay.triangleMesh.setColor(i, ofFloatColor(sin(ofGetElapsedTimef() + pos.x / 300.0) * 0.5 + 0.5, cos(ofGetElapsedTimef()) * 0.5 + 0.5, 1.0));
//    }

    ofBackground(255, 255, 255);
    
    shaderBG.begin();
    
    if (imgLoaded) delaunay2d.triangleMesh.draw();
    
    shaderBG.end();
    
    ofSetColor(255, 255, 255, 200);
    ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    camera.begin();
//    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2, 0);
    //comment when is a sphere

    //Backgroud Color

    // Bind Texture
    
    shaderColor.begin();
    shaderColor.setUniform1f("time", ofGetElapsedTimef());
    shaderColor.setUniform1f("bluecolor", floatSliderBlue);
    shaderColor.setUniform1f("redcolor", floatSliderRed);
    shaderColor.setUniform1f("greencolor", floatSliderGreen);
    shaderColor.setUniform1f("noisevalue", floatSlider1);
    
    // Draw Delaunay meshes
    delaunay.triangleMesh.draw();
    
    shaderColor.end();

//    img.getTexture().bind();
    shaderWire.begin();
    shaderWire.setUniform1f("time", ofGetElapsedTimef());
    shaderWire.setUniform1f("noisevalue", floatSlider1);
    shaderWire.setUniform1f("wirevalue", wireSlider.getValue());
    
    delaunay.triangleMesh.drawWireframe();
    
    shaderWire.end();
//    img.getTexture().unbind();
//    img.unbind();

    // unbind Texture
    //mTex.unbind();

    camera.end();
    // gui.draw();
    
    loadDButton.draw();
    wireSlider.draw();
    volumeSlider.draw();
    countSlider.draw();
    
    if (!imgLoaded) {
        ofPushStyle();
        
        std:string tipsText = "<- CLICK TO START AN EXPERIMENT OF SOUND AND VISUAL.";
        ofSetColor(0, 0, 0);
        font.drawString(tipsText, 240, ofGetWindowHeight() - 40 + font.getAscenderHeight() * 0.5);
        
        ofPopStyle();
    }
    
    logo.draw(ofGetWindowWidth() * 0.5 - logo.getWidth() * 0.5, 30);

//    ofSetColor(255);
    
//    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
}

void ofApp::audioOut(ofSoundBuffer& output) {
    std::size_t outChannels = output.getNumChannels();
    
    float upPos = ofLerp(0.15, 0.45, warmness);
    float downPos = 0.6 - upPos;
    
    for (int i = 0; i < output.getNumFrames(); ++i) {
        clock.ticker();
        dspTime += tickTime;
        
        if (clock.tick) {
            if (skipTick) {
                skipTick = false;
            } else {
                double r = ofRandom(0, 1);
                if (r > 1 - upPos) {
                    currentNote += (r - 1 + upPos) > 0.35 ? 2 : 1;
                    currentNote = ofClamp(currentNote, 0, 7);
                } else if (r > 1 - upPos - downPos) {
                    currentNote -= (r - 1 + upPos + downPos) > 0.35 ? 2 : 1;
                    currentNote = ofClamp(currentNote, 0, 7);
                } else if (r > 0.1) {
                    // Do nothing
                } else if (r > 0.05) {
                    currentNote = currentNote + 3 > 7 ? currentNote - 3 : currentNote + 3;
                    currentNote = ofClamp(currentNote, 0, 7);
                } else {
                    currentNote = -1;
                }
                skipTick = true;
            }
        }
        
        float env;
        env = envelope.adsr(1.0, clock.playHead % 2 == 0);
        
        double freq = currentNote >= 0 ? scaleFreq[currentNote] : 0;
        output[i * outChannels] = getSound(freq) * 0.8 * env * (imgLoaded ? 1.0 : 0.0) * volumeSlider.getValue();
        output[i * outChannels + 1] = output[i * outChannels];
    }
}

double ofApp::getSound(double freq) {
    return oscillator1.sinewave(freq) * 0.4 + oscillator2.sinewave(freq * 2) * 0.3 + oscillator3.sinewave(freq * 3) * 0.15 + oscillator4.sinewave(freq * 4) * 0.05;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    delaunay.reset();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    loadDButton.handleMove(x, y);
    // wireSlider.handleMove(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//    delaunay.addPoint(ofPoint(x,y));
//    delaunay.triangulate();
    loadDButton.handlePressed(x, y);
    wireSlider.handlePressed(x, y);
    volumeSlider.handlePressed(x, y);
    countSlider.handlePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    loadDButton.handleReleased(x, y);
    wireSlider.handleReleased(x, y);
    volumeSlider.handleReleased(x, y);
    countSlider.handleReleased(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
