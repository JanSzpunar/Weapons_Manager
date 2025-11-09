// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "Core/GameplayTagStack.h"
#include "WeaponInstance.generated.h"

struct FWeaponActorToSpawn;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class SURVIVAL_API UWeaponInstance : public UObject
{
	GENERATED_BODY()
public:
	UWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;

public:
	UFUNCTION(BlueprintPure, Category = "WeaponInstance")
	UObject* GetInstigator() const { return Instigator; }

	UFUNCTION(BlueprintPure, Category = "WeaponInstance")
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category = "WeaponInstance", meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "WeaponInstance")
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "WeaponInstance")
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	bool HasStatTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "WeaponInstance")
	void AddGameplayTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "WeaponInstance")
	bool RemoveGameplayTag(FGameplayTag Tag);

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category = "WeaponInstance")
	bool HasGameplayTag(FGameplayTag Tag, bool Exact = true) const;



public:
	void SpawnWeaponActors(const TArray<FWeaponActorToSpawn>& ActorsToSpawn);
	void DestroyWeaponActors();
	void SetSpawnedActorsVisibility(bool bNewVisibility);

	virtual void OnEquipped();
	virtual void OnUnequipped();
	void SetInstigator(UObject* NewInstigator) { Instigator = NewInstigator; }

	virtual bool StartWeaponUse();
	virtual void EndWeaponUse();

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagStack ResourcesOnUse;
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "WeaponInstance", meta = (DisplayName = "OnEquipped"))
	void Event_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "WeaponInstance", meta = (DisplayName = "OnUnequipped"))
	void Event_OnUnequipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "WeaponInstance", meta = (DisplayName = "OnNoResources"))
	void Event_OnNoResources();
protected:
	int32 GetBaseDamage() const { return GetStatTagStackCount(FGameplayTag::RequestGameplayTag(FName("Weapons.Damage")));}
	bool CheckResourcesAndNotify();
	void SpendResources();
private:
	UFUNCTION()
	void OnRep_Instigator();

private:
	UPROPERTY(ReplicatedUsing = OnRep_Instigator)
	TObjectPtr<UObject> Instigator;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY(Replicated)
	FGameplayTagContainer GameplayTags;

};
