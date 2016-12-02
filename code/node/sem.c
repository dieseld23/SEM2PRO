#include "sem.h"

char test[14] =  "5522.03848, N";

Node::Node(int id[4], vector<int> types, Sensor sensor){
	for(int i=0; i<4; i++){
		nodeid[i] = id[i];
	}

	messagetypes = types;
	node_sensor = sensor;
}
void Node::print_nodeid(){
	cout << "Nodeid: ";
	for(int i=0; i<4; i++){
		cout << nodeid[i] ;
	}
	cout << "\n";
}

void Node::print_messagetypes(){
	cout << "Messagetypes contains:";
	for (vector<int>::iterator it = messagetypes.begin() ; it != messagetypes.end(); ++it){
		cout << ' ' << *it;
	}
	cout << '\n';
}

void Node::set_send_message(void (*f)(int)){
	send_message = f;
}



void Sensor::set_dummy_data(){
	flag = 1;
}


void my_int_func(int x)
{
	cout << x*2 <<". \n";
}


int main()
{
	int id[4] = {1,0,1,0};
	int int_types[]={13,12,11,1,2};
	vector<int> types (int_types, int_types + sizeof(int_types) / sizeof(int) );			//Make vector with int_types values
	Sensor gps_sensor;


	Rectangle rectangle1;

	//Node mynode(id,types,gps_sensor);

	//mynode.print_nodeid();
	//mynode.print_messagetypes();

	void (*foo)(int);
	foo = &my_int_func;

	//mynode.set_send_message(my_int_func);

	//mynode.send_message(10);

	return 0;
}