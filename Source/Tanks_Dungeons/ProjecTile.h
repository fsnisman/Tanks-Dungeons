// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTraker.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>
#include "ProjecTile.generated.h"

class UStaticMeshComponent;

UCLASS()
class TANKS_DUNGEONS_API AProjecTile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* TrailEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float FlyRange = 10000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float Damage = 1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystem* ExpotionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float PushForce = 1000;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		bool EnableExlode = false;


	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		FVector MoveVector;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		TArray<FVector> CurrentTrajectory;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		int32 TragectoryPointIndex;

	FTimerHandle MovementTimerHandle;

public:	
	
	AProjecTile();

	virtual void Start();
	virtual void Explode();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDestroyedTarger, AActor*);
	FOnDestroyedTarger OnDestroyedTarget;


protected:
	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void Move();
};
