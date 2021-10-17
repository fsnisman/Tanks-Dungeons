// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "DamageTraker.h"
#include "Engine/TargetPoint.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "HealthComponent.h"
#include "AmmoBox.h"
#include "CoreMinimal.h"
#include "TankPawn.h"
#include "HealthComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "MapLoader.h"
#include "TankFactory.generated.h"


UCLASS()
class TANKS_DUNGEONS_API ATankFactory : public AActor, public IDamageTraker
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BuildingMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* TankSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* DieBuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
		AMapLoader* LinkedMapLoader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
		TSubclassOf<ATankPawn> SpawnTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
		float SpawnTankRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
		TArray<ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UAudioComponent* AudioEffectDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystemComponent* EffectDie;

public:
	// Sets default values for this actor's properties
	ATankFactory();

	UFUNCTION()
		bool TakeDamage(FDamageData DamageData) override;

	bool bDieFactory = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnNewTank();

	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);
};
