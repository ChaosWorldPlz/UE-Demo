// Fill out your copyright notice in the Description page of Project Settings.

//GAME
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryToolTip.h"
//ENGINE
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ToolTipClass)
	{
		UInventoryToolTip* ToolTip = CreateWidget<UInventoryToolTip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ItemReference)
	{
		switch (ItemReference->ItemQuality)
		{
		case EItemQuality::Rarity_N:
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EItemQuality::Rarity_R:
			ItemBorder->SetBrushColor(FLinearColor(0,255,255));//Blue
			break;
		case EItemQuality::Rarity_SR:
			ItemBorder->SetBrushColor(FLinearColor(75,0,130));//Purple
			break;
		case EItemQuality::Rarity_SSR:
			ItemBorder->SetBrushColor(FLinearColor(255,215,0));//Gold

			break;
		case EItemQuality::Rarity_UR:
			ItemBorder->SetBrushColor(FLinearColor(139,0,0));//Red

			break;
		default:
			break;
		}
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		ItemName->SetText(ItemReference->TextData.Name);
		if (ItemReference->NumericData.IsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}



FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
