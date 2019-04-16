#include <sparki.h>
#include <math.h>
#define TRUE 1
#define FALSE 0
#define GET_NEXT_INSTRUCTION 0
#define STATE_START 1
#define STATE_HAS_PATH 2
#define STATE_SEEKING_POSE 3
#define STATE_PICK_UP_BLOCK 4
#define STATE_DROP_BLOCK 5
#define DEFAULT_STATE 6

int state = GET_NEXT_INSTRUCTION;
int next_i, next_j, theta_i, theta_j;
float next_x, next_y, theta_x, theta_y, theta;

//Bluetooth / Queue variables
byte queue_store;
byte queue_read;
byte queue[5] = {9, 9, 9, 9, 9};
int location[5] = { -1, -1, -1, -1, -1};
int new_theta[5] = {0, 0, 0, 0, 0};

// Robot constants
#define DISTANCE_MARGIN 0.02 // 2cm of tolerance
#define HEADING_MARGIN 0.1  //0.061 // ~3.5 degrees of tolerance

#define ROBOT_SPEED 0.0278
#define CYCLE_TIME .010
#define AXLE_DIAMETER 0.0865
#define M_PI 3.14159
#define WHEEL_RADIUS 0.03
#define FWD 1
#define NONE 0
#define BCK -1

#define INITIAL_GOAL_STATE_I 3
#define INITIAL_GOAL_STATE_J 3


// Number of vertices to discretize the map
#define NUM_X_CELLS 11
#define NUM_Y_CELLS 14
//Size of map is 55 x 70

#define MAP_SIZE_X 0.55
#define MAP_SIZE_Y 0.70

#define BIG_NUMBER 255


// IK/Odometry Variables
float pose_x = 0., pose_y = 0., pose_theta = 0.;
float dest_pose_x = 0., dest_pose_y = 0., dest_pose_theta = 0.;
float d_err = 0., b_err = 0., h_err = 0., phi_l = 0., phi_r = 0.;

// Wheel rotation vars
float left_speed_pct = 0.;
float right_speed_pct = 0.;
int left_dir = DIR_CCW;
int right_dir = DIR_CW;
int left_wheel_rotating = NONE;
int right_wheel_rotating = NONE;


byte goal_i = INITIAL_GOAL_STATE_I;
byte goal_j = INITIAL_GOAL_STATE_J;
byte source_i = 0;
byte source_j = 0;
long program_start_time = 0; // Track time since controller began

void setup() {
  //index = 0;
  // IK Setup
  pose_x = 0.;
  pose_y = 0.;
  pose_theta = 0.;
  left_wheel_rotating = NONE;
  right_wheel_rotating = NONE;

  //set_pose_destination(0.1, 0.6, 0);

  program_start_time = millis();

  // Bluetooth/ queue setup
  Serial1.begin(9600);
  queue_store = 0;
  queue_read = 0;
}

/*****************************
   IK Helper Functions
 *****************************/
void set_pose_destination(float x, float y, float t) {
  dest_pose_x = x;
  dest_pose_y = y;
  dest_pose_theta = t;
  if (dest_pose_theta > M_PI) dest_pose_theta -= 2 * M_PI;
  if (dest_pose_theta < -M_PI) dest_pose_theta += 2 * M_PI;
}

float to_radians(double deg) {
  return  deg * 3.1415 / 180.;
}

float to_degrees(double rad) {
  return  rad * 180 / 3.1415;
}

void updateOdometry() {
  pose_x += cos(pose_theta) * CYCLE_TIME * ROBOT_SPEED
            * (float(left_wheel_rotating) * left_speed_pct
               + float(right_wheel_rotating) * right_speed_pct) / 2.;
  pose_y += sin(pose_theta) * CYCLE_TIME * ROBOT_SPEED
            * (float(left_wheel_rotating) * left_speed_pct
               + float(right_wheel_rotating) * right_speed_pct) / 2.;
  pose_theta += (float(right_wheel_rotating) * right_speed_pct
                 - float(left_wheel_rotating) * left_speed_pct)
                * CYCLE_TIME * ROBOT_SPEED / AXLE_DIAMETER;
  if (pose_theta > M_PI) pose_theta -= 2.*M_PI;
  if (pose_theta <= -M_PI) pose_theta += 2.*M_PI;
}

