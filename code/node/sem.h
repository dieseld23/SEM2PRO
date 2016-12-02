#include <iostream>
#include <vector>
using namespace std;


class Sensor {
  int 	flag;
  char	time[10];
  char 	status;
  char	latitude[11];
  char 	latitude_dir;
  char	longitude[12];
  char	longitude_dir;
  char 	date[7];
public:
  void get_data();
  void set_dummy_data();
};

class Node {
  int nodeid[4];
  vector<int> messagetypes;
  Sensor node_sensor;
public:
  Node(int id[4], vector<int> types, Sensor sensor);
  void print_nodeid(void);
  void print_messagetypes(void);
  void set_send_message(void (*f)(int));
  void (*send_message)(int message_type);
};
