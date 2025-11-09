// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponInstance_Ranged.h"
#include "WeaponInstance_Ranged_Single.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class SURVIVAL_API UWeaponInstance_Ranged_Single : public UWeaponInstance_Ranged
{
	GENERATED_BODY()
public:
	virtual bool StartWeaponUse() override;
	virtual void EndWeaponUse() override;
};
