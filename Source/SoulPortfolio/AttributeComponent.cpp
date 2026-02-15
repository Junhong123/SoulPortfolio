// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// 기본 체력 설정
	MaxHealth = 100.0f;
	Health = MaxHealth;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UAttributeComponent::ReceiveDamage(float Damage) {
	// 체력 깎기
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), Health);

	// 체력이 0보다 크면 살아있음
	return Health > 0.0f;
}

bool UAttributeComponent::IsAlive() {
	return Health > 0.0f;
}

float UAttributeComponent::GetHealthPercent() {
	return Health / MaxHealth;
}