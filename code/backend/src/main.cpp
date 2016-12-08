#include "interpreter.hpp"
#include "fifo.hpp"
#include "backend.hpp"
#include "ui.hpp"
#include "receiver.hpp"

int main(int argc, char const *argv[])
{
	fifo fifo_o;
	interpreter interpreter_o(&fifo_o);
	receiver receiver_o(&interpreter_o);
	backend backend_o(&fifo_o);
	ui ui_o(&backend_o);

	while(1);

	return 0;
}