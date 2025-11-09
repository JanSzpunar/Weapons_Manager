// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponInstance.h"
#include "WeaponInstance_Ranged.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class SURVIVAL_API UWeaponInstance_Ranged : public UWeaponInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileType;
protected:
	void FireProjectile();
};
