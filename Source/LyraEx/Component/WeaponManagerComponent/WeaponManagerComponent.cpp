#include "WeaponManagerComponent.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
}

void UWeaponManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
