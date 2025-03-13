// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BasicHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;


UCLASS()
class DEMOWITHRIDER_API ABasicHUD : public AHUD
{

	
	GENERATED_BODY()

public:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
	UPROPERTY(EditDefaultsOnly,Category="Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly	, Category= "Widgets" )
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;
	

	bool bIsMenuVisible;

	//======================================================================
	// FUNCTIONS
	//======================================================================

	ABasicHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();
	

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);



protected:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	//======================================================================
	// FUNCTIONS
	//======================================================================

	virtual void BeginPlay() override;
};
