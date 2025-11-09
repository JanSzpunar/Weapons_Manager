// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponInstance_Ranged.h"
#include "Weapons/Projectile.h"

void UWeaponInstance_Ranged::FireProjectile()
{
    APlayerController* PC = Cast<APlayerController>(GetPawn()->GetController());
    if (!PC) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    // Muzzle offset from camera
    FVector MuzzleLocation = CameraLocation + CameraRotation.Vector() * 100.f;
    FRotator MuzzleRotation = CameraRotation;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetPawn();
    SpawnParams.Instigator = GetPawn();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (ProjectileType)
    {
        float Damage = static_cast<float>(GetBaseDamage());
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileType, MuzzleLocation, MuzzleRotation, SpawnParams);
        Projectile->SetDamage(Damage);
        
    }
}
