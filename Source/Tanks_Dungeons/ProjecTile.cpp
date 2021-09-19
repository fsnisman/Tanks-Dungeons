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

	ExpotionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shooting effect"));
	ExpotionEffect->SetupAttachment(RootComponent);
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
		ExpotionEffect->ActivateSystem();
		OtherActor->Destroy();
		bWasTargetDestroyed = true;
	}
	else if (IDamageTraker* DamageTraker = Cast<IDamageTraker>(OtherActor)) 
	{
		AActor* MyInstigator = GetInstigator();
		if (OtherActor != MyInstigator)
		{
			ExpotionEffect->ActivateSystem();
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.DamageMaker = this;
			DamageData.Instigator = MyInstigator;
			DamageTraker->TakeDamage(DamageData);
			bWasTargetDestroyed = DamageTraker->TakeDamage(DamageData);
		}
	}

	if (bWasTargetDestroyed && OnDestroyedTarget.IsBound())
	{
		ExpotionEffect->ActivateSystem();
		OnDestroyedTarget.Broadcast(OtherActor);
	}
	
	ExpotionEffect->ActivateSystem();
	Destroy();
	OnDestroyedTarget.Clear();
}

void AProjecTile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

