// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainMenu.h"

#include "Player/PlayerCharacter.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter	=	Cast<APlayerCharacter>(GetOwningPlayerPawn());
}

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	//cast operaction to item drag drop , ensure player is valid , call drop item on player
}
