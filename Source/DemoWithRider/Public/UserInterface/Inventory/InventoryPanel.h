// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//Engine
#include "Components/WrapBox.h"
#include "Components/TextBlock.h" 
//Game
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"


class UInventoryItemSlot;
class APlayerCharacter;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class DEMOWITHRIDER_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
	UFUNCTION()
	void RefreshInventory();

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;



protected:
	//======================================================================
	// FUNCTIONS
	//======================================================================
	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation) override;
};
