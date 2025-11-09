// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponInstance.h"
#include "Weapons/WeaponDefinition.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UWeaponInstance::UWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWeaponInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, GameplayTags);
}

UWorld* UWeaponInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

APawn* UWeaponInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UWeaponInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

void UWeaponInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);

}

void UWeaponInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UWeaponInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UWeaponInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void UWeaponInstance::AddGameplayTag(FGameplayTag Tag)
{
	GameplayTags.AddTag(Tag);
}

bool UWeaponInstance::RemoveGameplayTag(FGameplayTag Tag)
{
	return GameplayTags.RemoveTag(Tag);
}

bool UWeaponInstance::HasGameplayTag(FGameplayTag Tag, bool Exact) const
{
	if (Exact)
	{
		return GameplayTags.HasTagExact(Tag);
	}
	return GameplayTags.HasTag(Tag);
}



void UWeaponInstance::SpawnWeaponActors(const TArray<FWeaponActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}
		for (const FWeaponActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
			SpawnedActors.Add(NewActor);
		}

	}
}

void UWeaponInstance::DestroyWeaponActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UWeaponInstance::SetSpawnedActorsVisibility(bool bNewVisibility)
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(!bNewVisibility);
		}
	}
}

void UWeaponInstance::OnEquipped()
{
	Event_OnEquipped();
}

void UWeaponInstance::OnUnequipped()
{
	Event_OnUnequipped();
}

bool UWeaponInstance::StartWeaponUse()
{
	if (!GetPawn() || !GetWorld() || !CheckResourcesAndNotify()) return false;

	SpendResources();
	return true;
}

void UWeaponInstance::EndWeaponUse()
{
	if (!GetPawn() || !GetWorld()) return;

}

bool UWeaponInstance::CheckResourcesAndNotify()
{
	if (ResourcesOnUse.Tag.IsValid())
	{
		if (GetStatTagStackCount(ResourcesOnUse.Tag) >= ResourcesOnUse.StackCount)
		{
			return true;
		}
		Event_OnNoResources();
		return false;
	}
	return true;
}

void UWeaponInstance::SpendResources()
{
	RemoveStatTagStack(ResourcesOnUse.Tag, ResourcesOnUse.StackCount); //Remove resources e.g. ammo
}

void UWeaponInstance::OnRep_Instigator()
{

}






