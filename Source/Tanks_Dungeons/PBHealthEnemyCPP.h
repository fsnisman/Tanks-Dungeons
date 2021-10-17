// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PBHealthEnemyCPP.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_DUNGEONS_API UPBHealthEnemyCPP : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
		UProgressBar* EnemyPB;

	virtual void NativeConstruct() override;
};
