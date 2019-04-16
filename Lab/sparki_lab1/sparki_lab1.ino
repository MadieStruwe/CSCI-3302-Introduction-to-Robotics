#include<sparki.h> //include sparkie library
#define STATE_ROTATE 0
#define STATE_DRIVE 1
#define STATE_GRIP 2
#define STATE_TURN 3
#define STATE_LINE_DETECT 4
#define STATE_ENDING 5

void setup() 
{
  //set up sparki. runs at the beginning only
  sparki.RGB(RGB_RED);
  sparki.servo(SERVO_CENTER); // Center the Servo
  delay(1000); //1 second
  sparki.gripperOpen();//Open the grippers
  delay(5000);//5 seconds
  sparki.gripperStop();//stop opening grippers
  sparki.RGB(RGB_GREEN);
}

  int current_state; //for switch

void loop() 
{
  int cm_distance = sparki.ping();
  int line_left = sparki.lineLeft();
  int line_right = sparki.lineRight();
  int line_center = sparki.lineCenter();
  int threshold = 500;

  
  switch (current_state)
  {
  case 0://rotate
  /*
   * move sparkie in a circle. it will rotate
   * to the right 1 degree at a time, until it
   * detects something
   */
  sparki.moveRight(1);//turn right 1 degree
  if(cm_distance != -1)
  {
    if (cm_distance <= 30)
    {
     /*
     * if object is within 30 cm of sparki,
     * turn light red, beep, then start "drive"
     * to move to the object
     */
      sparki.RGB(RGB_RED);
          //sparki.beep(); 
          current_state = 1; 
    }
  }
  break;
  case 1://drive
  /*
   * move sparki forward to within 7cm of the object
   * then move onto the next state, close grippers
   */
   sparki.moveForward();
   if (cm_distance < 7)
   {
    sparki.moveStop();
    current_state = 2;
   }
  break;
  case 2://grip
  /*
   * grip the object. once sparki is within 7cm of the
   * object, its grippers will close.
   * then move onto next state, turning 180 degrees
   * and drive forward
   */
   sparki.gripperClose();
   delay(7000); //for 7 seconds
   sparki.gripperStop();
   current_state = 3;
  break;
  case 3: //turn
  /*
   * after the grippers are closed, and the object is 
   * "secured" sparki will turn 180 degrees, stop,
   * and drive forward until it detects a line,
   * then it will move to the next state
   * to follow the line
   */
   sparki.moveRight(180);
   sparki.moveStop();
   delay(1000);//1 second delay
   sparki.moveForward();
   current_state = 4;
  break;
  case 4: //line detect
  /*
   * after sparki finds a line, it will follow the line,
   * until it reaches the "start" marker.
   * after it reaches marker, the sparki will move onto
   * the next state to stop moving, beep and
   * drop the object
   */
   //the line folloe code if from the AcBotics websight
   //http://arcbotics.com/lessons/line-following/
  if ( line_left < threshold ) // if line is below left line sensor
  {  
    sparki.moveLeft(); // turn left
  }
  if ( line_right < threshold ) // if line is below right line sensor
  {  
    sparki.moveRight(); // turn right
  }
  // if the center line sensor is the only one reading a line
  if ( (line_center < threshold) && (line_left > threshold) && (line_right > threshold) )
  {
    sparki.moveForward(); // move forward
  }  
  //gotta add somthing to when it gets to start, threshold for all >800?
  //then move to next state?
  if ((line_center>800)&&(line_left>800)&&(line_right>800))
  {
    current_state = 5;
  }
  delay(100); // wait 0.1 seconds
  break;
  case 5: //ending
  /*
   * after sparki reaches the start line, it will 
   * stop, beep, and drop the object.
   */
   sparki.moveStop();
   sparki.beep();
   sparki.gripperOpen();
   delay(7000);//for 7 seconds
  break;

  }
}
