// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBullet.h"
#include "AbilitySystem/BaseGameplayAbility.h"
#include "Weapon_Fire.generated.h"


/**
 * 
 */
UCLASS()
class DEMOWITHRIDER_API UWeapon_Fire : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UWeapon_Fire();

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UAnimMontage* FireHitMontage;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UAnimMontage* FireIronsightsMontage;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TSubclassOf<AWeaponBullet> BulletClass;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override ;
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
 
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
 
	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);


	
};
