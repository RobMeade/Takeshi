// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once


UENUM(BlueprintType)
enum class EGameOverOutcome : uint8
{
	PlayerWin UMETA(Displayname = "Player Win"),
	PlayerLoss UMETA(DisplayName = "Player Loss")
};
