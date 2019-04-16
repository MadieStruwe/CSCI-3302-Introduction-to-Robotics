#include <Sparki.h> // include the sparki library

const int i = 4;
const int j = 4;
int width = 1;
int index = 0;
int index1, index2;
int test_map[4][4];
int dist[i*j];
int prev[i*j];
int source = 0;
int goal = 9;
int result1_x, result1_y, result2_x, result2_y;
int path[i*j];
int path_index = 0;
int min = 99;

void setup() {
  // put your setup code here, to run once:
  for(int x = 0; x < i; x++){
    for(int y = 0; y < j; y++){
      test_map[i][j] = index;
      index = index++;
    }
  }
  index1 = source;
}

struct ITOC
{
  int itoc;
  int itoc_y;
  int itoc_x;
};
struct ITOC funct();
struct ITOC funct()
{
  struct ITOC result;
  result.itoc_x = (index / width); // if division returns zero, it's the first column.  Needs checking for rounding down in C
  result.itoc_y = (index % width); //if modulo zero, coordinate is in first row
  return result;
}
/*
int cost(index1, index2){
  struct ITOC result1;
  struct ITOC result2;
  result1.itoc_x = (index1/width);
  result1.itoc_y = (index1 % width);
  result2.itoc_x = (index2/width);
  result2.itoc_y = (index2 % width);
  if((result1.itoc_x - result2.itoc.x == 1 ) || (result2.itoc_x - result2.itoc.x == 1 ){
    if((result1.itoc_y - result2.itoc.y == 1 ) || (result2.itoc_y - result1.itoc.y == 1 ){
      return 1;
    }
  }
  else {return 99};
}
*/
void loop() {
  // put your main code here, to run repeatedly:
for(index2 = 0; index2 < (i*j); index2++)
{
  result1_x = (index1/i);
  result1_y = (index1 % j);
  result2_x = (index2/i);
  result2_y = (index2 % j);
  if(index1 == index2){dist[index2] = 0;}
  else if((result2_x - result1_x == 1 ) || (result1_x - result2_x == 1 || result2_x - result1_x == 0 ) || (result1_x - result2_x == 0))
  {
    if((result2_y - result1_y == 1 ) || (result1_y - result2_y == 1 ||result2_y - result1_y == 0 ) || (result1_y - result2_y == 0))
    {
      dist[index2] = 1;
      prev[index2] = index1;
    }
    else {dist[index2] = 99;}
  }
  else {dist[index2] = 99;}
}

for (int l = 0; l<(i*j); l++)
{
  if((dist[l] == 1) && l == goal)
  {
    min = goal;
    l = (i*j);
  }
  else if(dist[l] == 1)
  {
    min = l;
  }
}

path[path_index] = index1 = min;
path_index = path_index + 1;
/*
for(int k = 0; k < (i*j); k++)
{
  sparki.clearLCD();
  sparki.println(k);
  sparki.println(": ");
  sparki.println(dist[k]);
  sparki.updateLCD();
  delay(2000);
}*/

for(int k = 0; k < (i*j); k++)
{
  sparki.clearLCD();
  sparki.println("path: ");
  sparki.println(path[k]);
  sparki.updateLCD();
  delay(1000);
}
}
