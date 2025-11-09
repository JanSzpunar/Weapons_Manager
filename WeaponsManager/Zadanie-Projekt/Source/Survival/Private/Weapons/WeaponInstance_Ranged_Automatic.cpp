// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponInstance_Ranged_Automatic.h"

bool UWeaponInstance_Ranged_Automatic::StartWeaponUse()
{
    if (!Super::StartWeaponUse()) { return false; }
    if (ShotsPerSecond <= 0) {return false;}

    // Calculate fire interval
    const float FireInterval = 1.f / static_cast<float>(ShotsPerSecond);

    // Start looping fire
    GetWorld()->GetTimerManager().SetTimer(
        FireLoopHandle,
        this,
        &UWeaponInstance_Ranged_Automatic::FireProjectileAutomatic,
        FireInterval,
        true
    );

    // Fire immediately on press
    FireProjectile();
	return true;

}


void UWeaponInstance_Ranged_Automatic::EndWeaponUse()
{
	Super::EndWeaponUse();
    GetWorld()->GetTimerManager().ClearTimer(FireLoopHandle);

}

void UWeaponInstance_Ranged_Automatic::FireProjectileAutomatic()
{
    if(!CheckResourcesAndNotify())
    {
        EndWeaponUse();
        return;
	}
    FireProjectile();
    SpendResources();
}
