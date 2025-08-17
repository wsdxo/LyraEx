#pragma once

#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "WeaponManagerComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponManager,Log,All);

UCLASS(ClassGroup="Custom",Blueprintable,BlueprintType,meta=(BlueprintSpawnableComponent))
class UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponManagerComponent();
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FGameplayAbilitySpecHandle>WeaponGiveAbilities;
};
