// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Ability/GA_Fireball.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Actor/FireballProjectile.h"
#include "GameAbilitySystem/AttributeSet/PlayerAttributeSet.h"

UGA_Fireball::UGA_Fireball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Fireball")));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Cooldown.Fireball")));
}

bool UGA_Fireball::ApplyGEToSelf(TSubclassOf<UGameplayEffect> GEClass) const
{
	if (!GEClass) return false;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC) return false;

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GEClass, 1.f, Context);
	if (!Spec.IsValid()) return false;

	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	return true;
}

void UGA_Fireball::SpawnProjectile_Server() const
{
	if (!ProjectileClass) return;

	ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Char) return;

	if (!Char->HasAuthority()) return;

	const FRotator Rot = Char->GetControlRotation();
	const FVector Dir = Rot.Vector();

	const FVector SpawnLoc = Char->GetActorLocation() + Dir * 100.f + FVector(0, 0, 50.f);

	FActorSpawnParameters Params;
	Params.Owner = Char;
	Params.Instigator = Char;

	Char->GetWorld()->SpawnActor<AFireballProjectile>(ProjectileClass, SpawnLoc, Rot, Params);
}

void UGA_Fireball::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Mana 체크
	const float Mana = ASC->GetNumericAttribute(UPlayerAttributeSet::GetManaAttribute());
	if ((int32)Mana < ManaCost)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Cost / Cooldown 적용
	ApplyGEToSelf(CostEffect);
	ApplyGEToSelf(CooldownEffect);

	SpawnProjectile_Server();

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}