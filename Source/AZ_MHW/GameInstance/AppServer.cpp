#include "AppServer.h"

void AppServer::ServerStart()
{
	server_check_ = true;

	Init(MAX_IO_WORKER_THREAD);

	//소켓과 서버 주소를 연결하고 등록 시킨다.
	BindAndListen(SERVER_PORT);

	Run(MAX_CLIENT);
}
