// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/StatsComponent.h"
#include "Net/UnrealNetwork.h"


UStatsComponent::UStatsComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}
void UStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
}

void UStatsComponent::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);

}

void UStatsComponent::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UStatsComponent::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UStatsComponent::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void UStatsComponent::Client_ReceiveDamage_Implementation()
{
	OnClientDamageReceived.Broadcast(GetStatTagStackCount(HealthTag), GetStatTagStackCount(MaxHealthTag));
}

void UStatsComponent::ReceiveDamage(float DamageAmount)
{
	if (HealthTag.IsValid() && DamageAmount > 0)
	{
		RemoveStatTagStack(HealthTag, FMath::RoundToInt(DamageAmount));
	}
	Client_ReceiveDamage();
	if(GetStatTagStackCount(HealthTag) <= 0)
	{
		if (AActor* Owner = GetOwner())
		{
			OnRespawnRequested.Broadcast();
			Owner->Destroy();
		}
	}
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		for (const TPair<FGameplayTag, int32>& Pair : DefaultStats)
		{
			AddStatTagStack(Pair.Key, Pair.Value);
		}
	}
}


