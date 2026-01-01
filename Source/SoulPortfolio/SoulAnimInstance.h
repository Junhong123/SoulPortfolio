// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoulAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOULPORTFOLIO_API USoulAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	// 캐릭터 참조 캐싱
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	class ACharacter* SoulCharacter;

	// 이동 컴포넌트 참조 캐싱 (속도, 가속도, 낙하 상태 확인용)
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	class UCharacterMovementComponent* SoulCharacterMovement;

	// 지상 이동 속도
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	// 공중에 떠 있는가?
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling;

	// 가속 중인가? (입력이 있는가?)
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsAccelerating;

	// 이동 방향 (-180 ~ 180도, 스트레이핑용)
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction;
};
