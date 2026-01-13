// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay() {
	Super::BeginPlay();
}

void AEnemyAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// 1. 플레이어 찾기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// 2. 만약 플레이어가 있고, NavMesh가 깔려 있다면
	if (PlayerPawn) {
		// MoveToActor(누구한테, 멈출 거리);
		// AcceptanceRadius: 50.0f (50cm 앞에서 멈춰라)
		MoveToActor(PlayerPawn, 50.0f);
	}
}
