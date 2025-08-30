#pragma once

#include "LyraEx/ItemInteraction/Items/Pickable/TPItemActor_Pickable.h"
#include  "TPItemActor_Weapon.generated.h"

class UWeaponManagerComponent;
class UWeaponInstance;

UCLASS()
class ATPItemActor_Weapon : public ATPItemActor_Pickable
{
	GENERATED_BODY()

public:
	ATPItemActor_Weapon();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="TPItemActor_Normal")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon Defaults")
	TSubclassOf<UWeaponInstance> WeaponInstanceClass;

public:
	UFUNCTION(BlueprintPure,Category="TPItemActor_Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}

	UFUNCTION(BlueprintPure,Category="TPItemActor_Weapon")
	UWeaponInstance* GetWeaponInstance() const {return WeaponInstance;}

	UFUNCTION(BlueprintCallable,Category="TPItemActor_Weapon")
	void EquipToSlot();

	UFUNCTION(BlueprintCallable,Category="TPItemActor_Weapon")
	void EquipToHand();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnStartTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent, bool bForceInHand) override;
	virtual void OnEndTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent, bool bIsPutPack) override;

protected:
	UWeaponManagerComponent* GetWeaponManager(UTPItemInteractionComponent* InInteractionComponent);

private:
	UPROPERTY(Replicated)
	TObjectPtr<UWeaponInstance> WeaponInstance;
};
