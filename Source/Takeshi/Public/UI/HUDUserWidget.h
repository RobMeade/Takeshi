// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "HUDUserWidget.generated.h"


// Forward Declarations
class UCourseTimerUserWidget;
class UPlayerLivesUserWidget;


UCLASS()
class TAKESHI_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	void SetCourseTimer(const float NewTimeInSeconds) const;
	void SetPlayerLives(const int32 NewPlayerLives) const;


protected:

	virtual void NativeConstruct() override;


private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCourseTimerUserWidget> CourseTimer = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerLivesUserWidget> PlayerLives = nullptr;

};