/*****************************
   Core IK Functions
 *****************************/


void moveStop() {
  left_wheel_rotating = NONE;
  right_wheel_rotating = NONE;
  left_speed_pct = 0.0;
  right_speed_pct = 0.0;
  sparki.moveStop();
}
void compute_IK_errors() {
  // Distance, Bearing, and Heading error
  d_err = sqrt( (dest_pose_x - pose_x) * (dest_pose_x - pose_x) + (dest_pose_y - pose_y) * (dest_pose_y - pose_y) );
  b_err = atan2( (dest_pose_y - pose_y), (dest_pose_x - pose_x) ) - pose_theta;
  h_err = dest_pose_theta - pose_theta;

  if (b_err <= -M_PI) b_err += 2.*M_PI;
  else if (b_err > M_PI) b_err -= 2.*M_PI;

  if (h_err <= -M_PI) h_err += 2.*M_PI;
  else if (h_err > M_PI) h_err -= 2.*M_PI;
}

void compute_IK_wheel_rotations() {
  float dTheta, dX;
  if (d_err > DISTANCE_MARGIN) { // Get reasonably close before considering heading error
    dTheta = b_err;
  } else {
    dTheta = h_err;
    dX = 0.; // Force 0-update for dX since we're supposedly at the goal position anyway
  }

  dX  = 0.1 * min(M_PI, d_err); // De-prioritize distance error to help avoid paths through unintended grid cells

  phi_l = (dX  - (dTheta * AXLE_DIAMETER / 2.)) / WHEEL_RADIUS;
  phi_r = (dX  + (dTheta * AXLE_DIAMETER / 2.)) / WHEEL_RADIUS;
}

void set_IK_motor_rotations() {
  float wheel_rotation_normalizer = max(abs(phi_l), abs(phi_r));
  if (wheel_rotation_normalizer == 0) {
    moveStop();
    return;
  }
  left_speed_pct = abs(phi_l) / wheel_rotation_normalizer;
  right_speed_pct = abs(phi_r) / wheel_rotation_normalizer;

  // Figure out which direction the wheels need to rotate
  if (phi_l <= -to_radians(1)) {
    left_dir = DIR_CW;
    left_wheel_rotating = BCK;
  } else if (phi_l >= to_radians(1)) {
    left_dir = DIR_CCW;
    left_wheel_rotating = FWD;
  } else {
    left_speed_pct = 0.;
    left_wheel_rotating = 0;
  }

  if (phi_r <= -to_radians(1)) {
    right_dir = DIR_CCW;
    right_wheel_rotating = BCK;
  } else if (phi_r >= to_radians(1)) {
    right_dir = DIR_CW;
    right_wheel_rotating = FWD;
  } else {
    right_speed_pct = 0.;
    right_wheel_rotating = 0;
  }

  sparki.motorRotate(MOTOR_LEFT, left_dir, int(left_speed_pct * 100));
  sparki.motorRotate(MOTOR_RIGHT, right_dir, int(right_speed_pct * 100));
}

bool is_robot_at_IK_destination_pose() {
  return (d_err <= DISTANCE_MARGIN && abs(h_err) < HEADING_MARGIN);
}



/*****************************
   Dijkstra Helper Functions
 *****************************/

// Return 1 if there are entries in range [0,inf) in arr
// otherwise return 0, signifying empty queue
bool is_not_empty(short *arr, int len) {
  for (int i = 0; i < len; ++i) {
    if (arr[i] >= 0) {
      return TRUE;
    }
  }
  return FALSE;
}

int get_min_index(short *arr, int len) {
  int min_idx = 0;
  for (int i = 0; i < len; ++i) {
    if (arr[min_idx] < 0 || (arr[i] < arr[min_idx] && arr[i] >= 0)) {
      min_idx = i;
    }
  }
  if (arr[min_idx] == -1) return -1;
  return min_idx;
}


