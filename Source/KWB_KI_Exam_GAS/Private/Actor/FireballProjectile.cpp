// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FireballProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"

// Sets default values
AFireballProjectile::AFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->InitSphereRadius(16.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AFireballProjectile::OnOverlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2500.f;
	ProjectileMovement->MaxSpeed = 2500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	SetLifeSpan(5.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Mesh->SetRelativeScale3D(FVector(0.25f));
}

static FGameplayTag TAG_BurnState = FGameplayTag::RequestGameplayTag(TEXT("State.Burn"));

//auto ApplyToTarget = [&](UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> GE)
//	{
//		if (!SourceASC || !TargetASC || !GE) return;
//
//		FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
//		Ctx.AddSourceObject(this);
//		Ctx.AddHitResult(SweepResult); // 히트 위치 전달(큐에서 사용 가능)
//
//		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(GE, 1.f, Ctx);
//		if (Spec.IsValid())
//		{
//			SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
//		}
//	};

void AFireballProjectile::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor || OtherActor == GetOwner() || OtherActor == GetInstigator())
		return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!TargetASC) return;

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
	if (!SourceASC) return;

	auto ApplyToTarget = [&](TSubclassOf<UGameplayEffect> GE)
		{
			if (!GE) return;

			FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
			Ctx.AddSourceObject(this);
			Ctx.AddHitResult(SweepResult);

			FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(GE, 1.f, Ctx);
			if (Spec.IsValid())
			{
				SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
			}
		};

	const bool bAlreadyBurn = TargetASC->HasMatchingGameplayTag(TAG_BurnState);

	ApplyToTarget(bAlreadyBurn ? Damage20 : Damage10);
	ApplyToTarget(BurnGE);

	Destroy();
}
