// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "CourseTimerUserWidget.generated.h"


// Forward Declarations
class UTextBlock;


UCLASS()
class TAKESHI_API UCourseTimerUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	void SetTimer(const int32 NewTimeInSeconds) const;


protected:

	virtual void NativeConstruct() override;


private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Timer = nullptr;

};
