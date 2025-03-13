// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryComponent.h"

#include "Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn )
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}


// make sure there is a unfilled stack for place item 
UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
		{
			return InventoryItem -> ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
		}
		))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightCapacity(UItemBase* ItemIn, int32 RequestedAddAmount)
{

	// the most number of such a type of item could be added in
	const int32 WeightMaxAddAmount =
		FMath::FloorToInt((GetWeightCapacity()-InventoryTotalWeight)/ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack =
		StackableItem->NumericData.MaxStackSize - StackableItem->Quantity ;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.Remove(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove =
		FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove* ItemIn->GetItemSingleWeight();
	
	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove; 
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	// when Inventory exceed maximum capacity
	if (!(InventoryContents.Num() + 1 >InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn,AmountToSplit);
	}
	
	
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
	// the function "IsNearlyZero" is always used in the field of physically movement
	//避免除以零的错误 ;	判断物体是否静止	;	平滑过渡和避免奇点	; 数值比较，差值足够小则认为相等
	// check if in the InputItem has valid weight
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight())|| InputItem->GetItemStackWeight()<0)
	{
		//return added none
		return FItemAddResult::AddedNone(FText::Format(
		FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."),
					InputItem->TextData.Name));
	}
	// will the item weight overflow weight capacity
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight()	> GetWeightCapacity())
	{
		//return added none
		// InputItem->TextData.Nmae  >> {0}
		return FItemAddResult::AddedNone(FText::Format(
		FText::FromString("Could not add {0} to the inventory. Item would overflow weight capacity."),
					InputItem->TextData.Name));
	}
	// Adding one more item would overflow slot capacity
	if (InventoryContents.Num() + 1 >InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
		FText::FromString("Could not add {0} to the inventory. Item would overflow slot capacity."),
					InputItem->TextData.Name));
	}
	AddNewItem(InputItem, 1);
	return FItemAddResult::AddedAll(1,FText::Format(
		FText::FromString("Successfully added a single {0}  to the inventory."),
			 InputItem->TextData.Name ));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	return 0;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		//   for non-stackable items
		if (InputItem->NumericData.IsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}
		//	for stackable items
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			// return  added all result
			return FItemAddResult::AddedAll(InitialRequestedAddAmount,FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."),
				InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0 )
		{
			//return added partial result
			return FItemAddResult::AddedPartial(StackableAmountAdded,FText::Format(
				FText::FromString("Partial add {0} to the inventory. Number added= {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
		}
		if (StackableAmountAdded <= 0)
		{
			//return added none result
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString
				("Failed to added {0}to the inventory.No remaining inventory slots,or invalid item."),
				InputItem->TextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone
		(FText::FromString("TryAddItem fallthrough error.GetOwner() check somehow failed."));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;
	
	if (Item->bIsCopy || Item->bIsPickup)
	{
		// if the item is already a copy , or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}else
	{	// used when splittingor dragging to/from another inventory
		NewItem	= Item->CreateItemCopy();
	}
	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}




