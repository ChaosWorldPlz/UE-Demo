// Fill out your copyright notice in the Description page of Project Settings.


#include "BackTrackingSystem/BackTrackingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Player/PlayerCharacter.h"

// Sets default values for this component's properties
UBackTrackingComponent::UBackTrackingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Owner=nullptr;
	IsBackTracking=false;
	IsFinished=false;
	IsInit=false;
	RecordTimeLength=0.0f;
}


// Called when the game starts
void UBackTrackingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!IsInit)
	{
		Owner=GetOwner();
		IsInit=true;
		APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
		player->TimeReverseDelegate.AddDynamic(this,&UBackTrackingComponent::SetBackTracking);
	}
}


// Called every frame
void UBackTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsInit){return;}
	if (!IsBackTracking)
	{
		TArray<UActorComponent*> compArray= Owner->K2_GetComponentsByClass(UStaticMeshComponent::StaticClass());
		if (compArray.Num() > 0)
		{
			UStaticMeshComponent* usmc = Cast<UStaticMeshComponent>(compArray[0]);
			if (usmc)
			{
				SpaceInfo NewFrame(
					Owner->GetActorLocation(),
					Owner->GetActorRotation(),
					usmc->GetPhysicsLinearVelocity(),
					usmc->GetPhysicsAngularVelocityInDegrees(),
					DeltaTime
				);
				if (RecordTimeLength <10.0f)
				{
					SpaceInfoFrames.AddTail(NewFrame);
					RecordTimeLength += DeltaTime;
					IsFinished=false;
				}
				else
				{
					auto HeadNode = SpaceInfoFrames.GetHead();
					float HeadDeltaTime = HeadNode->GetValue().DeltaTime;
					SpaceInfoFrames.RemoveNode(HeadNode);
					RecordTimeLength -= HeadDeltaTime;

					SpaceInfoFrames.AddTail(NewFrame);
					RecordTimeLength += DeltaTime;
					IsFinished=false;	
				}
			}
		}
	}
	else if (!IsFinished)
	{
		auto HeadNode = SpaceInfoFrames.GetHead();
		auto TailNode = SpaceInfoFrames.GetTail();
		if (HeadNode && TailNode && HeadNode==TailNode) //List is empty
		{
			RecordTimeLength=0.0f;
			IsFinished=true;
		}
		else if (TailNode)  
		{
			Owner->SetActorLocation(TailNode->GetValue().Location);
			Owner->SetActorRotation(TailNode->GetValue().Rotation);
			TArray<UActorComponent*> comArray=Owner->K2_GetComponentsByClass(UStaticMeshComponent::StaticClass());
			if (comArray.Num() > 0)
			{
				UStaticMeshComponent* usmc = Cast<UStaticMeshComponent>(comArray[0]);
				if (usmc)
				{
					usmc->SetPhysicsLinearVelocity(TailNode->GetValue().LinearVelocity);
					usmc->SetPhysicsAngularVelocityInDegrees(TailNode->GetValue().AngularVelocity);
				}
			}
			RecordTimeLength -= TailNode->GetValue().DeltaTime;
			SpaceInfoFrames.RemoveNode(TailNode);
		}
	}
}

void UBackTrackingComponent::SetBackTracking(bool isBackTracking)
{
	IsBackTracking=isBackTracking;
	/*if (isBackTracking)
	{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,FString::FromInt(IsBackTracking));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,FString::FromInt(IsBackTracking));*/
}

