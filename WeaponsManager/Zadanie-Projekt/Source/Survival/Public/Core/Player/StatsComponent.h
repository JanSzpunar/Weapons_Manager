// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/GameplayTagStack.h"
#include "StatsComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClientDamageReceived, int32, CurrentHealth, int32, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawnRequested);
// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UStatsComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class SURVIVAL_API IStatsComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category = "StatsComponent")
	virtual UStatsComponent* GetStatsComponent() const = 0;
};

UCLASS(MinimalAPI, BlueprintType)
class UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "StatsComponent")
	void ReceiveDamage(float DamageAmount);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatsComponent")
	int32 GetStatTagStackCount(FGameplayTag Tag) const;
	UPROPERTY(BlueprintAssignable)
	FOnClientDamageReceived OnClientDamageReceived;
	UPROPERTY(BlueprintAssignable)
	FOnRespawnRequested OnRespawnRequested;
public:
	UPROPERTY(EditDefaultsOnly, Category = "StatsComponent")
	FGameplayTag HealthTag;
	UPROPERTY(EditDefaultsOnly, Category = "StatsComponent")
	FGameplayTag MaxHealthTag;
	UPROPERTY(EditDefaultsOnly, Category = "StatsComponent")
	TMap<FGameplayTag,int32> DefaultStats;
private:
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	bool HasStatTag(FGameplayTag Tag) const;
private:
	UFUNCTION(Client, Reliable)
	void Client_ReceiveDamage();
private:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
