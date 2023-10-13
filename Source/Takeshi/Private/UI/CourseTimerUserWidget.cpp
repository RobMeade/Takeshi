// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#include "UI/CourseTimerUserWidget.h"

#include "Components/TextBlock.h"


void UCourseTimerUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCourseTimerUserWidget::SetTimer(const int32 NewTimeInSeconds) const
{
	if (Timer)
	{
		int32 TimerMinutes = 0;
		int32 TimerSeconds = 0;

		TimerMinutes = NewTimeInSeconds / 60;
		TimerSeconds = NewTimeInSeconds % 60;

		FNumberFormattingOptions* TimerFormattingOptions = new FNumberFormattingOptions();
		TimerFormattingOptions->MaximumFractionalDigits = 0;
		TimerFormattingOptions->SetMinimumIntegralDigits(2);

		FString TimerTextString = FText::AsNumber(TimerMinutes, TimerFormattingOptions).ToString();
		TimerTextString += ":";
		TimerTextString += FText::AsNumber(TimerSeconds, TimerFormattingOptions).ToString();
		
		Timer->SetText(FText::FromString(TimerTextString));
	}
}
