// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController() {
	// 1. 컴포넌트 생성
	EnemyPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (EnemyPerception && SightConfig) {
		// 2. 시야 설정
		SightConfig->SightRadius = 1000.0f; // 10미터까지 보임
		SightConfig->LoseSightRadius = 1200.0f; // 12 미터 멀어지면 놓침
		SightConfig->PeripheralVisionAngleDegrees = 60.0f; // 시야각 (양옆 60도)

		// 3. 감지 대상 설정 (적, 중립, 아군 다 감지하게 설정해야 안전)
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		// 4. 눈에 설정을 적용
		EnemyPerception->ConfigureSense(*SightConfig);
		EnemyPerception->SetDominantSense(SightConfig->GetSenseImplementation());

		// 5. 함수 연결
		EnemyPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
	}
}

void AEnemyAIController::BeginPlay() {
	Super::BeginPlay();

	// BehaviorTree가 지정되어 있다면 실행
	if (AIBehavior) {
		RunBehaviorTree(AIBehavior);
	}
}

void AEnemyAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {
	// 감지된 놈이 플레이어인지 확인? (태그나 클래스로 확인 가능)
	// 여기서는 간단하게 Actor가 null이 아니면 쫓아가게 함
	if (Actor) {
		// Stimulus.WasSuccessfullySensed():
		// true = 방금 눈에 들어왔다 / false = 시야에서 사라졌다
		if (Stimulus.WasSuccessfullySensed()) {
			// 발견, 블랙보드에 기록 -> 쫓아감
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
			UE_LOG(LogTemp, Warning, TEXT("I see you! %s"), *Actor->GetName());
		}
		else {
			// 놓침! 블랙보드 지움 -> 멈춤
			GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
			UE_LOG(LogTemp, Warning, TEXT("Where did he go?"));
		}
	}
}