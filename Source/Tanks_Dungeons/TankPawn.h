// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "DamageTraker.h"
#include "HealthComponent.h"
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


	UPROPERTY()
		ATankPlayerController* TankController;

	UPROPERTY()
		ACannon* ActiveCannon;

	UPROPERTY()
		ACannon* InactiveCannon;


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
		TArray<FVector> PatrollingPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
		float MovementAccuracy = 50.f;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		const TArray<FVector>& GetPatrollingPoints() 
	{ 
		return PatrollingPoints; 
	};
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

	
	//virtual void TargetDestroyed(AActor* Target);

private:
	float TargetForwardAxisValue = 0.f;
	float TargetRightAxisValue = 0.f;
	float CurrentRightAxisValue = 0.f;
	float CurrentForwardAxisValue = 0.f;

	int32 AccumulatedScores = 0;
};
