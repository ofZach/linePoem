#include "testApp.h"


#include <fstream>
#include <iterator>
#include <vector>


ofPolyline             PR;
ofPolyline             PR2;
angleLengthLineMorpher   * ALM;
angleLengthLine          ** AL;
int                       nAngleLines;
int                       curAngleLine;
bool                   bDrawing;
int                       counter;
bool                   bMakingNYCard;
bool                   bPlaying;
float                     avgRate;
int                       count;




//---------------------------------------------------------------------
void loadAllDrawings(){
    
  
    ofBuffer buf = ofBufferFromFile("allDrawings.dat");
    
    char * buff  = buf.getBinaryBuffer();

    
    int pos = 0;
    for (int j = 0; j < 150; j++){
        for (int i = 0; i < 250; i++){
            
            
            
            float a;
            int * temp = (int *)&a;
            
            *temp  = (  ((buff[pos]  << 24) & 0xFF000000) + ((buff[pos+1] << 16) & 0x00FF0000) +  ((buff[pos+2] << 8) & 0x0000FF00) +   ((buff[pos+3] << 0) & 0x000000FF));
            pos += 4;
            //cout << a << endl;
            
            AL[j]->ang[i].angle = a;
        }
    }
}






//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
    ALM        = new angleLengthLineMorpher(250);
    
    nAngleLines    = 150;
    curAngleLine   = 5;
    AL = new angleLengthLine*[nAngleLines];
    for (int i = 0; i < nAngleLines; i++){
        AL[i] = new angleLengthLine(250);
    }
    
    bDrawing       = false;                                // am I drawing?
    counter        = 0;
    
    for (int i = 0; i < 10; i++){
        PR.addVertex(i * 20, 0);
        if (PR.size() > 4){
            PR2 = PR.getResampledByCount(250);
            //PR2.resampleIntoMe(PR, 250);
        }
    }
    for (int i = 0; i < nAngleLines; i++){
        AL[i]->convertFromAngleLengthLine(PR2);
        if (i == 0) ALM->setPres(AL[i]);
        else ALM->setTarg(AL[i]);
        ALM->setPres(AL[i]);
    }
    PR.clear();
    bDrawing = false;
    bPlaying = true;
   // status("loading data ");
    loadAllDrawings();
    //status("initializing system ");
    for (int i = 0; i < 100; i++){
        ALM->draw(0.5f + 0.5f * sin(ofGetElapsedTimeMillis() / 1000.0f));
    }
    //background(255);
    //stroke(0);
    //line(0,100,width, 100);
    bMakingNYCard = false;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    if (bDrawing == false) ofBackground(255);
    else                   ofBackground(230);
    ofFill();
    
    if (bDrawing == true){
        ofSetColor(255);
        ofRect(10,10, ofGetWidth()-20, ofGetHeight()-20);
        ofSetColor(100);
        PR2.draw();
    }
    
    
    
    // this is our "play" routine... play consecutive drawings *unless* we are drawing, then freeze...
    if (!bMakingNYCard || bPlaying){
        if (ALM->bAmAnimating == false && bDrawing == false){
            ALM->setTarg(AL[curAngleLine]);
            //if (bMakingNYCard) println("---> playing line : " + curAngleLine);
            curAngleLine++;
            curAngleLine %= nAngleLines;
            if (curAngleLine < 6) curAngleLine = 6;
            
        }
    }
    
    cout << curAngleLine << endl;
    
    // draw
    
    //stroke(0);
    if (bDrawing == false) ofSetColor(0);
    else                   ofSetColor(0);
    
    ALM->draw(0.5f + 0.5f * sin(ofGetElapsedTimeMillis() / 1000.0f));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == 'r'){
        curAngleLine++;
    }
    
    if (key == 'f'){
        ofToggleFullscreen();
    }
    
    if (!bMakingNYCard) return;
    
    
   
    if (key == 'I'){
        loadAllDrawings();
    }
    
    
    
    // when we are making the card, I want to save a bunch of drawings and
    // use key commands to hand select and overwrite...
    if (bMakingNYCard == true){
        
        if (key == 'a'){
            bPlaying = false;
            curAngleLine--;
            if (curAngleLine < 0) curAngleLine += nAngleLines;
            ALM->setPres(AL[curAngleLine]);
            ALM->setTarg(AL[curAngleLine]);
            //println("selected line " + curAngleLine);
        } else if (key == 's'){
            bPlaying = false;
            curAngleLine++;
            curAngleLine %= nAngleLines;
            ALM->setPres(AL[curAngleLine]);
            ALM->setTarg(AL[curAngleLine]);
            //println("selected line " + curAngleLine);
        } else if (key == 'z'){
            bPlaying = true;
            
        }
        
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if (!bMakingNYCard) return;
    PR.addVertex(mouseX, mouseY);
    if (PR.size() > 4){
        PR2 = PR.getResampledByCount(250);
    }
    bDrawing = true;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (!bMakingNYCard) return;
    AL[curAngleLine]->convertFromAngleLengthLine(PR2);
    ALM->setTarg(AL[curAngleLine]);
    if (bMakingNYCard != true){
        curAngleLine++;
        curAngleLine %= nAngleLines;
        
    }
    
    PR.clear();
    PR2.clear();
    bDrawing = false; 
    counter++;
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