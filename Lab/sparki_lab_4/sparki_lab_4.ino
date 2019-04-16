#include <Sparki.h> // include the sparki library
#include <math.h> //include math library

const int threshold = 700;
int line_left = 1000;
int line_right = 1000;
int line_center = 1000;
float obj_x;
float obj_y;


float pose_x = 0, pose_y = 0, pose_theta = 0;

unsigned long time;
float distance = .278;

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

void loop() {
  time = millis();
  sparki.servo(SERVO_CENTER); // Center the Servo
  sensors();
  sparki.clearLCD(); // wipe the screen

  int cm_distance = sparki.ping();
  sparki.print("Distance": );
  sparki.print(cm_distance);
  sparki.println(" cm");
  obj_x = pose_x*(cos(pose_theta)) - pose_y*(sin(pose_theta)) + cm_distance;
  obj_y = pose_x*(sin(pose_theta)) + pose_y*(cos(pose_theta)) + 0;

  if (line_center > threshold && line_left > threshold && line_right > threshold)
  {
    pose_x = pose_y = pose_theta = 0;
  }
  else if ( line_center < threshold ) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
    pose_x = pose_x + (distance * cos(pose_theta));
    pose_y = pose_y + (distance * sin(pose_theta));
  }
  else{
    if ( line_left < threshold ) // if line is below left line sensor
    {  
      sparki.moveLeft(); // turn left
      pose_theta = pose_theta + 0.0654;
    }
  
    if ( line_right < threshold ) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
      pose_theta = pose_theta - 0.0654;
    }
  }
  
  
  sparki.print("Pose X: "); // show left line sensor on screen
  sparki.println(pose_x);
  
  sparki.print("Pose Y: "); // show center line sensor on screen
  sparki.println(pose_y);
  
  sparki.print("Pose Theta: "); // show right line sensor on screen
  sparki.println(pose_theta);
 
  delay(88);
  time = millis() - time;
  sparki.print("Time: ");
  sparki.println(time);
  
  sparki.updateLCD(); // display all of the information written to the screen

}
