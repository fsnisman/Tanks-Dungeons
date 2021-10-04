// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "DamageTraker.h"
#include "Engine/TargetPoint.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "HealthComponent.h"
#include "AmmoBox.h"
#include "TankPawn.generated.h"


class ATankPlayerController;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class ACannon;

UCLASS()
class TANKS_DUNGEONS_API ATankPawn : public APawn, public IDamageTraker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UFUNCTION()
		void MoveForward(float AxisValue);

	UFUNCTION()
		void RotateRight(float AxisValue);

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void FireSpecial();

	UFUNCTION()
		void SetupCannon(TSubclassOf<ACannon> InCannonClass);

	UFUNCTION()
		void SwapCannon();

	UFUNCTION()
		bool TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
		void TargetDestroyed(AActor* Target);

	UFUNCTION()
		ACannon* GetActiveCannon() const;

	UFUNCTION()
		void SetTurretRotationAxis(float AxisValue);

	UFUNCTION()
		void SetTurretTarget(FVector TargetPosition);

protected:
	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> CannonClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UAudioComponent* AudioEffectStopped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UAudioComponent* AudioEffectMoved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		USoundBase* AudioEffectDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* EffectDie;

	UPROPERTY()
		ATankPlayerController* TankController;

	UPROPERTY()
		ACannon* ActiveCannon;

	UPROPERTY()
		ACannon* InactiveCannon;

	UPROPERTY()
		AAmmoBox* DestructionBonusBox;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float ForwardSmootheness = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSmootheness = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Speed")
		float TurretRotationSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
		TArray<ATargetPoint*> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
		float MovementAccuracy = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Check Die Tank")
		bool bDieTankPawn = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		UWidgetComponent* BarHP;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		TArray<FVector> GetPatrollingPoints();

	UFUNCTION()
		float GetMovementAccurency() 
	{ 
		return MovementAccuracy; 
	};

	UFUNCTION()
		FVector GetTurretForwardVector();

	UFUNCTION()
		void RotateTurretTo(FVector TargetPosition);

	UFUNCTION()
		FVector GetEyesPosition();

	UFUNCTION()
		void SetPatrollingPoints(TArray<ATargetPoint*>& NewPatrollingPoints);

	//virtual void TargetDestroyed(AActor* Target);

private:
	float TargetForwardAxisValue = 0.f;
	float TargetRightAxisValue = 0.f;
	float CurrentRightAxisValue = 0.f;
	float CurrentForwardAxisValue = 0.f;
	float TurretRotationAxis = 0.f;
	FVector TurretTarget;

	bool bIsTurretTargetSet = false;

	int32 AccumulatedScores = 0;
};
