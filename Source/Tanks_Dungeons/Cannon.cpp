// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceenceCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceenceCpm;

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileSpawnPoint = CreateAbstractDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shooting effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Shoot audio effect"));
	AudioEffect->SetupAttachment(ProjectileSpawnPoint);
}

void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::AddAmmo(int32 InNumAmmo)
{
	NumAmmo = FMath::Clamp(NumAmmo + InNumAmmo, 0, MaxAmmo);
	UE_LOG(LogTemp, Log, TEXT("AddAmmo(%d)! NumAmmo: %d"), InNumAmmo, NumAmmo);
}

void ACannon::AddAmmoSpecial(int32 InNumAmmo)
{
	NumAmmoSpecial = FMath::Clamp(NumAmmoSpecial + InNumAmmo, 0, MaxAmmoSpecial);
	UE_LOG(LogTemp, Log, TEXT("AddAmmo(%d)! NumAmmo: %d"), InNumAmmo, NumAmmoSpecial);
}

void ACannon::Fire()
{
	if (!ReadyToFire)
	{
		return;
	}
	if(NumAmmo <= 0)
	{
		//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, "No Ammo");
		return;
	}
	ReadyToFire = false;

	ShootEffect->ActivateSystem();
	AudioEffect->Play();

	--NumAmmo;

	if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ShootForceEffect)
		{
			FForceFeedbackParameters shootForceEffectParams;
			shootForceEffectParams.bLooping = false;
			shootForceEffectParams.Tag = "shootForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, shootForceEffectParams);
		}

		if (ShootShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShake);
		}
	}

	if (Type == ECannonType::FireProjectile)
	{
		//GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - projectile");

		FTransform projectileTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector(1));

		AProjecTile* projectile = GetWorld()->SpawnActor<AProjecTile>(ProjecTileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (projectile) 
		{
			projectile->SetInstigator(GetInstigator());
			projectile->OnDestroyedTarget.AddUObject(this, &ACannon::NotifytargetDestroed);
			projectile->Start();
		}
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Orange, "Fire - trace");

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;

		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Yellow, false, 0.5f, 0, 5);
			if (hitResult.Component.IsValid() && hitResult.Component->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
			{
				hitResult.Actor.Get()->Destroy();
			}
			else if (IDamageTraker* DamageTraker = Cast<IDamageTraker>(hitResult.Actor))
			{
				AActor* MyInstigator = GetInstigator();
				if (hitResult.Actor != MyInstigator)
				{
					FDamageData DamageData;
					DamageData.DamageValue = FireDamage;
					DamageData.DamageMaker = this;
					DamageData.Instigator = MyInstigator;
					DamageTraker->TakeDamage(DamageData);
				}
			}
		}
		
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 0.5f, 0, 5);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::FireSpecial()
{

	if (!ReadyToFire)
	{
		return;
	}
	if(NumAmmoSpecial <= 0)
	{
		//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, "No Ammo");
		return;
	}
	ReadyToFire = false;

	ShootEffect->ActivateSystem();
	AudioEffect->Play();

	--NumAmmoSpecial;

	if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ShootForceEffect)
		{
			FForceFeedbackParameters shootForceEffectParams;
			shootForceEffectParams.bLooping = false;
			shootForceEffectParams.Tag = "shootForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, shootForceEffectParams);
		}

		if (ShootShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShake);
		}
	}


	if (Type == ECannonType::FireProjectile)
	{
		for (int i = 0; i < 3; i++) {

			//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Green, "Fire - special");

			FTransform projectileTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector(1));

			AProjecTile* projectile = GetWorld()->SpawnActor<AProjecTile>(ProjecTileClassSpecial, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			if (projectile)
			{
				projectile->Start();
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 0.1f, false);
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

void ACannon::NotifytargetDestroed(AActor* Target)
{
	if (OnDestroyedTarget.IsBound())
	{
		OnDestroyedTarget.Broadcast(Target);
	}
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	NumAmmo = MaxAmmo;
	NumAmmoSpecial = MaxAmmoSpecial;
	Reload();
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

