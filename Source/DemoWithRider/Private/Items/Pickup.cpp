// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup.h"

#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryComponent.h"


APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh= CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
	
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	InitializePickup(UItemBase::StaticClass(),ItemQuantity); 
}


void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID,DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this,BaseClass);
		//检索到地面上的可交互Item时从DT库中调信息赋值
		ItemReference->ID = ItemData->ID;
		ItemReference->ItemQuality = ItemData->Quality;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->AssetData = ItemData->AssetData;
		ItemReference->TextData = ItemData->TextData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	//
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action=ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name=ItemReference->TextData.Name;
	InstanceInteractableData.Quantity=ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(APlayerCharacter* PlayerCharacter)
{	// Pickup Anim
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}


void APickup::TakePickup(const APlayerCharacter* Taker)
{	// If Item is gonna be taken or has already been taken that should be destroyed in World  , return true 
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch(AddResult.OperationResult)
				{
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				default:
					break;
				}
				UE_LOG(LogTemp,Warning,TEXT("%s"),*AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("Player Inventory Component Is Null!"));
			}
		}else
		{
			UE_LOG(LogTemp,Warning,TEXT("Pickup internal item was somehow null!"));
		}
	}
}


// When we are in editor model , any item we create by ItemDataTable will get the proper static mesh through
// the item id .
void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// if the Ptr (PropertyChangedEvent.Property ) is valid , GetName
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID))
	{
		if (ItemDataTable)
		{
			const FString ContextString{DesiredItemID.ToString()};

			// if we get a valid row
			if (const FItemData* ItemData =
				ItemDataTable->FindRow<FItemData>(DesiredItemID,DesiredItemID.ToString()))
			{
				 PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}


