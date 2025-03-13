// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Interaction/InteractionInterface.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


class UInventoryComponent;
class ABasicHUD;
class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UBackTrackingComponent;


USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData():CurrentInteractable(nullptr),LastInteractionCheckTime(0.0f){};
	
	UPROPERTY()
	AActor* CurrentInteractable;
	
	UPROPERTY()
	float LastInteractionCheckTime;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeEvent,float,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeReverseDelegate, bool ,IsTimeReversing);
UCLASS()
class DEMOWITHRIDER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
		// Sets default values for this character's properties
		APlayerCharacter();

	    FOnHealthChangeEvent OnHealthChange;
	//======================================================================
	// FUNCTIONS
	//======================================================================

	FORCEINLINE bool IsInteracting()const{return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction);};

	FORCEINLINE UInventoryComponent* GetInventory() const{return PlayerInventory;}

	void UpdateInteractionWidget() const;

	USkeletalMeshComponent* GetGunComponent() const;
	protected:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		UPROPERTY()
		ABasicHUD* HUD;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float ChaosWorldPlz;

		UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
		TScriptInterface<IInteractionInterface> TargetInteractable;

		UPROPERTY(VisibleAnywhere,Category=" Character | Inventory")
		UInventoryComponent* PlayerInventory;


		float InteractionCheckFrequency;
	
		float InteractionCheckDistance;

		FTimerHandle TimerHandle_Interaction;

		FInteractionData InteractionData;

		UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USkeletalMeshComponent* GunComponent;	
	
	//======================================================================
	// FUNCTIONS
	//======================================================================
		void PerformInteractionCheck();
		void FoundInteractable(AActor* NewInteractable);
		void NoInteractableFound();
		void BeginInteract();
		void EndInteract();
		void Interact();

		UFUNCTION(BlueprintCallable)
		void ToggleMenu();

	
		void Move(const FInputActionValue& Value);
		void Look(const FInputActionValue& Value);
		/*
		void Jump(const FInputActionValue& Value);
		*/
		void Shoot(const FInputActionValue& Value);
		void Lock(const FInputActionValue& Value);

		

	private:
	//======================================================================
	// PROPERTIES & VARIABLES
	//======================================================================
		UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess="true"))
		TObjectPtr<USpringArmComponent>CameraBoom;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCameraComponent>PlayerCamera;
		
		//Input
		UPROPERTY(EditDefaultsOnly,Category="Input | Base")
		TObjectPtr<UInputMappingContext>DefaultMapping;

		UPROPERTY(EditDefaultsOnly,Category="Input | Base")

		TObjectPtr<UInputAction>LookAction;

		UPROPERTY(EditDefaultsOnly,Category="Input | Base")
		TObjectPtr<UInputAction>MoveAction;
		/*UPROPERTY(EditDefaultsOnly, Category = "Input")
		 *TObjectPtr<UInputAction>JumpAction;*/
		UPROPERTY(EditDefaultsOnly,Category="Input | Base")
		TObjectPtr<UInputAction>ShootAction;
		UPROPERTY(EditDefaultsOnly, Category = "Input | Lock")
		TObjectPtr<UInputAction>LockAction;
		UPROPERTY(EditDefaultsOnly, Category = "Input | TimeReverse")
		TObjectPtr<UInputAction>TimeReverseAction;
		UPROPERTY(EditDefaultsOnly, Category = "Input | Interaction")
		TObjectPtr<UInputAction>InteractionAction;
		UPROPERTY(EditDefaultsOnly, Category = "Input | Interaction")
		TObjectPtr<UInputAction>ToggleMenuAction;
	public:

	// Function to get the camera boom component
	USpringArmComponent* GetCameraBoom() const;
 
	// Function to get the follow camera component
	UCameraComponent* GetFollowCamera() const;

	//======================================================================
	// FUNCTIONS
	//======================================================================
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartTimeReverse(const FInputActionValue& Value);
	void StopTimeReverse(const FInputActionValue& Value);
	FTimeReverseDelegate TimeReverseDelegate;


	
};