// Fill out your copyright notice in the Description page of Project Settings.


// Game
#include "Player/PlayerCharacter.h"
#include "DemoWithRider/Public/UserInterface/Interaction/InteractionInterface.h"
#include "UserInterface/BasicHUD.h"
#include "UserInterface/Inventory/InventoryComponent.h"

// Engine
#include "Camera/CameraComponent.h"
#include"GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h"



// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//第三人称可以通过类似代码设置Camera 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f;
	/*
	CameraBoom->SetRelativeLocation(FVector(0.f, -15.f, 100.f));
	*/
	CameraBoom->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	//SetRootComponent(CameraBoom); 如果希望相机附着到Boom
	PlayerCamera=CreateDefaultSubobject<UCameraComponent>("Player Camera");
	PlayerCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);

	//1、不要让角色跟着控制器旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	//2、相机杆旋转 而相机不旋转
	CameraBoom->bUsePawnControlRotation=true;
	PlayerCamera->bUsePawnControlRotation=true;
	//角色根据运动方向旋转
	GetCharacterMovement()->bOrientRotationToMovement=true;
	GetCharacterMovement()->RotationRate=FRotator(0.f, 400.f, 0.f);

	// Interact LineChannel 
	InteractionCheckFrequency=0.1;
	InteractionCheckDistance=225.0f;
	BaseEyeHeight=74.0f;


	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(78.0f);
	
}

USpringArmComponent* APlayerCharacter::GetCameraBoom() const
{
	return CameraBoom.Get();
}

UCameraComponent* APlayerCharacter::GetFollowCamera() const
{
	return PlayerCamera.Get();
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if ( const ULocalPlayer* Player= (GEngine&&GetWorld()) ? GEngine->GetFirstGamePlayer(GetWorld()):nullptr  ) {
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player);
		if (DefaultMapping) {
			Subsystem->AddMappingContext(DefaultMapping, 0);
		}
	}

	HUD=Cast<ABasicHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

}




// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime)>InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

}

void APlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	// 射线检测，严查BUG
	/*FVector TraceStart{PlayerCamera->GetComponentLocation()};*/
	/*FVector TraceEnd{TraceStart+(PlayerCamera->GetComponentRotation().Vector()*InteractionCheckDistance)};*/
	
	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{TraceStart+(GetViewRotation().Vector()*InteractionCheckDistance)};
	float LookDirection=FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
		
	if (LookDirection>0)
	{
		DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Red,false,1.0f,0,2.0f);
	
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit,TraceStart,TraceEnd,ECC_Visibility,QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor()	!=	InteractionData.CurrentInteractable	)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}
				if (TraceHit.GetActor() ==	InteractionData.CurrentInteractable)
				{
					return;
				}
			
			}
		}
	}
	 NoInteractableFound();
}

void APlayerCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}
	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable=InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}
	InteractionData.CurrentInteractable=NewInteractable;
	TargetInteractable=NewInteractable;

	//Update	interactable widget
	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	
	TargetInteractable->BeginFocus();
	/*
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,"Start Focus Successfully");
*/
}

void APlayerCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
			/*
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,"NoInteractableFound Success");
		*/
		}
		//Hide Interaction widget on the HUD
		HUD->HideInteractionWidget();
		
		InteractionData.CurrentInteractable=nullptr;
		TargetInteractable=nullptr;
	}
}

void APlayerCharacter::BeginInteract()
{
	PerformInteractionCheck();
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration,0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
				this,
				&APlayerCharacter::Interact,
				TargetInteractable->InteractableData.InteractionDuration,
				false);
			}
		}
	}
}

void APlayerCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void APlayerCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}


void APlayerCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

USkeletalMeshComponent* APlayerCharacter::GetGunComponent() const
{
	return GunComponent;
}

void APlayerCharacter::ToggleMenu() 
{
	HUD->ToggleMenu();
	/*GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,"ToggleMenu Success");*/
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MoveVector.X);
		AddMovementInput(RightDirection, MoveVector.Y);

	}
}
/*void APlayerCharacter::Jump(const FInputActionValue& Value)
{
	
}*/
void APlayerCharacter::Shoot(const FInputActionValue& Value)
{
	
}
void APlayerCharacter::Lock(const FInputActionValue& Value)
{
	
}



void APlayerCharacter::StartTimeReverse(const FInputActionValue& Value)
{
	TimeReverseDelegate.Broadcast(true); 
}

void APlayerCharacter::StopTimeReverse(const FInputActionValue& Value)
{
	TimeReverseDelegate.Broadcast(false);
}



void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller) {
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}



// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		/*EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);*/
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &APlayerCharacter::Shoot);
		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Started, this, &APlayerCharacter::Lock);
		EnhancedInputComponent->BindAction(TimeReverseAction,ETriggerEvent::Started,this,&APlayerCharacter::StartTimeReverse);
		EnhancedInputComponent->BindAction(TimeReverseAction,ETriggerEvent::Completed,this,&APlayerCharacter::StopTimeReverse);

		EnhancedInputComponent->BindAction(InteractionAction,ETriggerEvent::Started,this,&APlayerCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractionAction,ETriggerEvent::Completed,this,&APlayerCharacter::EndInteract);

		EnhancedInputComponent->BindAction(ToggleMenuAction,ETriggerEvent::Started,this,&APlayerCharacter::ToggleMenu);
	}
}

