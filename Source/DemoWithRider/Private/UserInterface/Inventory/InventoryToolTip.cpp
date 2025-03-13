// Fill out your copyright notice in the Description page of Project Settings.

//GAME
#include "UserInterface/Inventory/InventoryToolTip.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"
//ENGINE
#include "Components/TextBlock.h"


void UInventoryToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Armor:
		ItemType->SetText(FText::FromString("Armor"));

		break;
	case EItemType::Weapon:
		ItemType->SetText(FText::FromString("Weapon"));
		break;
	case EItemType::Shield:
		ItemType->SetText(FText::FromString("Shield"));
		break;
	case EItemType::Spell:
		ItemType->SetText(FText::FromString("Spell"));
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Quest:
		ItemType->SetText(FText::FromString("Quest"));
		break;
	case EItemType::Mundane:
		ItemType->SetText(FText::FromString("Mundane Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		break;
	}
	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->Statistics.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->Statistics.ArmorRating));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	SellValue->SetText(FText::AsNumber(ItemBeingHovered->Statistics.SellValue));
	StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));

	if (ItemBeingHovered->NumericData.IsStackable)
	{
		MaxStackSize->SetText(FText::AsNumber(ItemBeingHovered->NumericData.MaxStackSize));
	}
	else
	{
		MaxStackSizeText->SetVisibility(ESlateVisibility::Collapsed);
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);	
	}
	
}






