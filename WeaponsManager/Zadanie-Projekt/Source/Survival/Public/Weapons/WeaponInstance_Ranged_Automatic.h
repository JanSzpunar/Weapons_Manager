// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponInstance_Ranged.h"
#include "WeaponInstance_Ranged_Automatic.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class SURVIVAL_API UWeaponInstance_Ranged_Automatic : public UWeaponInstance_Ranged
{
	GENERATED_BODY()
public:
	virtual bool StartWeaponUse() override;
	virtual void EndWeaponUse() override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", UIMin = "1"))
	int32 ShotsPerSecond = 5;

private:
	FTimerHandle FireLoopHandle;
	void FireProjectileAutomatic();
};
