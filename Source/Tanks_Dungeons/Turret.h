// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.h"
#include "DamageTraker.h"
#include "TankPawn.h"
#include "HealthComponent.h"
#include "Scorable.h"
#include "Turret.generated.h"

UCLASS()
class TANKS_DUNGEONS_API ATurret : public AActor, public IDamageTraker, public IScorable
{
	GENERATED_BODY()

public:


protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UStaticMeshComponent* BodyMesh;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UStaticMeshComponent* TurretMesh;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UArrowComponent* CannonSetupPoint;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UBoxComponent* HitCollider;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UHealthComponent* HealthComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
        TSubclassOf<ACannon> CannonClass;

    UPROPERTY()
        ACannon* Cannon;
    UPROPERTY()
        APawn* PlayerPawn;
    UPROPERTY()
        ATankPawn* TankPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
        float TargetingRange = 1000;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
        float TargetingSpeed = 0.1f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
        float TargetingRate = 0.005f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
        float Accurency = 20;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scorce")
        int32 DestructionScores = 0;

    const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
    const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

public:
    ATurret();

    UFUNCTION()
        bool TakeDamage(FDamageData DamageData) override;
    UFUNCTION()
        int32 GetScores() const override;

    virtual void PostInitializeComponents() override;

protected:
    bool DetectPlayerVisibility();
    virtual void Destroyed() override;
    void Targeting();
    void RotateToPlayer();
    bool IsPlayerInRange();
    bool CanFire();
    void Fire();
	virtual void BeginPlay() override;

    UFUNCTION()
        void Die();

    UFUNCTION()
        void DamageTaken(float InDamage);

};
