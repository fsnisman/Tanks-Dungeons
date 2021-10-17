// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAxis("RotateTurretRight");
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GetMousePosition(LastFrameMousePosition.X, LastFrameMousePosition.Y);
	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(AxisValue);
	}
}

void ATankPlayerController::RotateRight(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(AxisValue);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}
}

void ATankPlayerController::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	FVector mouseDirection;
	DeprojectMousePositionToWorld(MousePos, mouseDirection);

	if (!TankPawn) {
		return;
	}

	FVector2D MouseScreenPosition;
	GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y);
	bool bWasMouseMoved = !LastFrameMousePosition.Equals(MouseScreenPosition);
	LastFrameMousePosition = MouseScreenPosition;

	if (TankPawn)
	{
		float TurretRotationAxis = GetInputAxisValue("RotateTurretRight");
		if (FMath::IsNearlyZero(TurretRotationAxis) && (bWasMouseMoved || bIsControllingFromMouse))
		{
			bIsControllingFromMouse = true;
			FVector WorldMousePosition, MouseDirection;
			DeprojectMousePositionToWorld(WorldMousePosition, MouseDirection);

			FVector PawnPos = TankPawn->GetActorLocation();
			WorldMousePosition.Z = PawnPos.Z;
			FVector NewTurretDirection = WorldMousePosition - PawnPos;
			NewTurretDirection.Normalize();

			FVector TurretTarget = PawnPos + NewTurretDirection * 1000.f;
			TankPawn->SetTurretTarget(TurretTarget);
		}
		else
		{
			bIsControllingFromMouse = false;
			TankPawn->SetTurretRotationAxis(TurretRotationAxis);
		}
	}
}