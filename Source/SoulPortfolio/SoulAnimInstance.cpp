// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void USoulAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	// 소유 폰 가져오기
	APawn* OwningPawn = TryGetPawnOwner();

	if (OwningPawn) {
		// ACharacter로 형변환하여 저장
		SoulCharacter = Cast<ACharacter>(OwningPawn);

		if (SoulCharacter) {
			SoulCharacterMovement = SoulCharacter->GetCharacterMovement();
		}
	}
}

void USoulAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);

	// 널 포인터 방어 코드
	if (SoulCharacter == nullptr) {
		SoulCharacter = Cast<ACharacter>(TryGetPawnOwner());
		if (SoulCharacter) {
			SoulCharacterMovement = SoulCharacter->GetCharacterMovement();
		}
	}

	// 참조가 유효할 때만 값 갱신
	if (SoulCharacter && SoulCharacterMovement) {
		// 1. 속도 계산 (Z축 제외)
		// 소울라이크 이동은 주로 평면 이동이므로 수직 속도는 배제
		FVector Velocity = SoulCharacterMovement->Velocity;
		Velocity.Z = 0.f;
		GroundSpeed = Velocity.Size();
		
		// 2. 공중 상태 확인
		bIsFalling = SoulCharacterMovement->IsFalling();

		// 3. 가속 상태 확인 (입력 여부)
		// Velocity와 달리 CurrentAcceleration은 플레이어의 입력 의도를 나타낸다.
		// 벽에 막혀 속도가 0이어도 가속도는 존재할 수 있다 (미는 동작 등에 활용).
		bIsAccelerating = SoulCharacterMovement->GetCurrentAcceleration().Size() > 0.f;

		// 4. 이동 방향 계산 (스트레이핑)
		// 캐릭터의 회전과 이동 벡터 사이의 각도를 계산한다.
		FRotator Rotation = SoulCharacter->GetActorRotation();

		// Kismet 라이브러리를 사용하거나 직접 UnrotateVector를 사용할 수 있다.
		Direction = CalculateDirection(Velocity, Rotation);
	}
}