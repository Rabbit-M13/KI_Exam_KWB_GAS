// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Cue/GCN_Burn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameplayEffectTypes.h"

static FVector GetHitLocation(const FGameplayCueParameters& Params, const AActor* FallbackTarget)
{
	if (Params.EffectContext.IsValid())
	{
		const FHitResult* HR = Params.EffectContext.GetHitResult();
		if (HR)
		{
			return HR->ImpactPoint;
		}
	}

	if (!Params.Location.IsNearlyZero())
	{
		return Params.Location;
	}

	return FallbackTarget ? FallbackTarget->GetActorLocation() : FVector::ZeroVector;
}

bool AGCN_Burn::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (!BurnNiagara || !MyTarget) return false;

	const FVector SpawnLoc = GetHitLocation(Parameters, MyTarget);

	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), BurnNiagara, SpawnLoc, FRotator::ZeroRotator, FVector(1.f), true, true);

	return NiagaraComp != nullptr;
}

bool AGCN_Burn::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (NiagaraComp)
	{
		NiagaraComp->Deactivate();
		NiagaraComp = nullptr;
	}
	return true;
}