// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class DEMOWITHRIDER_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	//======================================================================
	// FUNCTIONS
	//======================================================================


protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation) override;
};
