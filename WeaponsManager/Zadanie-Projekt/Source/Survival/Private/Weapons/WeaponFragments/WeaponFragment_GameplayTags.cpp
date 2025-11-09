// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponFragments/WeaponFragment_GameplayTags.h"
#include "Weapons/WeaponInstance.h"

void UWeaponFragment_GameplayTags::OnInstanceCreated(UWeaponInstance* Instance) const
{
	for (const FGameplayTag& Tag : InitialGameplayTags)
	{
		Instance->AddGameplayTag(Tag);
	}

}
