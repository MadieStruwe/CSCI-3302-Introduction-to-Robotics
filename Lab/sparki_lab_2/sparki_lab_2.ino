#include<Sparki.h>

unsigned long time;
float distance=.278;
float pose_x=0;
float pose_y=0;
float pose_theta=0;
//from line following code
const int threshold = 700;
int line_left = 1000;
int line_right = 1000;
int line_center = 1000;

void setup() 
{
  pose_x = 0;
  pose_y = 0;
  pose_theta = 0;
}

void sensors()
{
  line_left = sparki.lineLeft();
  line_right = sparki.lineRight();
  line_center = sparki.lineCenter();
}

//most of the code is from the line following code from the sparki websight
//our group added some things for the lab requirments
void loop() {
  time = millis();
  sensors();
  sparki.clearLCD(); // wipe the screen

//reset the poses to 0 when sparki crosses start line
  if (line_center > threshold && line_left > threshold && line_right > threshold)
  {
    pose_x = pose_y = pose_theta = 0;
  }
  else if ( line_center < threshold ) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
    //calculate the x and y pose
    pose_x = pose_x + (distance * cos(pose_theta));
    pose_y = pose_y + (distance * sin(pose_theta));
  }
  else{
    if ( line_left < threshold ) // if line is below left line sensor
    {  
      sparki.moveLeft(); // turn left
      //calculate the theta pose
      pose_theta = pose_theta + 0.0654;
    }
  
    if ( line_right < threshold ) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
      //calculate the theta pose
      pose_theta = pose_theta - 0.0654;
    }
  }
  
  sparki.print("Pose X: "); // show left line sensor on screen
  sparki.println(pose_x);
  
  sparki.print("Pose Y: "); // show center line sensor on screen
  sparki.println(pose_y);
  
  sparki.print("Pose Theta: "); // show right line sensor on screen
  sparki.println(pose_theta);
 
  delay(88); //delay set to 88ms so when the program is run,
  //each loop should take 100ms to complete
  time = millis() - time;
  sparki.print("Time: ");
  sparki.println(time);
  
  sparki.updateLCD(); // display all of the information written to the screen

}
