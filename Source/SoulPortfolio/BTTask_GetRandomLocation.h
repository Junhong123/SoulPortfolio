// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetRandomLocation.generated.h"

/**
 *
 */
UCLASS()
class SOULPORTFOLIO_API UBTTask_GetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_GetRandomLocation();

	// Task가 실행될 때 호출되는 진짜 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 에디터에서 순찰 반경을 조절할 수 있게 만듦 (기본값 15미터)
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float SearchRadius = 1500.0f;
};
