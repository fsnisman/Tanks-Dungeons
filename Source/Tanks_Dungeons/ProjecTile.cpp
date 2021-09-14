// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "ProjecTile.h"
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

	if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageTraker* DamageTraker = Cast<IDamageTraker>(OtherActor)) 
	{
		AActor* MyInstigator = GetInstigator();
		if (OtherActor != MyInstigator)
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.DamageMaker = this;
			DamageData.Instigator = MyInstigator;
			DamageTraker->TakeDamage(DamageData);
		}
	}
	
	Destroy();
}

void AProjecTile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

