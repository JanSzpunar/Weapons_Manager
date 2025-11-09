// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponsManager.h"
#include "Weapons/WeaponInstance.h"
#include "Weapons/WeaponDefinition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

//Weapons list
UWeaponInstance* FWeaponsList::AddEntry(TSubclassOf<UWeaponDefinition> WeaponDefinition)
{
	UWeaponInstance* Result = nullptr;

	check(WeaponDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UWeaponDefinition* WeaponDefCDO = GetDefault<UWeaponDefinition>(WeaponDefinition);

	TSubclassOf<UWeaponInstance> InstanceType = WeaponDefCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UWeaponInstance::StaticClass();
		if (GEngine)
		{
			FString Message = FString::Printf(TEXT("Add weapon instance in: %s instance type."), *WeaponDefCDO->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
		}
	}

	FAppliedWeaponEntry& NewEntry = Entries.AddDefaulted_GetRef();

	NewEntry.Instance = NewObject<UWeaponInstance>(OwnerComponent->GetOwner(), InstanceType);

	for (UWeaponFragment* Fragment : GetDefault<UWeaponDefinition>(WeaponDefinition)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	Result = NewEntry.Instance;

	Result->SpawnWeaponActors(WeaponDefCDO->ActorsToSpawn);
	Result->SetSpawnedActorsVisibility(false);
	MarkItemDirty(NewEntry);

	return Result;
}

void FWeaponsList::RemoveEntry(UWeaponInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FAppliedWeaponEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			Instance->DestroyWeaponActors();


			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//Weapons manager
UWeaponsManager::UWeaponsManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), WeaponsList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UWeaponsManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, WeaponsList);
}

TArray<UWeaponInstance*> UWeaponsManager::AddWeapons(TArray<TSubclassOf<UWeaponDefinition>> WeaponDefinitions)
{
	TArray<UWeaponInstance*> Results;
	for (const TSubclassOf<UWeaponDefinition>& WeaponDefinition : WeaponDefinitions)
	{
		UWeaponInstance* Result = nullptr;
		if (WeaponDefinition != nullptr)
		{
			Result = WeaponsList.AddEntry(WeaponDefinition);

			if (Result != nullptr)
			{
				Results.Add(Result);

				if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
				{
					AddReplicatedSubObject(Result);
				}
			}
		}
	}
	return Results;
}

void UWeaponsManager::RemoveWeapon(UWeaponInstance* WeaponInstance)
{
	if (WeaponInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(WeaponInstance);
		}

		WeaponInstance->OnUnequipped();
		WeaponsList.RemoveEntry(WeaponInstance);
	}
}

bool UWeaponsManager::EquipWeapon(int32 WeaponIndex)
{
	if(!WeaponsList.Entries.IsValidIndex(WeaponIndex))
	{
		return false;
	}
	if (!WeaponsList.Entries[WeaponIndex].Instance)
	{
		return false;
	}
	if (WeaponsList.Entries.IsValidIndex(EquippedWeaponIndex))
	{
		if (WeaponsList.Entries[EquippedWeaponIndex].Instance)
		{
			WeaponsList.Entries[EquippedWeaponIndex].Instance->EndWeaponUse();
			WeaponsList.Entries[EquippedWeaponIndex].Instance->SetSpawnedActorsVisibility(false);
			WeaponsList.Entries[EquippedWeaponIndex].Instance->OnUnequipped();
		}
	}
	EquippedWeaponIndex = WeaponIndex;
	UWeaponInstance* CurrentWeapon = GetCurrentlyEquipedWeapon();
	CurrentWeapon->SetSpawnedActorsVisibility(true);
	CurrentWeapon->OnEquipped();
	return true;
}

bool UWeaponsManager::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FAppliedWeaponEntry& Entry : WeaponsList.Entries)
	{
		UWeaponInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UWeaponsManager::InitializeComponent()
{
	Super::InitializeComponent();
}

void UWeaponsManager::UninitializeComponent()
{
	TArray<UWeaponInstance*> AllWeaponsInstances;	
	for (const FAppliedWeaponEntry& Entry : WeaponsList.Entries)
	{
		AllWeaponsInstances.Add(Entry.Instance);
	}

	for (UWeaponInstance* WeaponInstance : AllWeaponsInstances)
	{
		RemoveWeapon(WeaponInstance);
	}

	Super::UninitializeComponent();
}

void UWeaponsManager::ReadyForReplication()
{
	Super::ReadyForReplication();
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FAppliedWeaponEntry& Entry : WeaponsList.Entries)
		{
			UWeaponInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UWeaponInstance* UWeaponsManager::GetCurrentlyEquipedWeapon() const
{
	if (!WeaponsList.Entries.IsValidIndex(EquippedWeaponIndex))
	{
		return nullptr;
	}
	if (!WeaponsList.Entries[EquippedWeaponIndex].Instance)
	{
		return nullptr;
	}

	return WeaponsList.Entries[EquippedWeaponIndex].Instance;
}

TArray<UWeaponInstance*> UWeaponsManager::GetWeaponInstances(TSubclassOf<UWeaponInstance> OptionalInstanceType) const
{
	TArray<UWeaponInstance*> Results;
	for (const FAppliedWeaponEntry& Entry : WeaponsList.Entries)
	{
		if (UWeaponInstance* Instance = Entry.Instance)
		{
			if(OptionalInstanceType == nullptr)
			{
				Results.Add(Instance);
			}
			else
			{
				if (Instance->IsA(OptionalInstanceType))
				{
					Results.Add(Instance);
				}
			}

		}
	}
	return Results;
}

void UWeaponsManager::StartWeaponUseServer_Implementation()
{
	StartWeaponUse();
}

void UWeaponsManager::StartWeaponUse() const
{
	if (UWeaponInstance* CurrentWeapon = GetCurrentlyEquipedWeapon())
	{
		CurrentWeapon->StartWeaponUse();
	}

}

void UWeaponsManager::EndWeaponUseServer_Implementation()
{
	EndWeaponUse();
}

void UWeaponsManager::EndWeaponUse() const
{
	if (UWeaponInstance* CurrentWeapon = GetCurrentlyEquipedWeapon())
	{
		CurrentWeapon->EndWeaponUse();
	}
}

void UWeaponsManager::SwapWeaponsServer_Implementation(bool Top)
{
	SwapWeapons(Top);
}

void UWeaponsManager::SwapWeapons(bool Top)
{
	if (WeaponsList.Entries.Num() < 2) return;

	int32 NewIndex = 0;
	if (Top)
	{
		NewIndex = (EquippedWeaponIndex + 1 < WeaponsList.Entries.Num()) ? EquippedWeaponIndex + 1 : 0;
	}
	else
	{
		NewIndex = (EquippedWeaponIndex - 1 >= 0) ? EquippedWeaponIndex - 1 : WeaponsList.Entries.Num() - 1;
	}
	if (EquipWeapon(NewIndex))
	{
		UWeaponInstance* CurrentWeapon = GetCurrentlyEquipedWeapon();
		UE_LOG(LogTemp, Warning, TEXT("Swapped to weapon: %s"), *CurrentWeapon->GetName());
	}
}




