// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Fire.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"


UWeapon_Fire::UWeapon_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Ability1"));
	AbilityTags.AddTag(Ability1Tag);
	ActivationOwnedTags.AddTag(Ability1Tag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));

	Range = 1000.0f;
	Damage = 12.0f;
}

void UWeapon_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UAnimMontage* MontageToPlay = FireHitMontage;

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.AimDownSights"))) &&
		!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.AimDownSights.Removal"))))
	{
		MontageToPlay = FireIronsightsMontage;
	}

	// Play fire montage and wait for event telling us to spawn the projectile
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayMontageTask"),
		MontageToPlay,
		1.0f,
		FName(),
		true,
		1.0f,
		0.0f,
		true
	);

	if (Task)
	{
		/*Task->ReadyForActivation();*/
	}
	

}

void UWeapon_Fire::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
 
void UWeapon_Fire::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
 
void UWeapon_Fire::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
 
    if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
    {
	    APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
    	if (!Player)
    	{
    		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    		return;
    	}
 
    	FVector Start = Player->GetGunComponent()->GetSocketLocation(FName("Muzzle"));
    	FVector End = Player->GetCameraBoom()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * Range;
    	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
 
    	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
    	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);
 
    	FTransform MuzzleTransform = Player->GetGunComponent()->GetSocketTransform(FName("Muzzle"));
    	MuzzleTransform.SetRotation(Rotation.Quaternion());
    	MuzzleTransform.SetScale3D(FVector(1.0f));
 
    	FActorSpawnParameters SpawnParameters;
    	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
 
    	AWeaponBullet* Projectile = GetWorld()->SpawnActorDeferred<AWeaponBullet>(BulletClass, MuzzleTransform, GetOwningActorFromActorInfo(), Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    	Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
    	Projectile->Range = Range;
    	Projectile->FinishSpawning(MuzzleTransform);
    }
}