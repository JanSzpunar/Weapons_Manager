// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AmmoSpawner.h"
#include "Weapons/WeaponManager.h"
#include "Weapons/WeaponInstance.h"
#include "Components/BoxComponent.h"
// Sets default values
AAmmoSpawner::AAmmoSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->SetHiddenInGame(false);
	BoxCollider->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAmmoSpawner::OnBoxOverlap);
	RootComponent = BoxCollider;
}

void AAmmoSpawner::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UWeaponManager::StaticClass()))
		{
			IWeaponManager* Interface = Cast<IWeaponManager>(OtherActor);
			if (!Interface)
			{
				return;
			}
			UWeaponsManager* Manager = Interface->GetWeaponManager();
			if (!Manager)
			{
				return;
			}
			UWeaponInstance* CurrentWeapon = Manager->GetCurrentlyEquipedWeapon();
			if (CurrentWeapon)
			{
				if (!CurrentWeapon->HasGameplayTag(AmmoType, true))
				{
					FString Message = FString::Printf(TEXT("Weapon with ammo type: %s not equiped."), *AmmoType.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);

					return;
				}
				CurrentWeapon->AddStatTagStack(ValueType, 10);

				BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				MulticastHideCollider(true);

				GetWorld()->GetTimerManager().SetTimer(SpawnerResetTimer, this, &AAmmoSpawner::EnableCollision, 5.f, false);

			}
		}
	}
}

void AAmmoSpawner::EnableCollision()
{
	if (BoxCollider)
	{
		BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MulticastHideCollider(false);
	}
}

void AAmmoSpawner::MulticastHideCollider_Implementation(bool Hidden)
{
	BoxCollider->SetHiddenInGame(Hidden);
}





