// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "WeaponBullet.generated.h"

UCLASS()
class DEMOWITHRIDER_API AWeaponBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBullet();

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Meta=(ExposeOnSpawn=true))
	float Range;

	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn=true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	class UProjectileMovementComponent* BulletMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
