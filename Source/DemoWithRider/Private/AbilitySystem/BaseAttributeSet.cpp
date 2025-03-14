// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Player/PlayerCharacter.h"

UBaseAttributeSet::UBaseAttributeSet()
{
	/*Initial values for All Attributes*/
	HP=100.0f;
	MaxHP=100.0f;

	MP=100.0f;
	MaxMP=100.0f;

	SP=100.0f;
	MaxSP=100.0f;
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	// If a Max value changes, adjust current to keep Current % of Current to Max
	if (Attribute == GetMaxHPAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
	{
		AdjustAttributeForMaxChange(HP, MaxHP, NewValue, GetHPAttribute());
	}
	else if (Attribute == GetMaxMPAttribute())
	{
		AdjustAttributeForMaxChange(MP, MaxMP, NewValue, GetMPAttribute());
	}
	else if (Attribute == GetMaxSPAttribute())
	{
		AdjustAttributeForMaxChange(SP, MaxSP, NewValue, GetSPAttribute());
	}
	
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//Source Actor 
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	APlayerCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<APlayerCharacter>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	APlayerCharacter* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<APlayerCharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<APlayerCharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// If character was alive before damage is added, handle damage
			// This prevents damage being added to dead things and replaying death animations
			bool WasAlive = true;

			// Apply the health change and then clamp it
			const float NewHealth = GetHP() - LocalDamageDone;
			SetHP(FMath::Clamp(NewHealth, 0.0f, GetMaxHP()));

			if (TargetCharacter && WasAlive)
			{
				// This is the log statement for damage received. Turned off for live games.
				//UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), TEXT(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);

				// Play HitReact animation and sound with a multicast RPC.
				const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();
				
			}
		}
	}// Damage
	else if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		// Handle other health changes.
		// Health loss should go through Damage.
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
	} // Health
	else if (Data.EvaluatedData.Attribute == GetMPAttribute())
	{
		// Handle mana changes.
		SetHP(FMath::Clamp(GetMP(), 0.0f, GetMaxMP()));
	} // Mana
	else if (Data.EvaluatedData.Attribute == GetSPAttribute())
	{
		// Handle stamina changes.
		SetSP(FMath::Clamp(GetSP(), 0.0f, GetMaxSP()));
	}

	
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UBaseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
	const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue)&&AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}





void UBaseAttributeSet::OnRep_HP(const FGameplayAttributeData& Old_HP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HP, Old_HP);
}

void UBaseAttributeSet::OnRep_MaxHP(const FGameplayAttributeData& Old_MaxHP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHP, Old_MaxHP);
}

void UBaseAttributeSet::OnRep_MP(const FGameplayAttributeData& Old_MP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MP, Old_MP);
}

void UBaseAttributeSet::OnRep_MaxMP(const FGameplayAttributeData& Old_MaxMP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMP, Old_MaxMP);
}

void UBaseAttributeSet::OnRep_SP(const FGameplayAttributeData& Old_SP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, SP, Old_SP);
}

void UBaseAttributeSet::OnRep_MaxSP(const FGameplayAttributeData& Old_MaxSP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxSP, Old_MaxSP);
}




