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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet")
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet")
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet")
	FGameplayAttributeData MP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet")
	FGameplayAttributeData MaxMP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet")
	FGameplayAttributeData SP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, SP);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseAttributeSet")
	FGameplayAttributeData MaxSP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxSP);
/*protected:
	/*Replication Notificaion Functions for Attributes#1#
	UFUNCTION()
	virtual void OnRep_HP(const FGameplayAttribute& Old_HP);
	
	UFUNCTION()
	virtual void OnRep_MaxHP(const FGameplayAttribute& Old_MaxHP);
	
	UFUNCTION()
	virtual void OnRep_MP(const FGameplayAttribute& Old_MP);
	
	UFUNCTION()
	virtual void OnRep_MaxMP(const FGameplayAttribute& Old_MaxMP);
	
	UFUNCTION()
	virtual void OnRep_SP(const FGameplayAttribute& Old_SP);
	
	UFUNCTION()
	virtual void OnRep_MaxSP(const FGameplayAttribute& Old_MaxSP);*/
};
