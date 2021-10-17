// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "DrawDebugHelpers.h"
#include "PhysicsComponent.h"
#include "Kismet/KismetMathLibrary.h"

APhysicsProjectile::APhysicsProjectile()
{
    PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));
}

void APhysicsProjectile::Start()
{
    MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
    CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep, 0);
    if (ShowTrajectory)
    {
        for (FVector position : CurrentTrajectory)
        {
            DrawDebugSphere(GetWorld(), position, 5, 8, FColor::Purple, true, 1, 0, 2);
        }
    }

    TragectoryPointIndex = 0;
    Super::Start();
}

void APhysicsProjectile::Move()
{
    FVector currentMoveVector = CurrentTrajectory[TragectoryPointIndex] - GetActorLocation();
    currentMoveVector.Normalize();
    FVector newLocation = GetActorLocation() + currentMoveVector * MoveSpeed * MoveRate;
    SetActorLocation(newLocation);
    if (FVector::Distance(newLocation, CurrentTrajectory[TragectoryPointIndex]) <= MoveAccurency)
    {
        TragectoryPointIndex++;
        if (TragectoryPointIndex >= CurrentTrajectory.Num())
        {
            Explode();
            Destroy();
        }
        else
        {
            FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryPointIndex]);
            SetActorRotation(newRotation);
        }
    }
}

void APhysicsProjectile::Explode()
{
    if (EnableExlode == true)
    {
        FVector startPos = GetActorLocation();
        FVector endPos = startPos + FVector(0.1f);

        FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
        FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
        params.AddIgnoredActor(this);
        params.bTraceComplex = true;
        params.TraceTag = "Explode Trace";
        TArray<FHitResult> AttackHit;

        FQuat Rotation = FQuat::Identity;

        bool sweepResult = GetWorld()->SweepMultiByChannel
        (
            AttackHit,
            startPos,
            endPos,
            Rotation,
            ECollisionChannel::ECC_Visibility,
            Shape,
            params
        );

        //GetWorld()->DebugDrawTraceTag = "Explode Trace";

        if (sweepResult)
        {
            for (FHitResult hitResult : AttackHit)
            {
                AActor* otherActor = hitResult.GetActor();
                if (!otherActor)
                    continue;

                IDamageTraker* damageTrakerActor = Cast<IDamageTraker>(otherActor);
                if (damageTrakerActor)
                {
                    FDamageData damageData;
                    damageData.DamageValue = Damage;
                    damageData.Instigator = GetOwner();
                    damageData.DamageMaker = this;

                    damageTrakerActor->TakeDamage(damageData);
                }
                else
                {
                    UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(otherActor->GetRootComponent());
                    if (PrimComp)
                    {
                        if (PrimComp->IsSimulatingPhysics())
                        {
                            FVector forceVector = otherActor->GetActorLocation() - GetActorLocation();
                            forceVector.Normalize();
                            PrimComp->AddImpulse(forceVector * PushForce, NAME_None, true);
                        }
                    }
                }

            }
        }
    }
}
