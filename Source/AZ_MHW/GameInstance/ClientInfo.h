#pragma once
#include "Define.h"
#include <stdio.h>
#include <mutex>
#include <queue>

class ClientInfo
{
public:
	ClientInfo()
	{
		ZeroMemory(&recv_overlapped_ex_, sizeof(stOverlappedEx));
		socket_ = INVALID_SOCKET;
	}

	void Init(const UINT32 index, HANDLE iocp_handle)
	{
		index_ = index;
		iocp_handle_ = iocp_handle;
	}

	UINT32 GetIndex() { return index_; }

	bool IsConnected() { return is_connect_ == 1; }

	SOCKET GetSock() { return socket_; }

	UINT64 GetLatestClosedTimeSec() { return latest_closed_time_sec_; }

	char* GetRecvBuffer() { return recv_buf_; }

	bool OnConnect(HANDLE iocpHandle, SOCKET socket)
	{
		socket_ = socket;
		is_connect_ = 1;

		Clear();

		// Ŭ���̾�Ʈ�� �����ϸ� listen_socket_ -> ReadyAccept()���� Socket���� ����
		if (SetIocpBindSocket(iocpHandle) == false)
		{
			return false;
		}

		return BindRecv();
	}

	void Close(bool bIsForce = false)
	{
		struct linger stLinger = { 0,0 }; // SO_DONTLINGER�� ����

		// bIsForce�� true�̸� SO_LINGER, timeout = 0���� �����Ͽ� ���� ���� ��Ų��. ���� : ������ �ս��� ������ ���� 
		if (true == bIsForce)
		{
			stLinger.l_onoff = 1;
		}

		//socketClose������ ������ �ۼ����� ��� �ߴ� ��Ų��.
		shutdown(socket_, SD_BOTH);

		//���� �ɼ��� �����Ѵ�.
		setsockopt(socket_, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

		is_connect_ = 0;
		latest_closed_time_sec_ = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		//���� ������ ���� ��Ų��. 
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}

	void Clear()
	{
	}

	// �񵿱� accept ���� ����
	bool ReadyAccept(SOCKET listen_socket, const UINT64 cur_time_sec)
	{
		//printf_s("ReadyAccept. client Index: %d\n", GetIndex());
		UE_LOG(LogTemp, Warning, TEXT("ReadyAccept. client Index: %d\n"), GetIndex());

		latest_closed_time_sec_ = UINT32_MAX;

		// socket_ = INVALID_SOCKET ���� ���ͳ� ���� ������ �Ҵ�
		socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == socket_)
		{
			//printf_s("client Socket WSASocket Error : %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("client Socket WSASocket Error : %d\n"), GetLastError());
			return false;
		}

		ZeroMemory(&accept_overlapped_ex_, sizeof(stOverlappedEx));

		DWORD bytes = 0;
		DWORD flags = 0;
		accept_overlapped_ex_.wsa_buf_.len = 0;
		accept_overlapped_ex_.wsa_buf_.buf = nullptr;
		accept_overlapped_ex_.E_operation_ = IOOperation::ACCEPT;
		accept_overlapped_ex_.session_index_ = index_;

		// AcceptEx : �񵿱� accept
		// https://snowfleur.tistory.com/116
		if (FALSE == AcceptEx(
			listen_socket, // ���� ���ø����̼��� �� ���Ͽ��� ������ �õ� �Ҷ����� ��ٸ�
			socket_, // ������ ������ ������ ����
			accept_buf_,  // �� ���ῡ�� ���� ù ��° ������ ���, ������ ���� �ּ� �� Ŭ���̾�Ʈ�� ���� �ּҸ� �����ϴ� ���ۿ� ���� ������
			0, // 0�̸� ���� �۾��� �߻����� �ʰ� ���� �ϴ� ��� �Ϸ�
			sizeof(SOCKADDR_IN) + 16, // ���� ���� �ּ� ���� ����� ����Ʈ �� ���� �������ݺ��� 16 �� ������
			sizeof(SOCKADDR_IN) + 16,  // ���� �ּ� ������ ���� ����� ����Ʈ �� ���� �������ݺ��� 16 �� ������
			&bytes, (LPWSAOVERLAPPED) & (accept_overlapped_ex_))) // ��ûó���� ���Ǵ� OVERLAPPED ����ü (NULL�ϼ��� ����)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				//printf_s("AcceptEx Error : %d\n", GetLastError());
				UE_LOG(LogTemp, Warning, TEXT("AcceptEx Error : %d\n"), GetLastError());
				return false;
			}
		}

		return true;
	}

	// accept �Ϸ� �Լ�
	bool AcceptCompletion()
	{
		//printf_s("AcceptCompletion : SessionIndex(%d)\n", index_);
		UE_LOG(LogTemp, Warning, TEXT("AcceptCompletion : SessionIndex(%d)\n"), index_);

		if (OnConnect(iocp_handle_, socket_) == false)
		{
			return false;
		}

		SOCKADDR_IN st_client_addr;
		int addr_len = sizeof(SOCKADDR_IN);
		char client_ip[32] = { 0, };
		inet_ntop(AF_INET, &(st_client_addr.sin_addr), client_ip, 32 - 1);
		//printf("Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)\n", client_ip, (int)socket_);
		UE_LOG(LogTemp, Warning, TEXT("Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)\n"), client_ip, (int)socket_);

		return true;
	}

	bool SetIocpBindSocket(HANDLE iocpHandle)
	{
		// listen_socket_ -> ReadyAccept()���� ������ �������� ����
		auto H_iocp = CreateIoCompletionPort((HANDLE)GetSock(),
			iocpHandle, // ExistingCompletionPort : ���� �Ǵ� ���ϰ� ������ ����� �Ϸ� ��Ʈ
			(ULONG_PTR)(this), // CompletionKey�� � ������ I/O�� �ߴ��� �� �� ����
			0);

		if (H_iocp == INVALID_HANDLE_VALUE)
		{
			//printf("[����] CreateIoCompletionPort()�Լ� ����: %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[����] CreateIoCompletionPort()�Լ� ����: %d\n"), GetLastError());
			return false;
		}
		return true;
	}

	bool BindRecv()
	{
		DWORD dwFlag = 0;
		DWORD dwRecvNumBytes = 0;

		recv_overlapped_ex_.wsa_buf_.len = MAX_SOCKBUF;
		recv_overlapped_ex_.wsa_buf_.buf = recv_buf_;
		recv_overlapped_ex_.E_operation_ = IOOperation::RECV;

		int nRet = WSARecv(socket_,
			&(recv_overlapped_ex_.wsa_buf_),
			1, &dwRecvNumBytes, &dwFlag,
			(LPWSAOVERLAPPED) & (recv_overlapped_ex_),
			NULL);

		// socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
		if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			//printf("[����] WSARecv()�Լ� ���� : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[����] WSARecv()�Լ� ���� : %d\n"), WSAGetLastError());
			return false;
		}

		return true;
	}

	bool SendMsg(const UINT32 data_size_, char* P_msg_)
	{
		Defind defind;
		auto P_login_req_packet = reinterpret_cast<Test_Login_Send_Packet*>(P_msg_);
		UE_LOG(LogTemp, Warning, TEXT("[SendMsg] packet_id : %d, packet_length : %d packet_data %s\n"),
			P_login_req_packet->packet_id, P_login_req_packet->packet_length, *defind.CharArrayToFString(P_login_req_packet->user_id));
		P_login_req_packet->user_id;
		// send_overlapped ����ü ����
		auto send_overlapped_ex = new stOverlappedEx;
		ZeroMemory(send_overlapped_ex, sizeof(stOverlappedEx));
		send_overlapped_ex->wsa_buf_.len = data_size_;
		// Send ���� ������ �����ŭ buf ���� �Ҵ�
		send_overlapped_ex->wsa_buf_.buf = new char[data_size_];
		CopyMemory(send_overlapped_ex->wsa_buf_.buf, P_msg_, data_size_);
		// send_overlapped ���� �˷��ֱ� 
		send_overlapped_ex->E_operation_ = IOOperation::SEND;

		std::lock_guard<std::mutex> guard(send_lock_);

		// send queue�� ������ �о� �ֱ�
		send_data_queue_.push(send_overlapped_ex);

		// ���� ť�� �Ѱ� �ۿ� ������ �����ص� �ǰ� 2�� �̻��̸� ���� �������� WSASend�� �ִ�
		if (send_data_queue_.size() == 1)
		{
			SendIO();
		}

		return true;
	}

	void SendCompleted(const UINT32 dataSize_)
	{
		//printf("[�۽� �Ϸ�] bytes : %d\n", dataSize_);
		UE_LOG(LogTemp, Warning, TEXT("[�۽� �Ϸ�] bytes : %d\n"), dataSize_);

		std::lock_guard<std::mutex> guard(send_lock_);

		// ť�� ���� �� �迭 ����
		delete[] send_data_queue_.front()->wsa_buf_.buf;
		// ť ���� �� ����
		delete send_data_queue_.front();

		// ť ���� �� ��������
		send_data_queue_.pop();

		// ť�� ��� �ִ� ���°� �ƴϸ� �� �������� �ٽ� wsasend
		if (send_data_queue_.empty() == false)
		{
			SendIO();
		}
	}

