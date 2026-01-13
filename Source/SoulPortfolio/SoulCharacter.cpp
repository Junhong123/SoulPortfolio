#include "SoulCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SoulWeapon.h"

// 생성자: 캐릭터의 부품을 조립하는 곳
ASoulCharacter::ASoulCharacter()
{
	// Tick 함수 켜기
	PrimaryActorTick.bCanEverTick = true;

	// 1. 캡슐 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// 2. 캐릭터 회전 설정 (카메라 따라 돌지 않기)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 3. 무브먼트 설정 (이동 방향 바라보기)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	// 점프 및 이동 속도 설정 (소울라이크 느낌)
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// 4. 스프링 암 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// 5. 카메라 설정
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ViewCamera->bUsePawnControlRotation = false;

	// 모듈형 파츠 설정
	// 1. 헬멧 생성
	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetMesh"));
	HelmetMesh->SetupAttachment(GetMesh());
	HelmetMesh->SetLeaderPoseComponent(GetMesh());

	// 2. 장갑 생성
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(GetMesh());
	HandMesh->SetLeaderPoseComponent(GetMesh());

	// 3. 바지 생성
	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	LegMesh->SetupAttachment(GetMesh());
	LegMesh->SetLeaderPoseComponent(GetMesh());

	// 4. 신발 생성
	BootMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BootMesh"));
	BootMesh->SetupAttachment(GetMesh());
	BootMesh->SetLeaderPoseComponent(GetMesh());

	// Trajcetory 컴포넌트
	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("CharacterTrajectory"));

}

// 게임 시작 시 실행 (BeginPlay)
void ASoulCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Input 시스템(IMC) 등록
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 블루프린트에서 IMC를 넣었는지 확인 후 등록
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	// 1. 무기 클래스가 있는지 확인
	if (DefaultWeaponClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		// 2. 월드에 무기 액터를 생성
		ASoulWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ASoulWeapon>(DefaultWeaponClass, GetActorTransform(), SpawnParams);

		if (SpawnedWeapon) {
			EquippedWeapon = SpawnedWeapon;

			// 3. 무기를 캐릭터에 붙임
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			EquippedWeapon->AttachToComponent(GetMesh(), AttachmentRules, FName("WeaponSocket"));
		}
	}
}

// 매 프레임 실행 (Tick)
void ASoulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 속도 계산 [3, 4]
	FVector Velocity = GetVelocity();
	FVector LateralVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
	GroundSpeed = LateralVelocity.Size();

	// 디버깅용: 속도 출력
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Speed: %f"), GroundSpeed));
}

// 입력 키와 함수 연결 (SetupPlayerInputComponent)
void ASoulCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input 컴포넌트로 변환해서 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// [디버깅 1] MoveAction 연결 확인
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulCharacter::Move);
			UE_LOG(LogTemp, Warning, TEXT("MoveAction Binding Success!")); // 성공 로그
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MoveAction is NULL! Check Blueprint!")); // 실패 로그 (빨간색)
		}

		// [디버깅 2] LookAction 연결 확인 (여기가 핵심!)
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoulCharacter::Look);
			UE_LOG(LogTemp, Warning, TEXT("LookAction Binding Success!")); // 성공 로그
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LookAction is NULL! Check Blueprint!")); // 실패 로그 (빨간색)
		}
		if (SprintAction) {
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASoulCharacter::SprintStart);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASoulCharacter::SprintStop);
		}
		if (CrouchAction) {
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASoulCharacter::OnCrouch);
		}
		if (RollAction) {
			EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ASoulCharacter::Roll);
		}
		if (AttackAction) {
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASoulCharacter::PerformAttack);
		}
	}
}

// W, A, S, D 이동 처리 함수
void ASoulCharacter::Move(const FInputActionValue& Value)
{
	if (CharacterState != ECharacterState::ECS_Unoccupied) return;

	// 입력값(X, Y) 가져오기
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 카메라가 보는 방향 알아내기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 앞쪽과 오른쪽 방향 벡터 계산
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 그 방향으로 이동 명령 내리기
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// 마우스 시선 처리 함수
void ASoulCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 마우스 움직임만큼 카메라 회전
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASoulCharacter::SprintStart(const FInputActionValue& Value) {
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ASoulCharacter::SprintStop(const FInputActionValue& Value) {
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ASoulCharacter::OnCrouch() {
	if (bIsCrouched) {
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ASoulCharacter::Roll() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	// 이미 구르는 중이면 패스
	if (AnimInstance->Montage_IsPlaying(RollSprintMontage)) {
		return;
	}

	// 입력 방향으로 즉시 회전
	FVector InputVector = GetLastMovementInputVector();

	if (!InputVector.IsNearlyZero()) {
		FRotator TargetRotation = InputVector.Rotation();

		TargetRotation.Pitch = 0.0f;
		TargetRotation.Roll = 0.0f;

		SetActorRotation(TargetRotation);
	}

	PlayAnimMontage(RollSprintMontage);
}

void ASoulCharacter::PerformAttack(const FInputActionValue& Value) {

	// 1. 무기가 없으면
	if (!EquippedWeapon) return;

	// 2. 만약 Unoccupied 라면
	if (CharacterState == ECharacterState::ECS_Unoccupied) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage) {
			AnimInstance->Montage_Play(AttackMontage);

			// 첫 공격 섹션 이름으로 점프
			AnimInstance->Montage_JumpToSection(FName("Attack1"), AttackMontage);

			CharacterState = ECharacterState::ECS_Attacking;
			ComboCount = 1;
			bHasNextComboInput = false;
		}
	}

	// 3. 만약 이미 공격 중이라면
	else if (CharacterState == ECharacterState::ECS_Attacking) {
		bHasNextComboInput = true;
	}
}

void ASoulCharacter::AttackEnd() {
	// 1. 다음 공격 예약이 있으면
	if (bHasNextComboInput) {
		ComboCount++;

		if (ComboCount > 3) ComboCount = 1;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage) {
			// 섹션 이름 만들기
			FString NextSection = FString::Printf(TEXT("Attack%d"), ComboCount);

			AnimInstance->Montage_JumpToSection(FName(*NextSection), AttackMontage);

			bHasNextComboInput = false;
			UE_LOG(LogTemp, Warning, TEXT("5."));
		}
	}
	else {
		CharacterState = ECharacterState::ECS_Unoccupied;
		ComboCount = 0;
		bHasNextComboInput = false;

		// 몽타주를 부드럽게 멈춰라
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) {
			AnimInstance->Montage_Stop(0.25f, AttackMontage);
		}
	}
}