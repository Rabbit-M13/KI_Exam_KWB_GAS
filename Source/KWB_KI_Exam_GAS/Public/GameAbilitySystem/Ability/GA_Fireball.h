#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Fireball.generated.h"

class UGameplayEffect;
class AFireballProjectile;

UCLASS()
class KWB_KI_EXAM_GAS_API UGA_Fireball : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Fireball();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	TSubclassOf<AFireballProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball|Cost")
	TSubclassOf<UGameplayEffect> CostEffect;        // GE_Fireball_Cost

	UPROPERTY(EditDefaultsOnly, Category = "Fireball|Cooldown")
	TSubclassOf<UGameplayEffect> CooldownEffect;    // GE_Fireball_Cooldown

	UPROPERTY(EditDefaultsOnly, Category = "Fireball|Cost")
	int32 ManaCost = 10;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	bool ApplyGEToSelf(TSubclassOf<UGameplayEffect> GEClass) const;
	void SpawnProjectile_Server() const;
};
