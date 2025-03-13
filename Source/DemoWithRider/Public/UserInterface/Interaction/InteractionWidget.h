// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UProgressBar;
struct FInteractableData ;
class APlayerCharacter;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEMOWITHRIDER_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
	UPROPERTY(VisibleAnywhere, Category= "Interaction Widget | Player Reference ")
	APlayerCharacter* PlayerReference;

	void UpdateWidget(const FInteractableData* InteractableData) const;

protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget),Category= "Interaction Widget | Interactable Data ")
	UTextBlock* NameText;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget),Category= "Interaction Widget | Interactable Data ")
	UTextBlock* ActionText;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget),Category= "Interaction Widget | Interactable Data ")
	UTextBlock* QuantityText;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget),Category= "Interaction Widget | Interactable Data ")
	UTextBlock* KeyPressText;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget),Category= "Interaction Widget | Interactable Data ")
	UProgressBar* InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget),Category= "Interaction Widget | Interactable Data ")
	float CurrentInteractionDuration;

	UFUNCTION(Category = "Interaction Widget | Interactable Data ")
	float UpdateInteractionProgress();
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
};
