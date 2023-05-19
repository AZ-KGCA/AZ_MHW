// Fill out your copyright notice in the Description page of Project Settings.


#include "Client_To_Server.h"

UClient_To_Server::UClient_To_Server()
{

}

void UClient_To_Server::Server_Connect()
{
    client_check = true;

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    /*----------------------
    SOCKET 생성
    -----------------------*/
    sock = socket(AF_INET, SOCK_STREAM, 0);

    short sData = 10000;
    short tData = 0x2710;
    short fData = 0x1027;

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");//"192.168.0.157"
    sa.sin_port = htons(10000);

    /*----------------------
        SOCKET 연결
        -----------------------*/
    int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa));

    u_long iMode = 1;
    /*----------------------언리얼 엔진 vector deleting destructor'() 에러 원인이 뭐야 : End시 Thread에 join하고 끝내야함 안하면 이지랄남
    SOCKET 논블럭킹 설정  | ioctlsocket
    -----------------------*/
    ioctlsocket(sock, FIONBIO, &iMode);

    rece_thread = std::thread(&UClient_To_Server::receive_thread, this);
    rece_queue_thread = std::thread(&UClient_To_Server::receive_data_read_thread, this);
    rece_queue_move_info_thread = std::thread(&UClient_To_Server::receive_ingame_moveinfo_data_read_thread, this);
}

void UClient_To_Server::Client_Shutdown()
{
    UE_LOG(LogTemp, Warning, TEXT("[Client_Shutdown]\n"));
    recevie_connected = false;

    rece_thread.join();
    rece_queue_thread.join();
    rece_queue_move_info_thread.join();

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
        // TODO 이곳에서 데이터 전송 받는거 확인하기

        if (result > 0)
        {
            Login_Send_Packet Login_data;
            ZeroMemory(&Login_data, sizeof(Login_data));
            CopyMemory(&Login_data, buffer, sizeof(Login_data));

            UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_thread] packet id : %d data : %s\n"), Login_data.packet_id, *defind.CharArrayToFString(Login_data.user_id));

            // 로그인 채팅 관련 패킷 : 400이하
            if (Login_data.packet_id <= 400)
            {
                std::lock_guard<std::mutex> lock(received_data_mutex);

                // 받은 데이터 큐에 밀어 넣기
                receive_header_check_data_queue.push(&Login_data);
            }
            // 인게임 관련 패킷 400이상
            else
            {
                FSetMoveInfo in_game_move_data_;
                ZeroMemory(&in_game_move_data_, sizeof(in_game_move_data_));
                CopyMemory(&in_game_move_data_, buffer, sizeof(in_game_move_data_));
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_thread else] packet id : %d size : %d fvector : %s frotator : %s\n"),
                    in_game_move_data_.packet_id, in_game_move_data_.packet_length, *in_game_move_data_.fvector_.ToString(), *in_game_move_data_.frotator_.ToString());

                std::lock_guard<std::mutex> lock(received_data_mutex);

                // 받은 데이터 큐에 밀어 넣기
                receive_ingame_moveinfo_data_queue.push(&in_game_move_data_);
            }
        }
    }
}

// 로그인 및 채팅 관련 쓰레드
void UClient_To_Server::receive_data_read_thread()
{
    while (recevie_connected)
    {
        // 대기열에 액세스하기 위한 잠금 획득
        std::lock_guard<std::mutex> lock(received_data_mutex);

        // 큐에 받은 데이터가 있는지 확인
        if (!receive_header_check_data_queue.empty()) {
            // 대기열에서 처음 받은 데이터 가져오기
            Login_Send_Packet* received_data = receive_header_check_data_queue.front();

            switch (received_data->packet_id)
            {
            case (UINT32)CLIENT_PACKET_ID::LOGIN_RESPONSE_SUCCESS:
                ::MessageBox(NULL, L"Signin_Success", L"SignIn", 0);
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 201] packet id : %d\n"), received_data->packet_id);
                // 캐릭터 선택창 델리게이트 달어야하지만 인게임 진입으로 일단 변경
                if(Fuc_in_game_connect.IsBound() == true) Fuc_in_game_connect.Execute();
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
                // TODO 여기에 델리게이트 달아서 챗 메세지 들어온거 확인하기                     
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 302] packet id : %d Data : %s\n"), received_data->packet_id, *defind.CharArrayToFString(received_data->user_id));
                if (Fuc_boradcast_success.IsBound() == true) Fuc_boradcast_success.Execute(*defind.CharArrayToFString(received_data->user_id));
                break;
            case (UINT32)CLIENT_PACKET_ID::IN_GAME_SUCCESS:
                ::MessageBox(NULL, L"IN_GAME_SUCCESS", L"Signup", 0);
                // TODO 여기에 델리게이트 달아서 챗 메세지 들어온거 확인하기                  
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 302] packet id : %d Data : %s\n"), received_data->packet_id, *defind.CharArrayToFString(received_data->user_id));
                if (Fuc_boradcast_success.IsBound() == true) Fuc_boradcast_success.Execute(*defind.CharArrayToFString(received_data->user_id));
                break;
            default:
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch default] packet id : %d\n"), received_data->packet_id);
                break;
            }

            receive_header_check_data_queue.pop();
        }
    }
}

// 인게임 쓰레드
void UClient_To_Server::receive_ingame_moveinfo_data_read_thread()
{
    while (recevie_connected)
    {
        // 대기열에 액세스하기 위한 잠금 획득
        std::lock_guard<std::mutex> lock(received_data_mutex);

        // 큐에 받은 데이터가 있는지 확인
        if (!receive_ingame_moveinfo_data_queue.empty()) {
            // 대기열에서 처음 받은 데이터 가져오기
            FSetMoveInfo* received_ingmae_data_ = receive_ingame_moveinfo_data_queue.front();

            switch (received_ingmae_data_->packet_id)
            {
            case (UINT32)CLIENT_PACKET_ID::IN_GAME_SUCCESS:
                ::MessageBox(NULL, L"IN_GAME_SUCCESS", L"SignIn", 0);
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 402] packet id : %d data : %s\n"), received_ingmae_data_->packet_id, *received_ingmae_data_->fvector_.ToString());
                // 캐릭터 선택창 델리게이트 달어야하지만 인게임 진입으로 일단 변경
                if (Fuc_in_game_init.IsBound() == true) Fuc_in_game_init.Execute(*received_ingmae_data_);
                break;
            case (UINT32)CLIENT_PACKET_ID::LOGIN_RESPONSE_FAIL:
                ::MessageBox(NULL, L"Signin_Fail", L"SignIn", 0);
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 202] packet id : %d\n"), received_ingmae_data_->packet_id);
                break;
            default:
                UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch default] packet id : %d\n"), received_ingmae_data_->packet_id);
                break;
            }

            receive_ingame_moveinfo_data_queue.pop();
        }
    }
}

void UClient_To_Server::InGameAccept()
{
    UE_LOG(LogTemp, Warning, TEXT("[InGameAccept]\n"));

    Login_Send_Packet login_send_packet;
    login_send_packet.packet_id = (int)CLIENT_PACKET_ID::IN_GAME_REQUEST;
    login_send_packet.packet_length = sizeof(login_send_packet);

    Server_Packet_Send((char*)&login_send_packet, login_send_packet.packet_length);
}
