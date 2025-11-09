// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponDefinition.h"
#include "Weapons/WeaponInstance.h"

UWeaponDefinition::UWeaponDefinition(const FObjectInitializer& ObjectInitializer)
{
	DisplayName = FText::FromString(TEXT("Default Weapon"));
	InstanceType = UWeaponInstance::StaticClass();
}
