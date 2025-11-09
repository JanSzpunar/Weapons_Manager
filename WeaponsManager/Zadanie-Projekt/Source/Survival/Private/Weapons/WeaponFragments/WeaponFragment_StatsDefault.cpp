// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponFragments/WeaponFragment_StatsDefault.h"
#include "Weapons/WeaponInstance.h"

void UWeaponFragment_StatsDefault::OnInstanceCreated(UWeaponInstance* Instance) const
{
	for (const auto& KVP : InitialItemStats)
	{
		Instance->AddStatTagStack(KVP.Key, KVP.Value);
	}
}
