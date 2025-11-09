// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/WeaponDefinition.h"
#include "GameplayTagContainer.h"
#include "WeaponFragment_GameplayTags.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UWeaponFragment_GameplayTags : public UWeaponFragment
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagContainer InitialGameplayTags;
public:
	virtual void OnInstanceCreated(UWeaponInstance* Instance) const override;
};
