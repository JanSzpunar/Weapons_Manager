// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "WeaponsManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponChanged, int32, CurrentIndex);
class UWeaponInstance;
class UWeaponDefinition;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FAppliedWeaponEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FAppliedWeaponEntry()
	{
	}

private:
	friend FWeaponsList;
	friend UWeaponsManager;
	UPROPERTY()
	TObjectPtr<UWeaponInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponsList : public FFastArraySerializer
{
	GENERATED_BODY()

	FWeaponsList(): OwnerComponent(nullptr)
	{
	}

	FWeaponsList(UActorComponent* InOwnerComponent): OwnerComponent(InOwnerComponent)
	{
	}

public:

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FAppliedWeaponEntry, FWeaponsList>(Entries, DeltaParms, *this);
	}

	UWeaponInstance* AddEntry(TSubclassOf<UWeaponDefinition> WeaponDefinition);
	void RemoveEntry(UWeaponInstance* Instance);

private:
	friend UWeaponsManager;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FAppliedWeaponEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};



UCLASS(MinimalAPI, BlueprintType)
class UWeaponsManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponsManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UPROPERTY(BlueprintAssignable, Category = "ClientEvents")
	FOnCurrentWeaponChanged OnCurrentWeaponChanged;
	// Adds weapon instances based on the provided definitions and returns the created instances
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	TArray<UWeaponInstance*> AddWeapons(TArray<TSubclassOf<UWeaponDefinition>> WeaponDefinitions);
	// Removes the specified weapon instance
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveWeapon(UWeaponInstance* WeaponInstance);

	// Equips the weapon at the specified index in the weapons list, returns true if successful
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool EquipWeapon (int32 WeaponIndex);


	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	// Returns equiped weapon, or nullptr if none are found
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UWeaponInstance* GetCurrentlyEquipedWeapon() const;

	// Returns all weapon instances of a given type, or all instances if type not specified or an empty array if none are found
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UWeaponInstance*> GetWeaponInstances(TSubclassOf<UWeaponInstance> OptionalInstanceType) const;

	UFUNCTION(Server, Reliable)
	void StartWeaponUseServer();
	UFUNCTION(Server, Reliable)
	void EndWeaponUseServer();
	UFUNCTION(Server, Reliable)
	void SwapWeaponsServer(bool Top);

private:
	void SwapWeapons(bool Top);
	void StartWeaponUse() const;
	void EndWeaponUse() const;

private:
	UPROPERTY(Replicated)
	FWeaponsList WeaponsList;

	UPROPERTY(Replicated)
	int32 EquippedWeaponIndex = -1;
		
};