private:
	bool SendIO()
	{
		// ť�� ���� �պκ� stOverlappedEx�� ��ü�� ���
		auto send_overlapped_ex = send_data_queue_.front();

		DWORD dwRecvNumBytes = 0;
		int nRet = WSASend(
			socket_, // �񵿱� ����� ����
			&(send_overlapped_ex->wsa_buf_), // WSABUF ����ü �迭 �ּ�
			1, // WSABUF ����ü ����
			&dwRecvNumBytes, // �Լ� ȣ���� �����ϸ� �� ������ �����ų� ���� ����Ʈ ���� ����
			0, // send()�� recv()�Լ��� ������ ���ڿ� ������ ����
			(LPWSAOVERLAPPED)send_overlapped_ex, // WSAOVERLAPPED ����ü ���� �ּҰ�
			NULL); // ����� �۾��� �Ϸ�Ǹ� �ü���� �ڵ����� ȣ���� �Ϸ��ƾ �ּ� ��

		//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
		if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			//printf("[����] WSASend()�Լ� ���� : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[����] WSASend()�Լ� ���� : %d\n"), WSAGetLastError());
			return false;
		}
		return true;
	} // SendIO End

	bool SetSocketOption()
	{
		int opt = 1;
		if (SOCKET_ERROR == setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt,
			sizeof(int)))
		{
			//printf_s("[DEBUG] TCP_NODELAY error: %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[DEBUG] TCP_NODELAY error : % d\n"), GetLastError());
			return false;
		}

		opt = 0;
		if (SOCKET_ERROR == setsockopt(socket_, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
		{
			//printf_s("[DEBUG] SO_RCVBUF change error: %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[DEBUG] SO_RCVBUF change error: %d\n"), GetLastError());
			return false;
		}

		return true;
	}

	INT32 index_ = 0;
	HANDLE iocp_handle_ = INVALID_HANDLE_VALUE;

	INT64 is_connect_ = 0;
	UINT64 latest_closed_time_sec_ = 0;


	SOCKET socket_; //Cliet�� ����Ǵ� ����

	// Accept
	stOverlappedEx accept_overlapped_ex_;
	char accept_buf_[64];

	// Recv
	stOverlappedEx recv_overlapped_ex_; //RECV Overlapped I/O�۾��� ���� ����
	char recv_buf_[MAX_SOCKBUF];  //������ ����

	// Send (queue ����)
	std::mutex send_lock_;
	std::queue<stOverlappedEx*> send_data_queue_;
};

