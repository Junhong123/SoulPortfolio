// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionData.generated.h"

/**
 *
 */
UCLASS()
class SOULPORTFOLIO_API UActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 공격 애니메이션 몽타주
	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = "Data")
	class UAnimMontage* AttackMontage;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Radius = 30.0f;
};