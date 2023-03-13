// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MyCharacterMovementComponent.h"
#include "Player/CT_BaseCharacter.h"

float UMyCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ACT_BaseCharacter* Player = Cast<ACT_BaseCharacter>(GetPawnOwner());

	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
