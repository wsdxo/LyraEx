#include "WeaponInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UE_DISABLE_OPTIMIZATION
UWeaponInstance::UWeaponInstance()
{
}

UWorld* UWeaponInstance::GetWorld() const
{
	if(Instigator)
	{
		return Instigator->GetWorld();
	}
	return nullptr;
}

void UWeaponInstance::CartridgeCost(int32 CostCounts)
{
	check(CostCounts<=CurrentCartridge)

	CurrentCartridge-=CostCounts;
}

void UWeaponInstance::ReloadCartridge(int32 CartridgeCounts)
{
	CurrentCartridge+=CartridgeCounts;

	check(CurrentCartridge<=ClipSize)
}

ACharacter* UWeaponInstance::GetCharacter() const
{
	return Cast<ACharacter>(Instigator);
}

void UWeaponInstance::WeaponTick(float DeltaSeconds)
{
	const bool bMinSpread=UpdateSpread(DeltaSeconds);
	const bool bMinMultiploers=UpdateMultipliers(DeltaSeconds);

	bHasFirstShotAccuracy=bAllowFirstShotAccuracy&&bMinSpread&&bMinMultiploers;
}

void UWeaponInstance::OnEquipped(UObject* InInstigator)
{
	float MinHeatRange,MaxHeatRange;
	ComputeHeatRange(MinHeatRange,MaxHeatRange);
	CurrentHeat=(MinHeatRange+MaxHeatRange)*0.5f;

	CurrentSpreadAngle=HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);

	CurrentSpreadAngleMultiplier=1.f;
	StandingStillMultiplier=1.f;
	CrouchingMultiplier=1.f;

	SetAnimationParamsOnEquipped(InInstigator);
}

void UWeaponInstance::OnUnequipped(UObject* InInstigator)
{
	SetAnimationParamsOnUnequipped(InInstigator);
}

void UWeaponInstance::AddSpread()
{
	const float HeatPerShot=HeatToHeatPerShotCurve.GetRichCurveConst()->Eval(CurrentHeat);

	CurrentHeat=ClampHeat(CurrentHeat+HeatPerShot);
	CurrentSpreadAngle=HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);
}

void UWeaponInstance::UpdateFiringTime()
{
	UWorld* World=GetWorld();
	check(World);
	LastFireTime=World->GetTimeSeconds();
}

void UWeaponInstance::ComputeSpreadRange(float& MinSpread, float& MaxSpread)
{
	HeatToSpreadAngleCurve.GetRichCurveConst()->GetValueRange(MinSpread,MaxSpread);
}

void UWeaponInstance::ComputeHeatRange(float& MinHeat, float& MaxHeat)
{
	float Min1,Max1;
	HeatToHeatPerShotCurve.GetRichCurveConst()->GetTimeRange(Min1,Max1);

	float Min2,Max2;
	HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetTimeRange(Min2,Max2);

	float Min3,Max3;
	HeatToSpreadAngleCurve.GetRichCurveConst()->GetTimeRange(Min3,Max3);

	MinHeat=FMath::Min(Min1,FMath::Min(Min2,Min3));
	MaxHeat=FMath::Max(Max1,FMath::Max(Max2,Max3));
}

bool UWeaponInstance::UpdateSpread(float DeltaSeconds)
{
	const float TimeSinceFired=GetWorld()?GetWorld()->TimeSince(LastFireTime):0.f;
	if(TimeSinceFired>SpreadRecoveryCooldownDelay)
	{
		const float CooldownRate=HeatToCoolDownPerSecondCurve.GetRichCurveConst()->Eval(CurrentHeat);
		CurrentHeat=ClampHeat(CurrentHeat-(CooldownRate*DeltaSeconds));
		CurrentSpreadAngle=HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);
	}

	float MinSpread,MaxSpread;
	ComputeSpreadRange(MinSpread,MaxSpread);

	return FMath::IsNearlyEqual(CurrentSpreadAngle,MinSpread,KINDA_SMALL_NUMBER);
}

bool UWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
	const float MultiplierNearlyEqualThreshold=0.05f;

	if(ACharacter* PlayerCharacter=GetCharacter())
	{
		if(UCharacterMovementComponent* CharMoveCmop=PlayerCharacter->GetCharacterMovement())
		{
			const float CharacterSpeed=PlayerCharacter->GetVelocity().Size();

			const float MovementTargetValue=FMath::GetMappedRangeValueClamped(
				FVector2D(StandingStillSpeedThreshold,StandingStillSpeedThreshold+StandingStillToMovingSpeedRange),
				FVector2D(SpreadAngleMultiplier_StandingStill,1.f),
				CharacterSpeed);

			StandingStillMultiplier=FMath::FInterpTo(StandingStillMultiplier,MovementTargetValue,DeltaSeconds,TransitionRate_StandingStill);
			const bool bStandingStillMultiplierAtMin=FMath::IsNearlyEqual(StandingStillMultiplier,SpreadAngleMultiplier_StandingStill,MultiplierNearlyEqualThreshold);

			const bool bIsCrouching=(CharMoveCmop!=nullptr)&&CharMoveCmop->IsCrouching();

			const float CrouchingTargetValue=bIsCrouching?SpreadAngleMultiplier_Crouching:1.f;

			CrouchingMultiplier=FMath::FInterpTo(CrouchingMultiplier,CrouchingTargetValue,DeltaSeconds,TransitionRate_Crouching);

			const bool bCrouchingMultiplierAtMin=FMath::IsNearlyEqual(CrouchingMultiplier,SpreadAngleMultiplier_Crouching,MultiplierNearlyEqualThreshold);

			float AimingAlpha=0.f;
			const float AimingMultiplier=FMath::GetMappedRangeValueClamped(
				FVector2D(0.f,1.f),
				FVector2D(1.f,SpreadAngleMultiplier_Aiming),
				AimingAlpha);

			const bool bAimingMultiplierAtMin=FMath::IsNearlyEqual(AimingMultiplier,SpreadAngleMultiplier_Aiming,MultiplierNearlyEqualThreshold);

			CurrentSpreadAngleMultiplier=StandingStillMultiplier*CrouchingMultiplier*AimingMultiplier;

			return bStandingStillMultiplierAtMin&&bCrouchingMultiplierAtMin&&bAimingMultiplierAtMin;
		}
	}
	return false;
}

void UWeaponInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponInstance,Instigator);
	DOREPLIFETIME(UWeaponInstance,CurrentCartridge);
}
UE_ENABLE_OPTIMIZATION