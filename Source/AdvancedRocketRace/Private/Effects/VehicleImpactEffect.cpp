// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "AdvancedRocketRace.h"
#include "VehicleImpactEffect.h"


AVehicleImpactEffect::AVehicleImpactEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bAutoDestroyWhenFinished = true;
}

void AVehicleImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial* HitPhysMat = HitSurface.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	// show particles
	UParticleSystem* ImpactFX = GetImpactFX(HitSurfaceType);
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
	}

	// play sound
	USoundCue* ImpactSound = bWheelLand ? WheelLandingSound : GetImpactSound(HitSurfaceType);
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FMath::Clamp(HitForce.Size() / HitForceSoundDivisor, 0.0f, 1.0f));
	}
}

UParticleSystem* AVehicleImpactEffect::GetImpactFX(TEnumAsByte<EPhysicalSurface> MaterialType)
{
	UParticleSystem* ImpactFX = NULL;

	switch (MaterialType)
	{
	case VEHICLE_SURFACE_Asphalt:	ImpactFX = AsphaltFX; break;
	case VEHICLE_SURFACE_Dirt:		ImpactFX = DirtFX; break;
	case VEHICLE_SURFACE_Water:		ImpactFX = WaterFX; break;
	case VEHICLE_SURFACE_Wood:		ImpactFX = WoodFX; break;
	case VEHICLE_SURFACE_Stone:		ImpactFX = StoneFX; break;
	case VEHICLE_SURFACE_Metal:		ImpactFX = MetalFX; break;
	case VEHICLE_SURFACE_Grass:		ImpactFX = GrassFX; break;
	case VEHICLE_SURFACE_Gravel:	ImpactFX = GravelFX; break;
	default:							ImpactFX = NULL; break;
	}

	return ImpactFX ? ImpactFX : DefaultFX;
}

USoundCue* AVehicleImpactEffect::GetImpactSound(TEnumAsByte<EPhysicalSurface> MaterialType)
{
	USoundCue* ImpactSound = NULL;

	switch (MaterialType)
	{
	case VEHICLE_SURFACE_Asphalt:	ImpactSound = AsphaltSound; break;
	case VEHICLE_SURFACE_Dirt:		ImpactSound = DirtSound; break;
	case VEHICLE_SURFACE_Water:		ImpactSound = WaterSound; break;
	case VEHICLE_SURFACE_Wood:		ImpactSound = WoodSound; break;
	case VEHICLE_SURFACE_Stone:		ImpactSound = StoneSound; break;
	case VEHICLE_SURFACE_Metal:		ImpactSound = MetalSound; break;
	case VEHICLE_SURFACE_Grass:		ImpactSound = GrassSound; break;
	case VEHICLE_SURFACE_Gravel:	ImpactSound = GravelSound; break;
	default:							ImpactSound = NULL; break;
	}
	
	return ImpactSound ? ImpactSound : DefaultSound;
}