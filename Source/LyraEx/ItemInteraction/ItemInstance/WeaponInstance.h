#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WeaponInstance.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class EWeaponSlot:uint8
{
	//手枪
	WS_Pistol=0,
	//步枪
	WS_Rifle=1,
	//冲锋枪
	WS_SMG=2,
	//狙击枪
	WS_Sniper=3,
	
	//无效
	WS_InValid=255,
};

USTRUCT(BlueprintType)
struct FWeaponBindAbility
{
	GENERATED_BODY()

	//武器赋予的技能
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon Ability")
	TSubclassOf<UGameplayAbility> WeaponAbility;

	//技能绑定的输入Tag
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon Ability")
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FWeaponEquippedMontage
{
	GENERATED_BODY()

	//蒙太奇资源
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon Equipped Montage")
	TObjectPtr<UAnimMontage> AnimMontage;

	//播放片段
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon Equipped Montage")
	FName TransactName;

	//播放速率
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon Equipped Montage")
	float PlayRate;

	//是否需要停止其他正在播放的蒙太奇
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon Equipped Montage")
	bool bStopAllMontage;
};
UCLASS(BlueprintType,Blueprintable)
class UWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	UWeaponInstance();
public:
	virtual bool IsSupportedForNetworking() const override{return true;}
	virtual UWorld* GetWorld() const override final;

	void SetInstigator(UObject* InInstigator){Instigator=InInstigator;}
	void InitInstance(){CurrentCartridge=CartridgeCountsOnSpawn;}

	void CartridgeCost(int32 CostCounts);
	void ReloadCartridge(int32 CartridgeCounts);

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	UObject* GetInstigator()const{return Instigator;}

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	ACharacter* GetCharacter()const;

	void WeaponTick(float DeltaSeconds);

	void OnEquipped(UObject* InInstigator);
	void OnUnequipped(UObject* InInstigator);

	void AddSpread();

	void UpdateFiringTime();

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	EWeaponSlot GetWeaponSlot() const{return WeaponSlot;}

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	FName GetInventorySlotName() const{return InventorySlotName;}

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	FTransform GetInventoryRelativeTransform() const {return InventoryRelativeTransform;}

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	TArray<FWeaponBindAbility> GetBindAbilities() const{return BindAbilities;}

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	FWeaponEquippedMontage GetEquippedMontage() const {return EquippedMontage;}

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	FWeaponEquippedMontage GetUnequippedMontage() const {return UnequippedMontage;}

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	int32 GetCurrentCartridge() const{return CurrentCartridge;}

	UFUNCTION(BlueprintPure,Category="Weapon Instance")
	int32 GetClipSize() const{return ClipSize;}

	int32 GetBulletsPerCartridge() const{return BulletsPerCartridge;}

	float GetCalculatedSpreadAngle() const{return CurrentSpreadAngle;}
	float GetCalculatedSpreadAngleMultiplier() const{return bHasFirstShotAccuracy?0.f:CurrentSpreadAngleMultiplier;}

	bool GetHasFirstShotAccuracy() const{return bHasFirstShotAccuracy;}
	float GetSpreadExponent() const{return SpreadExponent;}

	float GetMaxDamageDistance() const{return MaxDamageRange;}

	float GetBulletRadius() const{return BulletRadius;}
	bool GetCanDrawDebug() const{return bDrawDebug;}

protected:
	UFUNCTION(BlueprintImplementableEvent,Category="Weapon Animation")
	void SetAnimationParamsOnEquipped(UObject* InInstigator);

	UFUNCTION(BlueprintImplementableEvent,Category="Weapon Animation")
	void SetAnimationParamsOnUnequipped(UObject* InInstigator);

private:
	void ComputeSpreadRange(float& MinSpread,float& MaxSpread);
	void ComputeHeatRange(float& MinHeat,float&MaxHeat);

	inline float ClampHeat(float NewHeat)
	{
		float MinHeat;
		float MaxHeat;
		ComputeHeatRange(MinHeat,MaxHeat);

		return FMath::Clamp(NewHeat,MinHeat,MaxHeat);
	}

	bool UpdateSpread(float DeltaSeconds);
	bool UpdateMultipliers(float DeltaSeconds);
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	EWeaponSlot WeaponSlot=EWeaponSlot::WS_InValid;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	FName InventorySlotName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	FTransform InventoryRelativeTransform;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	TArray<FWeaponBindAbility> BindAbilities;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	FWeaponEquippedMontage EquippedMontage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	FWeaponEquippedMontage UnequippedMontage;

	//初始弹药数量
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	int32 CartridgeCountsOnSpawn=0;

	//弹夹最大容量
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	int32 ClipSize=10;

	//每次射击射出的弹丸数量，一般都是1，除了喷子
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	int32 BulletsPerCartridge=1;

	//最大伤害距离
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic",meta=(ForceUnits="cm"))
	float MaxDamageRange=25000.f;

	//弹丸半径
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic",meta=(ForceUnits="cm"))
	float BulletRadius=4.f;

	//伤害衰减曲线
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	FRuntimeFloatCurve DistanceDamageFalloffCurve;

	//不同部位伤害倍率
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Basic")
	TMap<FName,float> MaterialDamageMultiplier;

	
	//扩散参数（该参数越大腰射精度越差）
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Spread|Fire Params")
	float SpreadExponent=1.f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToHeatPerShotCurve;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToSpreadAngleCurve;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Spread|Fire Params",meta=(ForceUnits="s"))
	float SpreadRecoveryCooldownDelay=0.25f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Spread|Fire Params")
	bool bAllowFirstShotAccuracy=true;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon Instance Defaults|Spread|Fire Params",meta=(ForceUnits=x))
	float SpreadAngleMultiplier_Aiming=1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_StandingStill = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	float TransitionRate_StandingStill = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits="cm/s"))
	float StandingStillSpeedThreshold = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits="cm/s"))
	float StandingStillToMovingSpeedRange = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_Crouching = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	float TransitionRate_Crouching = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	bool bDrawDebug=false;

private:
	double LastFireTime=0.0;

	float CurrentHeat=0.f;

	float CurrentSpreadAngle=0.f;

	bool bHasFirstShotAccuracy=false;

	float CurrentSpreadAngleMultiplier=1.f;

	float StandingStillMultiplier=1.f;

	float CrouchingMultiplier=1.f;

private:
	UPROPERTY(Replicated)
	UObject* Instigator;

	UPROPERTY(Replicated)
	int32 CurrentCartridge=0;
};
