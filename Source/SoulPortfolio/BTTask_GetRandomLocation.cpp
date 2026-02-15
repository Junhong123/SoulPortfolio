// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "AITypes.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation() {
	NodeName = TEXT("Get Random Location"); // 비헤이비어 트리에 보일 이름
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	// 1. AI 컨트롤러와 늑대(Pawn) 정보 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	// 2. 현재 월드의 네비게이션 시스템 가져오기
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	FVector HomeLocation = BlackboardComp->GetValueAsVector(TEXT("HomeLocation"));

	if (HomeLocation == FAISystem::InvalidLocation) {
		HomeLocation = AIPawn->GetActorLocation();
		BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), HomeLocation);

		UE_LOG(LogTemp, Warning, TEXT("HomeLocation complete"));
	}

	// 3. 무작위 좌표 찾기
	FNavLocation RandomLocation;
	// HomeLocation을 중심으로 SearchRadius 안에서 갈 수 있는 곳 찾기
	if (NavSystem->GetRandomReachablePointInRadius(HomeLocation, SearchRadius, RandomLocation)) {
		// 4. 찾은 좌표를 블랙보드(PatrolLocation)에 적어두기
		BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

