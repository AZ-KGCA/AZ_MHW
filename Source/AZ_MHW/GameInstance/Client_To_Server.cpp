// Fill out your copyright notice in the Description page of Project Settings.


#include "Client_To_Server.h"

UClient_To_Server::UClient_To_Server()
{

}

void UClient_To_Server::Socket_Init()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    /*----------------------
    SOCKET ����
    -----------------------*/
    sock = socket(AF_INET, SOCK_STREAM, 0);

    short sData = 10000;
    short tData = 0x2710;
    short fData = 0x1027;

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("192.168.0.157");
    sa.sin_port = htons(10000);
}

void UClient_To_Server::Server_Connect()
{
    /*----------------------
        SOCKET ����
        -----------------------*/
    int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa));

    u_long iMode = 1;
    /*----------------------�𸮾� ���� vector deleting destructor'() ���� ������ ����?
    SOCKET ���ŷ ����  | ioctlsocket
    -----------------------*/
    ioctlsocket(sock, FIONBIO, &iMode);

    rece_thread = std::thread(&UClient_To_Server::receive_thread, this);
    rece_queue_thread = std::thread(&UClient_To_Server::receive_data_read_thread, this);

    client_check = true;
}

void UClient_To_Server::Client_Shutdown()
{
    UE_LOG(LogTemp, Warning, TEXT("[Client_Shutdown]\n"));
    recevie_connected = false;

    rece_thread.join();
    rece_queue_thread.join();

    closesocket(sock);
    WSACleanup();
}

void UClient_To_Server::Server_Packet_Send(const char* packet, int packet_size)
{
    UE_LOG(LogTemp, Warning, TEXT("[Server_Packet_Send] sendData : %s size : %d\n"), packet, packet_size);

    send(sock, packet, packet_size, 0);
}

void UClient_To_Server::Signin()
{
    send(sock, (char*)&signin_packet, sizeof(signin_packet), 0);
}

void UClient_To_Server::receive_thread()
{
    char buffer[1024];
    int result;

    while (recevie_connected)
    {
        result = recv(sock, buffer, sizeof(buffer), 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // TODO �̰����� ������ ���� �޴°� Ȯ���ϱ�

        if (result > 0)
        {
            Login_Send_Packet Login_data;
            ZeroMemory(&Login_data, sizeof(Login_data));
            //CopyMemory(&Login_data, (Login_Send_Packet*)buffer, sizeof((Login_Send_Packet*)buffer));
            CopyMemory(&Login_data, buffer, sizeof(Login_data));

            UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_thread] packet id : %d data : %s\n"), Login_data.packet_id, *defind.CharArrayToFString(Login_data.user_id));
            // TODO ���⼭ ����� ���� �б��ؾ��ҵ�?

            std::lock_guard<std::mutex> lock(received_data_mutex);

            // ���� ������ ť�� �о� �ֱ�
            receive_header_check_data_queue.push(&Login_data);

            // ReceivedData ����ü�� ����� ���� �����͸� �����մϴ�.
            //ReceivedData received_data = { new char[result], result };
            //std::copy(buffer, buffer + result, received_data.buffer);

            //// Acquire the lock to access the queue
            //std::lock_guard<std::mutex> lock(received_data_mutex);

            //// Store the received data in the queue
            //received_data_queue.push(received_data);

        }
    }
}

void UClient_To_Server::receive_data_read_thread()
{
    while (recevie_connected)
    {
        // ��⿭�� �׼����ϱ� ���� ��� ȹ��
        std::lock_guard<std::mutex> lock(received_data_mutex);

        // ť�� ���� �����Ͱ� �ִ��� Ȯ��
        if (!receive_header_check_data_queue.empty()) {
            // ��⿭���� ó�� ���� ������ ��������
            Login_Send_Packet* received_data = receive_header_check_data_queue.front();

            switch (received_data->packet_id)
            {
            case (UINT32)CLIENT_PACKET_ID::LOGIN_RESPONSE_SUCCESS:
                ::MessageBox(NULL, L"Signin_Success", L"SignIn", 0);
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 201] packet id : %d\n"), received_data->packet_id);
                // ĳ���� ����â ��������Ʈ �޾�������� �ΰ��� �������� �ϴ� ����
                if (Fuc_in_game_connect.IsBound() == true) Fuc_in_game_connect.Execute();
                break;
            case (UINT32)CLIENT_PACKET_ID::LOGIN_RESPONSE_FAIL:
                ::MessageBox(NULL, L"Signin_Fail", L"SignIn", 0);
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 202] packet id : %d\n"), received_data->packet_id);
                break;
            case (UINT32)CLIENT_PACKET_ID::SIGNIN_RESPONSE_SUCCESS:
                ::MessageBox(NULL, L"Signup_Success", L"Signup", 0);
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 203] packet id : %d\n"), received_data->packet_id);
                break;
            case (UINT32)CLIENT_PACKET_ID::SIGNIN_RESPONSE_FAIL:
                ::MessageBox(NULL, L"Signup_Fail", L"Signup", 0);
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 204] packet id : %d\n"), received_data->packet_id);
                break;
            case (UINT32)CLIENT_PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS:
                ::MessageBox(NULL, L"BroadCast Msg", L"Signup", 0);
                // TODO ���⿡ ��������Ʈ �޾Ƽ� ê �޼��� ���°� Ȯ���ϱ�
                //FString min = *defind.CharArrayToFString(received_data->user_id);                       
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 302] packet id : %d Data : %s\n"), received_data->packet_id, *defind.CharArrayToFString(received_data->user_id));
                if (Fuc_boradcast_success.IsBound() == true) Fuc_boradcast_success.Execute(*defind.CharArrayToFString(received_data->user_id));
                break;
            default:
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch default] packet id : %d\n"), received_data->packet_id);
                break;
            }

            receive_header_check_data_queue.pop();

            // ���� ������ ���
            // ...
            // TODO ��Ŷ ����� �켱 �ް� �������� �޾�ߵǳ� .......??
        }
    }
}
