import processing.core.*;



class pointRecorder {

  gesturePoint pos[];
  int nPoints;         // how big is the ring buffer?
  int pointCounter;    // how many points have we added?

  //-------------------------------------------------------------
  pointRecorder(  int nPointsToRecord  ) {
    nPoints        = nPointsToRecord;
    pos           = new gesturePoint[nPoints]; 
    for (int i = 0; i < nPoints; i++){
      pos[i] = new gesturePoint(); 
    }
    pointCounter   = 0;
  }

  void clear(){
    pointCounter   = 0;    // simple, just clear the counter
  }
  //-------------------------------------------------------------
  void addPoint( float x, float y  ) {
    // use the mod operator to put myself in the right place in the ring buffer

    if (pointCounter < nPoints){
      int posInArray =   pointCounter % nPoints;
      pos[posInArray].set(x,y);
      pointCounter++;
    }
  }  

  void draw(PApplet mom){

    // we have two scenarios : (a) we haven't filled the ring buffer
    //                         (b) we have filled the ring buffer


    if (pointCounter < nPoints){          // (a) 

      //--------------
      // draw in a normal manner, 
  
      mom.beginShape(mom.LINE_STRIP); 
      for (int i = 0; i < pointCounter; i++){
        mom.vertex(pos[i].x,pos[i].y);
      }
      mom.endShape(); 
      //--------------

    } 
    else {                              // (b) 

      //--------------
      // draw based on ring buffer oldest to newest, 

      int start = pointCounter;
      int end   = pointCounter + nPoints;
      
      mom.beginShape(mom.LINE_STRIP); 
      for (int i = start; i < end; i++){
        int posInArray = i % nPoints;              // this is tricky, it's how we deal with the "jump" in the ring buffer
        mom.vertex(pos[posInArray].x,pos[posInArray].y);
      }
      mom.endShape(); 
      //--------------
    }
  }

  // ------------------------------
  float calculateLength(){
    if (nPoints < 2){
      return 0;
    } 
    else {
      float len = 0.0f;
      float difx, dify;
      for (int i = 1; i < pointCounter; i++){
        difx = pos[i].x - pos[i-1].x;
        dify = pos[i].y - pos[i-1].y;
        len += Math.sqrt(difx*difx + dify*dify);
      }
      return len;
    }
  }

  //-------------------------------


  void 	resampleIntoMe(pointRecorder mom, int nPointsResample){

    gesturePoint	  lower;
    gesturePoint	  upper;	
    int 		  nResampledPoints = nPointsResample;
    int 		  nPts = mom.pointCounter;
    int 	          nPathPoints = nPts;
    float	          totalPathLength = mom.calculateLength(); 	
    //System.out.println(totalPathLength);
    if (totalPathLength == 0) totalPathLength = 0.0001f;
    float	     newPathLength = 0;
    float	     RSL = totalPathLength / (float)(nResampledPoints-1);
    float	     Dx, Dy, Dz;
    float	     dx, dy, dz;
    float	     px, py, pz; 		
    float	     RSLdx, RSLdy, RSLdz;

    float	     segLength, ASL; // available segment length
    float	     remainder, neededSpace;
    float	     prevRemainder = RSL;
    int 	     nsegs;
    int 	     p=0;
    int 	     i;
    int	             nPtsToDo; 
    int	             nResamples = 0;


    if (nPathPoints <= 1) { // special case for one-point path
      for (p=0; p<nResampledPoints; p++){
        lower =  mom.pos[0]; 				
        px = lower.x + (float)p*0.0001f;
        py = lower.y + (float)p*0.0001f; 				
   
        pos[nResamples].x = px;
        pos[nResamples].y = py;

        nResamples++;
      } 	

    } 
    else { 			
      for (i=0; i< nPathPoints-1; i++){

        lower = mom.pos[i];
        upper = mom.pos[i+1];
        Dx = upper.x - lower.x;
        Dy = upper.y - lower.y; 				
   

        if (Dx == 0 && Dy == 0){ 
          Dx = 0.0001f; 
          Dy = 0.0001f; 
        }
        segLength = (float)Math.sqrt(Dx*Dx + Dy*Dy);
        ASL = segLength;		// available segment length 				
        dx = (Dx/segLength);	// unit vector components
        dy = (Dy/segLength); 				
        

        RSLdx = dx * RSL;		// resampled segment vector components
        RSLdy = dy * RSL;
       

        neededSpace = RSL-prevRemainder;

        if (ASL >= neededSpace){ 

          // if there is enough room to place the first point
          // then place the first resample point in the latest segment
          remainder = ASL;
          px = lower.x + (neededSpace*dx);
          py = lower.y + (neededSpace*dy); 					
         

          if (p < (nResampledPoints-1)){ 
            pos[nResamples].x = px;
            pos[nResamples].y = py;
            nResamples++;
            newPathLength += RSL;
            remainder -= neededSpace;
            p++;
          }

          nPtsToDo = (int)(remainder/RSL);
          for (int d=0; d<nPtsToDo; d++){
            px += RSLdx;  						
            py += RSLdy; 						
    						
            if (p < (nResampledPoints-1)){
              pos[nResamples].x = px;
              pos[nResamples].y = py;
              nResamples++;	
              newPathLength += RSL;
              remainder -= RSL;
              p++;
            }
          }
          prevRemainder = remainder;
        } 
        else { 
          // if there is not enough room to place the first point 
          prevRemainder += ASL; 				
        } 			
      }
      upper = mom.pos[nPts-1]; 	


      while(nResamples < nPointsResample){
        pos[nResamples].x = upper.x;
        pos[nResamples].y = upper.y;
        nResamples++;	
      }
    }


    pointCounter = nResamples; 
  }









}



