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
	// 1. 공격 시작
	void EnableCollision();

	// 2. 공격 끝
	void DisableCollision();

	// 3. 매 프레임 판정 검사
	void ExecuteTrace(FHitResult& HitResult);

	// 4. 데미지랑 범위
	float AttackDamage = 20.0f;
	float AttackRadius = 30.0f;

protected:
	// 이미 맞은 적을 기억하는 배열
	UPROPERTY()
	TArray<AActor*> HitActors;

	// 소켓 이름
	FName SocketStart = FName("TraceStart");
	FName SocketEnd = FName("TraceEnd");
};