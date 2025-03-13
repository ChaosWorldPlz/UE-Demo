// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UserInterface/Interaction/InteractionInterface.h"
#include "Pickup.generated.h"


class UItemBase;
class UDataTable;
UCLASS()
class DEMOWITHRIDER_API APickup : public AActor,public IInteractionInterface
{


private:
	GENERATED_BODY()

public:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================

	//======================================================================
	// FUNCTIONS
	//======================================================================
	APickup();
	void InitializePickup(const TSubclassOf<UItemBase> BaseClass,const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop,const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData(){return ItemReference;}

	virtual void BeginFocus() override;

	virtual void EndFocus() override;

	
protected:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
UPROPERTY(VisibleAnywhere,category="Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly,category="Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly,category="Pickup | Item Database")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere,category="Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly,category="Pickup | Item Reference")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly,category="Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	//======================================================================
	// FUNCTIONS
	//======================================================================
	virtual void BeginPlay() override;
	
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	void UpdateInteractableData();
	void TakePickup(const APlayerCharacter* Taker);

	//这段代码的作用是：在编辑器模式下，当对象属性在编辑器中被修改后，通过重写 PostEditChangeProperty
	//函数来执行特定的响应逻辑。条件编译的使用确保了这些编辑器功能不会对非编辑器模式（如游戏运行）产生影响。
	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
};
