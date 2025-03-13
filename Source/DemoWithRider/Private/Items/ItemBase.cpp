// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryComponent.h"

UItemBase::UItemBase(): bIsCopy(false),bIsPickup(false)
{
	
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}


UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy =NewObject<UItemBase>(StaticClass());
	
	ItemCopy->ID=this->ID;
	ItemCopy->Quantity=this->Quantity;
	ItemCopy->Statistics=this->Statistics;
	ItemCopy->ItemType=this->ItemType;
	ItemCopy->TextData=this->TextData;
	ItemCopy->NumericData=this->NumericData;
	ItemCopy->AssetData=this->AssetData;
	ItemCopy->ItemQuality=this->ItemQuality;
	ItemCopy->bIsCopy=true;
	
	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity!=this->Quantity)
	{
		Quantity=FMath::Clamp(NewQuantity,0,NumericData.IsStackable?NumericData.MaxStackSize:1);
		if(OwningInventory){
			 if(Quantity<=0){
				 OwningInventory->RemoveSingleInstanceOfItem(this);
			 }
		}
	}
}

void UItemBase::Use(APlayerCharacter* Character)
{
	
}
