// Fill out your copyright notice in the Description page of Project Settings.

#include "UserInterface/Interaction/InterfaceTestActor.h"

// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh=CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	// RootComponent couldnt never delete but be replaced by its childcomponent.
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
	
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AInterfaceTestActor::BeginFocus()
{
	if (Mesh)
	{
		// Mesh HighLight
		Mesh->SetRenderCustomDepth(true	);
	}
}

void AInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false	);
	}
}

void AInterfaceTestActor::BeginInteract()
{
	UE_LOG(LogTemp,Warning,TEXT("Calling BeginInteract override on interface test actor"));
}

void AInterfaceTestActor::EndInteract()
{
	UE_LOG(LogTemp,Warning,TEXT("Calling EndInteract override on interface test actor"));
}

void AInterfaceTestActor::Interact(APlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp,Warning,TEXT("Calling Interact override on interface test actor"));
}

