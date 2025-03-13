// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BackTrackingComponent.generated.h"

struct SpaceInfo
{
	FVector Location;
	FRotator Rotation;
	FVector LinearVelocity;
	FVector AngularVelocity;
	float DeltaTime;
	
	SpaceInfo(FVector inLocation,FRotator inRotation,FVector inLinearVelocity,FVector inAngularVelocity,float inDeltaTime):
	Location(inLocation),Rotation(inRotation),LinearVelocity(inLinearVelocity),AngularVelocity(inAngularVelocity),DeltaTime(inDeltaTime){}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ,Blueprintable )
class DEMOWITHRIDER_API UBackTrackingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBackTrackingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* Owner;
	bool IsBackTracking;
	bool IsFinished;
	bool IsInit;
	float RecordTimeLength;
	TDoubleLinkedList<SpaceInfo> SpaceInfoFrames;

	UFUNCTION()
	void SetBackTracking(bool isBackTracking);
	
};
