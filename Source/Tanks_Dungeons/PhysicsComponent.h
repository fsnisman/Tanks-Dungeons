// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_DUNGEONS_API UPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
		float Gravity = -9.8f;
public:
	TArray<FVector> GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue);
		
	FVector CalculateBezierPoint(float t, FVector p0, FVector p1, FVector p2, FVector p3);
};
