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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* TrailEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MoveAccurency = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationMaxTime = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationTimeStep = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationSpeed = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		bool ShowTrajectory = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float ExplodeRadius = 50.f;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		FVector MoveVector;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		TArray<FVector> CurrentTrajectory;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		int32 TragectoryPointIndex;

public:
	APhysicsProjectile();
	virtual void Start() override;
	void Explode();

protected:
	virtual void Move() override;
	
};
