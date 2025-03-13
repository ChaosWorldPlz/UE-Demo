// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

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
/*
void UBaseAttributeSet::OnRep_HP(const FGameplayAttribute& Old_HP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet,HP,Old_HP);
}
*/


