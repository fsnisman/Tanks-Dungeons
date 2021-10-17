// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "MainMenuWidgetCPP.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class TANKS_DUNGEONS_API UMainMenuWidgetCPP : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
		UButton* StartGame;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
		UButton* QuitBtn;

	UFUNCTION()
		void OnButtonStartGameClicked();

	UFUNCTION()
		void OnButtonQuitGameClicked();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Animation;


	virtual void NativeConstruct() override;

	bool bIgnorePlatformRestrictions = false;

};
