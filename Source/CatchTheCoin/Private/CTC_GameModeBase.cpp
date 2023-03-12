// Fill out your copyright notice in the Description page of Project Settings.

#include "CTC_GameModeBase.h"
#include "Player/CT_BaseCharacter.h"
#include "Player/CTC_PlayerController.h"

ACTC_GameModeBase::ACTC_GameModeBase()
{
	DefaultPawnClass = ACT_BaseCharacter::StaticClass();
	PlayerControllerClass = ACTC_PlayerController::StaticClass();
}