/**********************************
   Coordinate Transform Functions
 **********************************/

// Returns 0 if something went wrong -- assume invalid i and j values being set
bool vertex_index_to_ij_coordinates(int v_idx, int *i, int *j) {
  *i = v_idx % NUM_X_CELLS;
  *j = v_idx / NUM_Y_CELLS;

  if (*i < 0 || *j < 0 || *i >= NUM_X_CELLS || *j >= NUM_Y_CELLS) return FALSE;
  return TRUE;
}

int ij_coordinates_to_vertex_index(byte i, byte j) {
  return j * NUM_X_CELLS + i;
}

// Returns 0 if something went wrong -- assume invalid x and y values are being set
// Returns 1 otherwise. x and y values are the middle of cell (i,j)
bool ij_coordinates_to_xy_coordinates(byte i, byte j, float *x, float *y) {
  if (i < 0 || j < 0 || i >= NUM_X_CELLS || j >= NUM_Y_CELLS) return FALSE;

  *x = (i + 0.5) * (MAP_SIZE_X / NUM_X_CELLS);
  *y = (j + 0.5) * (MAP_SIZE_Y / NUM_Y_CELLS);
  return TRUE;
}

// Returns 0 if something went wrong -- assume invalid x and y values are being set
// Returns 1 otherwise. x and y values are the middle of cell (i,j)
bool xy_coordinates_to_ij_coordinates(float x, float y, byte *i, byte *j) {
  if (x < 0 || y < 0 || x >= MAP_SIZE_X || y >= MAP_SIZE_Y) return FALSE;

  *i = byte((x / MAP_SIZE_X) * NUM_X_CELLS);
  *j = byte((y / MAP_SIZE_Y) * NUM_Y_CELLS);

  return TRUE;
}

/**********************************
        Core Dijkstra Functions
 **********************************/

int counter = 0;
int counter3 = 0;
int counter2 = 0;

void displayOdometry() {
  sparki.clearLCD();
  /*  sparki.print("Messages: ");
    sparki.println(counter);
    sparki.print("Message Loop: ");
    sparki.println(counter3);
    sparki.print("loops: ");
    sparki.println(counter2);
  */
  sparki.print("Queue Execution: ");
  sparki.println(queue_read);
  for (int i = 0; i < 5; i++) //print the queue on the screen port
  {
    sparki.print(i);
    sparki.print(':');
    sparki.print(queue[i]);
    sparki.print("   ");
    sparki.print(location[i]);
    sparki.print("   ");
    sparki.println(new_theta[i]);
  }
  sparki.updateLCD();
}

/**********************************
        Messaging Functions
 **********************************/
void readMessages()
{
  counter3++;
  char locale[4] = {0, 0, 0, '/'};
  char local_theta[4] = {0, 0, 0, '/'};
  if (Serial1.available())
  {
    counter ++;
    sparki.RGB(RGB_GREEN);
    for (byte i = 0; i < 10; i++)
    {
      Serial.print("Message loop: ");
      Serial.println(i);
      if (Serial1.available())
      {
        byte inByte = Serial1.read();
        Serial.print("Message received: ");
        Serial.println(char(inByte));
        if ((char)inByte == 'r') //reset command to empty queue
        {
          moveStop();
          for (byte j = 0; j < 5; j++)
          {
            queue[j] = 9;
            location[j] = -1;
            new_theta[j] = 0;

          }
          queue_store = 0;
          queue_read = 0;
          state = GET_NEXT_INSTRUCTION;
          i = 10;
        }
        if ((char)inByte == 'n')
        {
          Serial.println("new message finished");
          queue_store ++;
          if (queue_store == 5)
            queue_store = 0;
        }
        else
        {
          if (queue[queue_store] != 9 && i == 0)
          {
            Serial.println("Queue is full");
            return;
          }
          else if (i == 0)
          {
            queue[queue_store] = inByte - 48;
            Serial.println("New Command in Queue");
          }
          else if (i == 1 && (char)inByte != ',')
          {
            queue[queue_store] = 9;
            Serial.println("Packet read fail (1)");
          }
          else if (i > 1 && i < 5)
            locale[i - 2] = char(inByte);
          else if (i == 5)
          {
            Serial.print("Locale Var: ");
            Serial.print(locale);
            location[queue_store] = atoi(locale);
            Serial.print("Location Updated: ");
            Serial.println(location[queue_store]);
          }
          else if (i > 5 && i < 9)
          {
            local_theta[i - 6] = (char)inByte;
            if (i == 8)
            {
              new_theta[queue_store] = atoi(local_theta);
              Serial.print("Theta Updated: ");
              Serial.println(new_theta[queue_store]);
            }
          }
        }
      }
    }
  }
}




