// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "EnemyAIController.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 적의 충돌 설정 (무기가 감지할 수 있도록)
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// 이 캐릭터가 스폰되면 자동으로 AI 컨트롤러가 빙의
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 체력 설정 할 부품 생성
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint) {
	// 1. 소리 재생
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	// 2. 이펙트 재생
	if (HitParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}

	// 일단 로그로 확인
	UE_LOG(LogTemp, Warning, TEXT("Enemy was hit at: %s"), *ImpactPoint.ToString());
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	// 이미 죽었으면 무시
	if (Attributes && Attributes->IsAlive()) {
		// 체력 깎아라 (살았으면 true, 죽었으면 false)
		if (!Attributes->ReceiveDamage(DamageAmount)) {
			// 죽음 처리
			Die();
		}
	}

	return DamageAmount;
}

void AEnemy::Die() {
	if (bIsDead) return; // 두 번 죽는 거 방지

	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Died!"));

	// 1. AI 뇌 정지
	if (AAIController* AIController = Cast<AAIController>(GetController())) {
		AIController->StopMovement();
		// 필요하면 BrainComponent도 멈춤
	}

	// 2. 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	float MontageDuration = 0.0f;

	if (AnimInstance && DeathMontage) {
		MontageDuration = AnimInstance->Montage_Play(DeathMontage);
	}

	// 3. 타이머 설정
	float RagdollDelay = (MontageDuration > 0.5f) ? (MontageDuration - 0.5f) : 0.1f;

	GetWorldTimerManager().SetTimer(
		RagdollTimerHandle,
		this,
		&AEnemy::EnableRagdoll, // 이 함수를 실행해라
		RagdollDelay,			// 이 시간 뒤에
		false					// 반복하지 마라
	);
}

void AEnemy::EnableRagdoll() {
	// 진짜 물리 키기
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // 플레이어 발에 걸리게
	GetMesh()->SetSimulatePhysics(true);

	SetLifeSpan(5.0f);
}