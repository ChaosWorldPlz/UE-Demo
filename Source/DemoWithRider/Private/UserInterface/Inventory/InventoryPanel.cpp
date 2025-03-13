// Fill out your copyright notice in the Description page of Project Settings.

//Game
#include "UserInterface/Inventory/InventoryPanel.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Player/PlayerCharacter.h"
#include "UserInterface/Inventory/InventoryComponent.h"
//Engine
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Items/ItemBase.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Link InventoryPanel to Player Inventory
	PlayerCharacter	= Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this,&UInventoryPanel::RefreshInventory);
			
			SetInfoText();
		}
	}
}


void UInventoryPanel::SetInfoText() const 
 {
	WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
							InventoryReference->GetInventoryTotalWeight(),
							InventoryReference->GetWeightCapacity()));
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
							InventoryReference->GetInventoryContents().Num(),
							InventoryReference->GetSlotsCapacity()));
 }

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();
		
		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this,InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);
			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
