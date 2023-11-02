// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once


UENUM(BlueprintType)
enum class ESkippingStoneResponsivity : uint8
{
	NotSet UMETA(Displayname = "Not Set"),
	Responsive UMETA(DisplayName = "Responsive"),
	UnResponsive UMETA(DisplayName = "UnResponsive")
};
