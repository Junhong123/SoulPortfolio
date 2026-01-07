#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h" // 입력 값을 사용하기 위해 추가
#include "CharacterTrajectoryComponent.h"
#include "SoulCharacter.generated.h"

// 캐릭터의 행동 상태
UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_Equipping UMETA(DisplayName = "Equipping"),
	ECS_Dead UMETA(DisplayName = "Dead"),
	ECS_Stunned UMETA(DisplayName = "Stunned")
};

// 캐릭터의 행동 모드
UENUM(BlueprintType)
enum class ECharacterActionState : uint8 {
	ECAS_Idle UMETA(DisplayName = "Idle"),
	ECAS_Move UMETA(DisplayName = "Move"),
	ECAS_Dodge UMETA(DisplayName = "Dodge"),
};

UCLASS()
class SOULPORTFOLIO_API ASoulCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// 생성자
	ASoulCharacter();

	// 매 프레임 실행되는 함수
	virtual void Tick(float DeltaTime) override;

	// 입력 바인딩 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 애니메이션 시스템
	// 현재 이동 속도 (XY 평면 기준)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	//현재 캐릭터의 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	ECharacterState CharacterState;

	// 전투 관련 몽타주 애셋
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	class UAnimMontage* AttackMontage;

	// 히트 리액션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	class UAnimMontage* HitReactMontage;

	// 에디터에서 어떤 무기를 낄지 고를 변수
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ASoulWeapon> DefaultWeaponClass;

	// 현재 들고 있는 무기를 저장할 포인터
	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	TObjectPtr<class ASoulWeapon> EquippedWeapon;

	// Trajectory 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	UCharacterTrajectoryComponent* CharacterTrajectory;

	// 서있을때 구르기 몽타주
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* RollStandMontage;

	// 달릴 때 구르기 몽타주
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* RollSprintMontage;

protected:
	// 게임 시작 시 실행되는 함수
	virtual void BeginPlay() override;

	/* 입력 관련 변수들 (에디터에서 할당) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* RollAction;

	/** 실제 움직임을 처리할 함수들 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Roll();
	void OnCrouch();
	void SprintStart(const FInputActionValue& Value);
	void SprintStop(const FInputActionValue& Value);
	void PerformAttack(const FInputActionValue& Value);

private:
	// 카메라 지지대 (스프링 암)
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	// 카메라
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* ViewCamera;

	// 모듈형 캐릭터 파츠들
	UPROPERTY(VisibleAnywhere, Category = "Character Part")
	TObjectPtr<USkeletalMeshComponent> HelmetMesh;

	UPROPERTY(VisibleAnywhere, Category = "Character Part")
	TObjectPtr<USkeletalMeshComponent> HandMesh;

	UPROPERTY(VisibleAnywhere, Category = "Character Part")
	TObjectPtr<USkeletalMeshComponent> LegMesh;

	UPROPERTY(VisibleAnywhere, Category = "Character Part")
	TObjectPtr<USkeletalMeshComponent> BootMesh;
};