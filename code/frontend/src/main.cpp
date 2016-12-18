#include "interpreter.hpp"
#include "fifo.hpp"
#include "frontend.hpp"
#include "ui.hpp"
#include "receiver.hpp"

int main(int argc, char const *argv[])
{
	fifo fifo_o;
	interpreter interpreter_o(&fifo_o);
	receiver receiver_o(&interpreter_o);
	frontend frontend_o(&fifo_o);
	ui ui_o(&frontend_o);

	while(1);

	return 0;
}