
struct CREATE_PACKET : public PACKET_HEADER
{
	int32 guid;
};
struct REMOVE_PACKET :public PACKET_HEADER
{
	int32 guid;
};

struct INPUT_PACKET : public PACKET_HEADER
{
	int32 guid;
	
	FVector current_position;
	float current_direction;
	float input_direction;
	int32 input_data;

	INPUT_PACKET()
	{
		//packet_id = static_cast<int>(PACKET_ID::CS_PLAYER_ORIGIN_ACTION_REQ);
		packet_length = sizeof(INPUT_PACKET);

		current_direction = 0.f;
		input_direction = 0.f;
		input_data = 0;
	}
};

struct UPDATE_STATE_PACKET :public PACKET_HEADER
{
	int32 guid;
	
	int32 state_value;
	int32 _value;
};

struct EQUIP_PACKET : public PACKET_HEADER
{
	int32 guid;
	int32 item_id;
};