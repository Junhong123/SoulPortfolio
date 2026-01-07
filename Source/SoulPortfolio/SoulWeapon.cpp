// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulWeapon.h"

// Sets default values
ASoulWeapon::ASoulWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // 무기는 보통 Tick 불필요

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// 무기는 캐릭터와 충돌하면 안되므로 충돌 설정은 나중에 'NoCollision'이나 트레이스 채널을 따로 판다
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ASoulWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASoulWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

