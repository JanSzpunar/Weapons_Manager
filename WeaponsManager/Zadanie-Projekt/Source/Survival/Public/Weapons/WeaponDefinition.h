// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Templates/SubclassOf.h"

#include "WeaponDefinition.generated.h"

class UWeaponInstance;
/**
 * 
 */
USTRUCT()
struct FWeaponActorToSpawn
{
	GENERATED_BODY()

	FWeaponActorToSpawn()
	{
	}

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FTransform AttachTransform;
};

// Represents a fragment of an item definition
UCLASS(MinimalAPI, DefaultToInstanced, EditInlineNew, Abstract)
class UWeaponFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UWeaponInstance* Instance) const {}
};


UCLASS(Blueprintable, Const, Abstract)
class SURVIVAL_API UWeaponDefinition : public UObject
{
	GENERATED_BODY()
public:
	UWeaponDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Display name for this weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponDefinition")
	FText DisplayName;

	// The type of weapon instance to create for this definition
	UPROPERTY(EditDefaultsOnly, Category = "WeaponDefinition")
	TSubclassOf<UWeaponInstance> InstanceType;

	// Fragments that define the behavior of this weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponDefinition", Instanced)
	TArray<TObjectPtr<UWeaponFragment>> Fragments;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = "WeaponDefinition")
	TArray<FWeaponActorToSpawn> ActorsToSpawn;
};
