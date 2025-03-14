// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

/**

* To use this in your game you can define something like this, and then add game-specific functions as necessary:
 * 
 *	#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
 * 
 *	ATTRIBUTE_ACCESSORS(UMyHealthSet, Health)
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class DEMOWITHRIDER_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBaseAttributeSet();
	/*Basic Attribute
	 *	Health Points
	 *	Magic Points
	 *	Stamina Points
	 *	Damage
	 *	Heal
	 */
	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet", ReplicatedUsing = OnRep_HP)
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet", ReplicatedUsing = OnRep_MaxHP)
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet", ReplicatedUsing = OnRep_MP)
	FGameplayAttributeData MP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet", ReplicatedUsing = OnRep_MaxMP)
	FGameplayAttributeData MaxMP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet", ReplicatedUsing = OnRep_SP)
	FGameplayAttributeData SP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, SP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet", ReplicatedUsing = OnRep_MaxSP)
	FGameplayAttributeData MaxSP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxSP);

	UPROPERTY(BlueprintReadWrite,Category="Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Damage);
	
protected:

	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
	
	/*Replication Notificaion Functions for Attributes*/
	UFUNCTION()
	virtual void OnRep_HP(const FGameplayAttributeData& Old_HP);
	
	UFUNCTION()
	virtual void OnRep_MaxHP(const FGameplayAttributeData& Old_MaxHP);
	
	UFUNCTION()
	virtual void OnRep_MP(const FGameplayAttributeData& Old_MP);
	
	UFUNCTION()
	virtual void OnRep_MaxMP(const FGameplayAttributeData& Old_MaxMP);
	
	UFUNCTION()
	virtual void OnRep_SP(const FGameplayAttributeData& Old_SP);
	
	UFUNCTION()
	virtual void OnRep_MaxSP(const FGameplayAttributeData& Old_MaxSP);
};

