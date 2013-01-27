#include "ofMain.h"

#include "anglePoint.h"


#pragma once

class angleLengthLine {
  
public:
    
  anglePoint        *ang;
  ofPoint       *pts;
  int          pointCounter;
   float       length;
  int         nPoints;         // how big is the ring buffer?
         
  //-------------------------------------------------------------
  angleLengthLine(  int nPointsToRecord  ) {
   
    nPoints       = nPointsToRecord;
    ang           = new anglePoint[nPoints]; 
    pts           = new ofPoint[nPoints];
    
    for (int i = 0; i < nPoints; i++){
      //ang[i] = new anglePoint();
      //pts[i] = new gesturePoint();
    }

    clear();
    
    
  }

  void clear(){
    pointCounter   = 0;    // simple, just clear the counter
  }


  

  void 	convertFromAngleLengthLine(ofPolyline mom){


    // I will have less points then a pointRecorder since, eh, I represent angles not points.
    // there for I will be n - 2, as the first and last point DONT have angles.
    // angle from 3 points routine:
    
    
    float f1, f2, f3;
    f2 = 0;
    
    pointCounter = mom.size() - 2;
    
    for(int i = 2; i < mom.size(); i++){
      ofPoint anglepointdata    = mom.getVertices()[i - 2];
      ofPoint anglepointdata1   = mom.getVertices()[i - 1];
      ofPoint anglepointdata2   = mom.getVertices()[i    ];
      if(i == 2){
       f2 = (float)(6.2831853071795862 - (returnAngle(anglepointdata1, anglepointdata2) + 3.1415926535897931));
      } 
      else {
        f1 = (float)(6.2831853071795862 - (returnAngle(anglepointdata1, anglepointdata2) + 3.1415926535897931));
        f3 = f1 - f2;
        if((double)f3 >= 3.1415926535897931)
          f3 = (float)(-((double)f2 + (6.2831853071795862 - (double)f1)));
        else
          if((double)f3 < -3.1415926535897931)
            f3 = (float)((double)f1 + (6.2831853071795862 - (double)f2));
        
        
          ang[i-2].set(f3);
        
        
        f2 = f1;
      }
    }
  }

  //-----------------------------------------------------------
  float returnAngle(ofPoint pointdata, ofPoint pointdata1){
    return (float)atan2(pointdata1.x - pointdata.x, pointdata1.y - pointdata.y);
  }
  
  
  void draw(float scalef, float offsetx, float offsety, float rot, int startOff){
    
      ofPoint p0, p1;
      //p0 = new gesturePoint();
      //p1 = new gesturePoint();
      p0.x = offsetx;
      p0.y = offsety;
      length = scalef;
    
//      float angleAdder = rot;
//      mom.beginShape(mom.LINE_STRIP); 
//      for (int i = startOff; i < pointCounter - 24; i++){
//        angleAdder += ang[i].angle;
//        p1.x = p0.x - length * (float)Math.cos(angleAdder);
//        p1.y = p0.y - length * (float)Math.sin(angleAdder);
//        mom.vertex(p0.x, p0.y);
//        p0.x = p1.x;
//        p0.y = p1.y;
//      }
//      mom.endShape(); 
      
       
      
      
  
  
  }
  
  
};
