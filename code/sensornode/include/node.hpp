#pragma once

#include <thread>  
#include <iostream>
#include <vector>
#include <bitset>     
#include <mutex>


struct packed_data {
  std::vector<bool> messagetype;
  std::vector<bool> data;
};

class Node {
private:
  long int ms;
  std::bitset<4> node_id;
  std::thread node_thread;
  std::vector<packed_data> data_out;
  std::mutex data_out_mutex;
  void print_vector_bool(std::vector<bool> vector);
  int get_data_from_buffer(packed_data* data_packet);
public:
 Node(void);
 Node(int id);
 void start(void);
 void loop(void);
 void put_data_packet(struct packed_data);
};