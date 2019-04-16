#include<Sparki.h>
#include<math.h>


double pi =3.1416;
unsigned long time;
float distance=.278;
float pose_x=0;
float pose_y=0;
float pose_theta=0;

double x_robot=0; //current x coordinate of sparki
double y_robot=0; //current y coordinate of sparki
double x_goal=30; //x coordinate of the goal
double y_goal=10; //y coordinate of the goal
double theta_goal=0; //theta goal

double a_1;

int ccw = -1;
int cw = 1;
int q;


void setup() 
{
  pose_x = 0;
  pose_y = 0;
  pose_theta = 0;

double x_robot=0; //current x coordinate of sparki
double y_robot=0; //current y coordinate of sparki
double x_goal=30; //x coordinate of the goal
double y_goal=10; //y coordinate of the goal
double theta_goal=0; //theta goal

double a_1;
}

//most of the code is from the line following code from the sparki websight
//our group added some things for the lab requirments
void loop() 
{

  time = millis();
  sparki.clearLCD(); // wipe the screen
//part 2
//1, where we are - where we are going
double p; //eucliadian distance between current location and the goal position
p=sqrt(pow((x_robot-x_goal),2)+pow((y_robot-y_goal),2));
//2, heading errrrrr
//double a = pose_theta - atan2(y_robot-y_goal, x_robot-x_goal);
double a=pose_theta-atan2(y_goal, x_goal);
if (a>0)
{
  a_1=a*(180/pi); //convert to degrees
  
 // sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 100);
  //sparki.motorRotate(MOTOR_RIGHT, DIR_CCW, 100);
  //pose_theta = pose_theta - 0.0654;
 // sparki.moveRight(a_1);
 q = ccw;
  //pose_theta=a;
}
else 
{
  a_1=a*(180/pi); //convert to degree
 // sparki.moveLeft(a_1);
 q = cw;
  //pose_theta=a;
}
//sparki.moveForward(p);
sparki.motorRotate(MOTOR_LEFT, ccw, 100);
sparki.motorRotate(MOTOR_RIGHT, cw, 100);
pose_x = pose_x + (distance * cos(pose_theta));
pose_y = pose_y + (distance * sin(pose_theta));
delay(100);
sparki.moveStop();
sparki.motorRotate(MOTOR_LEFT, q, 100);
sparki.motorRotate(MOTOR_RIGHT, q, 100);
if (q == ccw)
  pose_theta = pose_theta - 0.0654;
else if (q == cw)
 pose_theta = pose_theta +0.0654;
delay(100);
sparki.moveStop();
//x_robot=x_goal;
//y_robot=y_goal;
//3, bearing errrr
double n=pose_theta-theta_goal;
if (n<0)
{
  n=n*(180/pi); //convert to degrees
 // sparki.moveRight(n);
 // pose_theta=theta_goal;
}
else 
{
  n=n*(180/pi); //conver to degree
 // sparki.moveLeft(n);
 // pose_theta=theta_goal;
}


sparki.print ("a");
sparki.println(a);

sparki.print("n");
sparki.print(n);
  
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
