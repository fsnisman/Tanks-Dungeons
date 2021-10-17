// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjecTile.h"
#include "PhysicsProjectile.generated.h"

class UPhysicsComponent;

/**
 * 
 */
UCLASS()
class TANKS_DUNGEONS_API APhysicsProjectile : public AProjecTile
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UPhysicsComponent* PhysicsComponent;

public:
	APhysicsProjectile();
	virtual void Start() override;
	virtual void Explode() override;

protected:
	virtual void Move() override;
	
};
