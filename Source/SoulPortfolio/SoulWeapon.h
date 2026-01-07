// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoulWeapon.generated.h"

UCLASS()
class SOULPORTFOLIO_API ASoulWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoulWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 무기 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
