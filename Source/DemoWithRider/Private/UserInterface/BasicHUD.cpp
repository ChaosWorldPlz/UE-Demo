// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/BasicHUD.h"

#include "ViewportInteractableInterface.h"
#include "Blueprint/UserWidget.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"


ABasicHUD::ABasicHUD()
{
}

void ABasicHUD::BeginPlay()
{
	Super::BeginPlay();


	// Create all the widgets in need and set them collapsed
	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(),MainMenuClass);
		/*MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(),UMainMenu::StaticClass());*/
		// ZOrder: Layer priority , The higher the number, the more on top this widget will be.
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (InteractionWidgetClass)
	{
		InteractionWidget= CreateWidget<UInteractionWidget>(GetWorld(),InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	
}

void ABasicHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible=true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABasicHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible=false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABasicHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		//Close Menu
		HideMenu();
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}else
	{
		//Open Menu
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ABasicHUD::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABasicHUD::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABasicHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}
		InteractionWidget->UpdateWidget(InteractableData);
	}
}
