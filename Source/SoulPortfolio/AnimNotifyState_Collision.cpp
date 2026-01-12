// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Collision.h"
#include "SoulCharacter.h"
#include "SoulWeapon.h"

void UAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 캐릭터를 통해 무기 가져오기
	if (ASoulCharacter* Player = Cast<ASoulCharacter>(MeshComp->GetOwner())) {
		if (ASoulWeapon* Weapon = Player->EquippedWeapon) {
			Weapon->EnableCollision();
		}
	}
}

void UAnimNotifyState_Collision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (ASoulCharacter* Player = Cast<ASoulCharacter>(MeshComp->GetOwner())) {
		if (ASoulWeapon* Weapon = Player->EquippedWeapon) {
			FHitResult HitResult;
			Weapon->ExecuteTrace(HitResult);
		}
	}
}

void UAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ASoulCharacter* Player = Cast<ASoulCharacter>(MeshComp->GetOwner())) {
		if (ASoulWeapon* Weapon = Player->EquippedWeapon) {
			Weapon->DisableCollision();
		}
	}
}

