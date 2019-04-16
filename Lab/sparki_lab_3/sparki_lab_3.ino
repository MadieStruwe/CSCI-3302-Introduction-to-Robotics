#include<Sparki.h>
#include<math.h>

double pi =3.1416;
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

double x_robot=0; //current x coordinate of sparki
double y_robot=0; //current y coordinate of sparki
double x_goal=30; //x coordinate of the goal
double y_goal=10; //y coordinate of the goal
double theta_goal=0; //theta goal

double a_1;

void setup() 
{
  sparki.servo(SERVO_CENTER); // Center the Servo
  delay(1000); //1 second
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

void sensors()
{
  line_left = sparki.lineLeft();
  line_right = sparki.lineRight();
  line_center = sparki.lineCenter();
}

void loop() 
{
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
  sparki.moveRight(a_1);
  pose_theta=a;
}
else 
{
  a_1=a*(180/pi); //convert to degree
  sparki.moveLeft(a_1);
  pose_theta=a;
}
sparki.moveForward(p);
x_robot=x_goal;
y_robot=y_goal;
//3, bearing errrr
double n=pose_theta-theta_goal;
if (n<0)
{
  n=n*(180/pi); //convert to degrees
  sparki.moveRight(n);
  pose_theta=theta_goal;
}
else 
{
  n=n*(180/pi); //conver to degree
  sparki.moveLeft(n);
  pose_theta=theta_goal;
}

  time = millis();
  sensors();
  sparki.clearLCD(); // wipe the screen


  sparki.print ("a");
  sparki.println(a);
  
  sparki.print("n");
  sparki.print(n);
  
  sparki.print("Pose X: "); // show left line sensor on screen
  sparki.println(x_robot);
  
  sparki.print("Pose Y: "); // show center line sensor on screen
  sparki.println(y_robot);
  
  sparki.print("Pose Theta: "); // show right line sensor on screen
  sparki.println(pose_theta);
 
  delay(37); //delay set to 88ms so when the program is run,
  //each loop should take 100ms to complete
  time = millis() - time;
  sparki.print("Time: ");
  sparki.println(time);
  
  sparki.updateLCD(); // display all of the information written to the screen

}