void loop () {
  unsigned long begin_time = millis();
  unsigned long end_time = 0;

  updateOdometry();
  displayOdometry();
  readMessages();
  counter2 ++;

  switch (state) {
    case GET_NEXT_INSTRUCTION:
      sparki.RGB(RGB_OFF);
      if (queue_read == 5)
        queue_read = 0;
      if (queue[queue_read] != 9)
      {
        if (queue[queue_read] == 1)
          state = STATE_PICK_UP_BLOCK;
        if (queue[queue_read] == 2)
          state = STATE_DROP_BLOCK;
        if (queue[queue_read] == 3)
          state = STATE_HAS_PATH;
      }


    case STATE_START:

      break;

    case STATE_HAS_PATH:
      sparki.RGB(RGB_BLUE);
      updateOdometry();
      displayOdometry();

      {
        if (location[queue_read] != -1)
        {
          vertex_index_to_ij_coordinates(location[queue_read], &next_i, &next_j);
          ij_coordinates_to_xy_coordinates(next_i, next_j, &next_x, &next_y);
          vertex_index_to_ij_coordinates(location[queue_read], &theta_i, &theta_j);
          ij_coordinates_to_xy_coordinates(theta_i, theta_j, &theta_x, &theta_y);
          theta = atan2(theta_y, theta_x);
        }
        else
        {
          theta = 0;
        }
        set_pose_destination(next_x, next_y, theta);
      }

      state = STATE_SEEKING_POSE;
      break;

    case STATE_SEEKING_POSE:
      sparki.RGB(RGB_RED);

      compute_IK_errors();
      compute_IK_wheel_rotations();
      set_IK_motor_rotations();
      if (is_robot_at_IK_destination_pose())
      {
        moveStop();
        queue[queue_read] = 9;
        location[queue_read] = -1;
        queue_read++;
        state = GET_NEXT_INSTRUCTION;

      }
      break;

    case STATE_PICK_UP_BLOCK:
      sparki.RGB(90, 100, 0);
      //pick up block
      sparki.gripperClose(5);
      delay(5000);
      queue[queue_read] = 9;
      location[queue_read] = -1;
      queue_read++;
      state = GET_NEXT_INSTRUCTION;

      break;

    case STATE_DROP_BLOCK:
      sparki.RGB(RGB_WHITE);
      //Drop block on goal
      sparki.gripperOpen(5);
      delay(5000);
      sparki.moveBackward();
      delay(1000);
      sparki.moveStop();
      queue[queue_read] = 9;
      location[queue_read] = -1;
      queue_read++;
      state = GET_NEXT_INSTRUCTION;

      break;

    case DEFAULT_STATE:
      sparki.RGB(100, 0, 100);
      //delete path; path=NULL; // Important! Delete the arrays returned from reconstruct_path when you're done with them!
      moveStop();
      break;
  }

  // Example code to use IK //
  //compute_IK_errors();
  //compute_IK_wheel_rotations();
  //set_IK_motor_rotations();
  //if (is_robot_at_IK_destination_pose()) {
  //moveStop();
  //

  end_time = millis();
  if (end_time - begin_time < 1000 * CYCLE_TIME)
    delay(1000 * CYCLE_TIME - (end_time - begin_time)); // each loop takes CYCLE_TIME ms
  else
    delay(10); // Accept some error
}

