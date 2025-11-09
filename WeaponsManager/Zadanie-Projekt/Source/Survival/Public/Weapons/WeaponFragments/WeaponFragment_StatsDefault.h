// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/WeaponDefinition.h"
#include "GameplayTagContainer.h"
#include "WeaponFragment_StatsDefault.generated.h"

class UWeaponInstance;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UWeaponFragment_StatsDefault : public UWeaponFragment
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "InitialItemStats")
	TMap<FGameplayTag, int32> InitialItemStats;
public:
	virtual void OnInstanceCreated(UWeaponInstance* Instance) const override;
};
