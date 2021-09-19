// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Cannon.h"
#include "Scorable.h"
#include <Camera/CameraComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Components/ArrowComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <Math/UnrealMathUtility.h>
#include <Kismet/KismetMathLibrary.h>
#include "TankPlayerController.h"

//DECLARE_LOG_CATEGORY_EXTERN(TankLog, All, All);
//DEFINE_LOG_CATEGORY(TankLog);


// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddDynamic(this, &ATankPawn::Die);
	HealthComponent->OnDamaged.AddDynamic(this, &ATankPawn::DamageTaked);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::MoveForward(float AxisValue) 
{
	TargetForwardAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float AxisValue)
{
	TargetRightAxisValue = AxisValue;
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());

	SetupCannon(CannonClass);
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> InCannonClass)
{
	if (ActiveCannon)
	{
		ActiveCannon->Destroy();
		ActiveCannon = nullptr;
	}

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	ActiveCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, params);
	ActiveCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ActiveCannon->OnDestroyedTarget.AddUObject(this, &ATankPawn::TargetDestroyed);
}

void ATankPawn::SwapCannon()
{
	Swap(ActiveCannon, InactiveCannon);
	if (ActiveCannon)
	{
		ActiveCannon->SetVisibility(true);
	}
	if (InactiveCannon)
	{
		InactiveCannon->SetVisibility(false);
	}
}

ACannon* ATankPawn::GetActiveCannon() const
{
	return ActiveCannon;
}

void ATankPawn::Fire()
{
	if (ActiveCannon)
	{
		ActiveCannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (ActiveCannon)
	{
		ActiveCannon->FireSpecial();
	}

}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank Movement
	CurrentForwardAxisValue = FMath::FInterpTo(CurrentForwardAxisValue, TargetForwardAxisValue, DeltaTime, ForwardSmootheness);
	TargetForwardAxisValue = CurrentForwardAxisValue;

	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector MovePosition = CurrentLocation + ForwardVector * MoveSpeed * TargetForwardAxisValue * DeltaTime;
	
	SetActorLocation(MovePosition, true);

	// Tank Rotation
	CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmootheness);
	TargetRightAxisValue = CurrentRightAxisValue;

	//UE_LOG(LogTemp, Warning, TEXT("CurrentRightAxisValue = %f TargetRightAxisValue = %f"), CurrentRightAxisValue, TargetRightAxisValue);

	FRotator CurrentRotation = GetActorRotation();
	float yawRotation = RotationSpeed * TargetRightAxisValue * DeltaTime;
	yawRotation += CurrentRotation.Yaw;
	
	FRotator newRotation = FRotator(0.f, yawRotation, 0.f);
	SetActorRotation(newRotation);

	// Turret rotation
	if (TankController) 
	{
		FVector mousePos = TankController->GetMousePos();
		RotateTurretTo(mousePos);
	}
}

bool ATankPawn::TakeDamage(FDamageData DamageData)
{
	return HealthComponent->TakeDamage(DamageData);
}

void ATankPawn::Die()
{
	Destroy();
}

void ATankPawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void ATankPawn::TargetDestroyed(AActor* Target)
{
	if (IScorable* Scorable = Cast<IScorable>(Target))
	{
		AccumulatedScores += Scorable->GetScores();
		UE_LOG(LogTemp, Log, TEXT("Destroyed target %s. Curret scores: %d"), *Target->GetName(), AccumulatedScores);
	}
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(currRotation, targetRotation, GetWorld()->GetDeltaSeconds(), TurretRotationSpeed));
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}