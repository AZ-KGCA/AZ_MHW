// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

enum class EGameMsgType : uint8
{
	None,
};

struct FAZGameMsg
{
	EGameMsgType MsgType;
	bool bIsQueueMsg;

	FAZGameMsg()
	{

	}
	FAZGameMsg(EGameMsgType msgType)
		: MsgType(msgType)
	{
		bIsQueueMsg = false;
	}
	FAZGameMsg(const FAZGameMsg& rhs)
	{
		*this = rhs;
	}
	virtual ~FAZGameMsg() {}
};

