// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponInstance_Ranged_Single.h"

bool UWeaponInstance_Ranged_Single::StartWeaponUse()
{
	if (!Super::StartWeaponUse()) { return false; }
	FireProjectile();
	return true;
}

void UWeaponInstance_Ranged_Single::EndWeaponUse()
{
	Super::EndWeaponUse();

}
