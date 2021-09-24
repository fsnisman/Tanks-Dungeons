// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjecTile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "DamageTraker.h"


// Sets default values
AProjecTile::AProjecTile()
{
 
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjecTile::OnMeshOverlapBegin);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);

	//ExpotionEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("Shooting effect"));
}


void AProjecTile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjecTile::Move, MoveRate, true, MoveRate);
	SetLifeSpan(FlyRange / MoveSpeed);
}

// Called every frame
void AProjecTile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	bool bWasTargetDestroyed = false;
	if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExpotionEffect, GetActorTransform());
		OtherActor->Destroy();
		bWasTargetDestroyed = true;
	}
	else if (IDamageTraker* DamageTraker = Cast<IDamageTraker>(OtherActor)) 
	{
		AActor* MyInstigator = GetInstigator();
		if (OtherActor != MyInstigator)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExpotionEffect, GetActorTransform());
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.DamageMaker = this;
			DamageData.Instigator = MyInstigator;
			DamageTraker->TakeDamage(DamageData);
			bWasTargetDestroyed = DamageTraker->TakeDamage(DamageData);
		}
	}
	else
	{
		UPrimitiveComponent* PrimCompont = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		if (PrimCompont)
		{
			if (PrimCompont->IsSimulatingPhysics())
			{
				FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
				PrimCompont->AddForceAtLocation(forceVector * PushForce, SweepResult.ImpactPoint);
			}
		}

	}

	if (bWasTargetDestroyed && OnDestroyedTarget.IsBound())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExpotionEffect, GetActorTransform());
		OnDestroyedTarget.Broadcast(OtherActor);
	}
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExpotionEffect, GetActorTransform());
	Destroy();
	OnDestroyedTarget.Clear();
}

void AProjecTile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

void AProjecTile::Explode()
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
			ECollisionChannel::ECC_MAX,
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
