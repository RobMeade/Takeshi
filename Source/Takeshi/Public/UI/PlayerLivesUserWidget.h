// Copyright Rob Meade & Ethan Mitchell.  All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "PlayerLivesUserWidget.generated.h"


// Forward Declarations
class UHorizontalBox;


UCLASS()
class TAKESHI_API UPlayerLivesUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	void SetPlayerLives(const int32 NewPlayerLives);


protected:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "Player Lives")
	TSubclassOf<UUserWidget> PlayerLifeUserWidgetClass = nullptr;


private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> PlayerLives = nullptr;
	
};
