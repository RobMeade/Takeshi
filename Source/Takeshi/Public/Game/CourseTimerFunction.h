// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once


UENUM(BlueprintType)
enum class ECourseTimerFunction : uint8
{
	Decrement UMETA(Displayname = "Decrement"),
	Increment UMETA(DisplayName = "Increment")
};