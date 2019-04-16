#include <Sparki.h> // include the sparki library

const int threshold = 700;
int line_left = 1000;
int line_right = 1000;
int line_center = 1000;

float pose_x = 0, pose_y = 0, pose_theta = 0;
float pi = 3.1415926;
unsigned long time;
unsigned long remTime;
float looptime = 150;
float distance;
float obj_x;
float obj_y;
float rem_time;
int cm_distance;
int x_coord;
int y_coord;
int index;
const int map_width = 12;
const int map_height = 8;
bool world_map[map_width][map_height];

void setup() 
{
  pose_x = 43; //set the x,y, and theta poses to 0
  pose_y = 0;
  pose_theta = 0;
  sparki.servo(-90); //turn sparki head to the left
  for (int i = 0; i<map_width ; i++) //populate array with zeros
    for (int j = 0; j<map_height; j++)
      world_map[i][j] = 0;
      
}

void sensors()
{
  line_left = sparki.lineLeft();
  line_right = sparki.lineRight();
  line_center = sparki.lineCenter();
  cm_distance = sparki.ping();
}

int coordToIndex(int i, int j)
{
  int index = ((i-1) * map_height)+j; //index increases down the first column and continues at the top of the next column anticipated starting in the top left corner
  return index;
}

struct ITOC
{
  int itoc_y;
  int itoc_x;
};
struct ITOC funct();
struct ITOC funct()
{
  struct ITOC result;
  result.itoc_x = (index / map_height); // if division returns zero, it's the first column.  Needs checking for rounding down in C
  result.itoc_y = (index % map_height); //if modulo zero, coordinate is in first row
  return result;
}


int indexToCoord(int index)
{
  int itoc_y = (index % map_height)+1; //if modulo zero, coordinate is in first row
  int itoc_x = (index / map_height)+1; // if division returns zero, it's the first column.  Needs checking for rounding down in C
  return itoc_x , itoc_y; // this doesn't work in C
}
/*
void printMap(world_map)
{
  for (int i = 0, i < map_width, i++)
    for (int j = 0, j < map_height, j++)
      if (world_map[i][j] == 1)
        sparki.drawCircle (8*i, 8*j, 4);
}
*/
void loop() {
  time = millis();
  sensors();
  distance = .00278 * looptime;
  sparki.clearLCD(); // wipe the screen
  obj_x = 0;
  obj_y = 0;
  if (cm_distance > 0 && cm_distance < 20 )
  {
    obj_x = pose_x*(cos(pose_theta)) - pose_y*(sin(pose_theta)); //calculare the x position of object from sparki
    obj_y = pose_x*(sin(pose_theta)) - pose_y*(cos(pose_theta)) + cm_distance; //calculate the y position of the object from sparki
    x_coord = obj_x/map_width; //using approx 8 cm grid x oriented along long axis of paper
    y_coord = obj_y/map_height;  //using approx 8 cm grid y oriented along short axis of paper
    world_map[x_coord][y_coord] = 1;  // 0 means no object/ 1 means object
 /*   if (obj_x !=0 && obj_y !=0)
    {
      sparki.drawCircle (8*x_coord, 8*y_coord, 4);
    }*/
  }
  if (line_center < threshold && line_left < threshold && line_right < threshold)
  {
    pose_x = 43; // assuming the start line is in the lower right corner of the map
    pose_y = pose_theta = 0;
  }
  else if ( line_center < threshold ) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
    pose_x = pose_x + (distance * cos(pose_theta)); //calculate the pose of x and y
    pose_y = pose_y + (distance * sin(pose_theta)); //sparkis current location
  }
  else{
    if ( line_left < threshold ) // if line is below left line sensor
    {  
      sparki.moveLeft(); // turn left
      pose_theta = pose_theta + (0.000654 * looptime); //calculate the pose theta, sparkis current angle
    }
  
    if ( line_right < threshold ) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
      pose_theta = pose_theta - (0.000654 * looptime); //calculate the pose theta, sparkis current angle
    }
  }
  //printMap();
   for (int i = 0; i < map_width; i++){
    for (int j = 0; j < map_height; j++){
      if (world_map[i][j] == 1)
        sparki.drawCircle (8*i, 8*j, 4);
    }
   }

/* 
  sparki.print("Object Distance: "); // show ping response on screen
  sparki.println(cm_distance);
  
  sparki.print("Object X: "); // show object X location on screen
  sparki.println(obj_x);

  sparki.print("Object Y: "); // show object Y location on screen
  sparki.println(obj_y);

  sparki.print("Position X: "); // show Sparki X position on screen
  sparki.println(pose_x);

  sparki.print("Position Y: "); // show Sparki Y position on screen
  sparki.println(pose_y);
  sparki.print("Theta: "); // show Sparki Heading on screen
  sparki.println(pose_theta);  //shown in radians
*/

  sparki.updateLCD(); // display all of the information written to the screen
  rem_time = looptime - (millis()-time);
  if (rem_time<1)
  {
    rem_time=1;
    sparki.RGB(0,100,50);
  }
  else
    sparki.RGB(100,0,0);
  delay(rem_time);

}
