// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "AttributeComponent.h"
#include "Enemy.generated.h"

UCLASS()
class SOULPORTFOLIO_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// HitInterface 함수 구현 (override)
	virtual void GetHit(const FVector& ImpactPoint) override;

private:
	// 이펙트(피/불꽃)
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitParticles;

	// 타격 사운드
	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* HitSound;

	// 체력 부품
	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

	// 죽었을 때 상태 관리
	bool bIsDead = false;

	// 죽는 애니메이션
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* DeathMontage;

	// 언리얼 엔진의 기본 데미지 처리 함수 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 죽음 처리 함수
	void Die();

	// 랙돌을 켜는 전용 함수
	void EnableRagdoll();

	// 타이머를 관리할 핸들
	FTimerHandle RagdollTimerHandle;
};
