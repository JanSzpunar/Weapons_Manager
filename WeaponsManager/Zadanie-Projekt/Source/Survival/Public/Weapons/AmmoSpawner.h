// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "AmmoSpawner.generated.h"

class UBoxComponent;
UCLASS()
class SURVIVAL_API AAmmoSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoSpawner();
public:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	FGameplayTag AmmoType;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	FGameplayTag ValueType;

private:
	FTimerHandle SpawnerResetTimer;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void EnableCollision();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHideCollider(bool Hidden);

};
