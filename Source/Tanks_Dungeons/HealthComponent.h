// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStructs.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_DUNGEONS_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, DamageValue);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
        float MaxHealth = 10;

    UPROPERTY()
        float CurrentHealth;

public:
    UPROPERTY(BlueprintAssignable)
    FOnDie OnDie;

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnDamaged;

public:
    // Sets default values for this component's properties
    UHealthComponent();

    void TakeDamage(FDamageData DamageData);

    virtual void BeginPlay() override;

    float GetHealth() const;

    float GetHealthState() const;

    void AddHealth(float AddiditionalHealthValue);
};
