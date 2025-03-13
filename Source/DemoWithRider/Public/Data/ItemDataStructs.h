// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

/**
 * 
 */
UENUM()
enum class EItemQuality : uint8
{
	/*Rarity Level*/
	Rarity_N UMETA(DisplayName = "Rarity_Normal"),
	Rarity_R UMETA(DisplayName = "Rarity_Rare"),
	Rarity_SR UMETA(DisplayName = "Rarity_SuperRare"),
	Rarity_SSR UMETA(DisplayName = "Rarity_SuperiorSuperRare"),
	Rarity_UR UMETA(DisplayName = "Rarity_UltraRare")
};

UENUM()
enum class EItemType : uint8
{
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield"),
	Spell UMETA(DisplayName = "Spell"),
	Consumable UMETA(DisplayName = "Consumable"),
	Quest UMETA(DisplayName = "Quest"),
	Mundane UMETA(DisplayName = "Mundane")
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float ArmorRating;
	
	UPROPERTY(EditAnywhere)
	float DamageValue;
	
	UPROPERTY(EditAnywhere)
	float RestorationAmount;
	
	UPROPERTY(EditAnywhere)
	float SellValue;
	
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;
	
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()
   UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool IsStackable;
	
};
USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT()
struct FItemData :public  FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category="Item Data")
	EItemQuality Quality;

	UPROPERTY(EditAnywhere,Category="Item Data")
	FName ID;
	
	UPROPERTY(EditAnywhere,Category="Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere,Category="Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere,Category="Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere,Category="Item Data")
	FItemAssetData AssetData;

	UPROPERTY(EditAnywhere,Category="Item Data")
	FItemStatistics Statistics;
	
};
