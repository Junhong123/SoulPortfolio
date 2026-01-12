// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulWeapon.h"
#include "Kismet/KismetSystemLibrary.h" // 스피어 트레이스용
#include "Kismet/GameplayStatics.h"     // 데미지 전달용
#include "Engine/DamageEvents.h"        // 데미지 이벤트용

// Sets default values
ASoulWeapon::ASoulWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // Notify가 대신 해줌

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// 무기 자체 물리 충돌을 끄고, 오직 트레이스로만 판정
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ASoulWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// 1. 공격 시작 : 피격 목록 초기화
void ASoulWeapon::EnableCollision() {
	HitActors.Empty();
}

// 2. 공격 종료
void ASoulWeapon::DisableCollision() {
	// 필요하면 공격 이펙트를 끄거나 가능
}

// 3. 트레이스 발사
void ASoulWeapon::ExecuteTrace(FHitResult& HitResult) {
	// 소켓 위치 가져오기 (소켓이 없으면 메쉬의 시작/끝으로 대체됨)
	FVector Start = WeaponMesh->GetSocketLocation(SocketStart);
	FVector End = WeaponMesh->GetSocketLocation(SocketEnd);

	// 나 자신과 주인(캐릭터), 그리고 이미 맞은 적은 무시
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* HitActor : HitActors) {
		ActorsToIgnore.AddUnique(HitActor);
	}

	// 스피어 트레이스 발사
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		AttackRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn), // 적(Pawn)만 감지하도록
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, // 테스트용 빨간 선 그리기 (나중에 None으로)
		HitResult,
		true
	);

	// 맞았다면
	if (bHit) {
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && !HitActors.Contains(HitActor)) {
			// 1. 목록에 추가 (중복 방지)
			HitActors.Add(HitActor);

			// 2. 데미지 전달
			UGameplayStatics::ApplyDamage(
				HitActor,
				AttackDamage,
				GetOwner()->GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);

			// 3. 로그 (확인용)
			UE_LOG(LogTemp, Warning, TEXT("Hit Enemy: %s"), *HitActor->GetName());
		}
	}
}
