// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No Item Added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial Amount Item Added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of Item Added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	// Initialize
	FItemAddResult():
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};

	// Actual amount of  item that was  added to the inventory 
	UPROPERTY(BlueprintReadOnly , Category= "Item Add Result" )
	int32 ActualAmountAdded;
	// Enum representing the end state of an add item operation	
	UPROPERTY(BlueprintReadOnly , Category= "Item Add Result" )
	EItemAddResult OperationResult;
	// Informational message that can be passed with the result	
	UPROPERTY(BlueprintReadOnly , Category= "Item Add Result" )
	FText ResultMessage;

	static  FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};
	static  FItemAddResult AddedPartial(const int32 PartialAmountAdded,const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};
	static  FItemAddResult AddedAll(const int32 AmountAdded,const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMOWITHRIDER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
	FOnInventoryUpdated OnInventoryUpdated;
	



	
	//======================================================================
	// FUNCTIONS
	//======================================================================
	UInventoryComponent();

	UFUNCTION(category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);
	
	UFUNCTION(category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;
	UFUNCTION(category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;
	UFUNCTION(category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

		
	UFUNCTION(category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);
	UFUNCTION(category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemIn,int32 DesiredAmountToRemove);	
	UFUNCTION(category = "Inventory")
	void SplitExistingStack(UItemBase* ItemIn,const int32 AmountToSplit);


	//Getters
	//----------------------------------------------------------------------
	UFUNCTION(category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const{	return InventoryTotalWeight; };	
	UFUNCTION(category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const{	return InventoryWeightCapacity; };	
	UFUNCTION(category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const{	return InventorySlotsCapacity; };	
	UFUNCTION(category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const{	return InventoryContents; };
	
	//Setters
	//----------------------------------------------------------------------	
	UFUNCTION(category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity){	InventorySlotsCapacity = NewSlotsCapacity; };	
	UFUNCTION(category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity){	InventoryWeightCapacity = NewWeightCapacity; };
	
protected:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
	UPROPERTY(VisibleAnywhere,Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(VisibleAnywhere,Category = "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(VisibleAnywhere,Category = "Inventory")
	float InventoryWeightCapacity;

	UPROPERTY(VisibleAnywhere,Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;
	
	//======================================================================
	// FUNCTIONS
	//======================================================================
	virtual void BeginPlay() override;
	FItemAddResult HandleNonStackableItems(UItemBase* InputItem);
	int32 HandleStackableItems(UItemBase* ItemIn,int32 RequestedAddAmount);
	int32 CalculateWeightCapacity(UItemBase* ItemIn,int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* StackableItem,int32 InitialRequestedAddAmount);

	void AddNewItem(UItemBase* Item,const int32 AmountToAdd);
};
