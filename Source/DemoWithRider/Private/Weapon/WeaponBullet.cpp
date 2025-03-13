// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBullet.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AWeaponBullet::AWeaponBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("BulletMovement"));
}

// Called when the game starts or when spawned
void AWeaponBullet::BeginPlay()
{
	Super::BeginPlay();
	
}


