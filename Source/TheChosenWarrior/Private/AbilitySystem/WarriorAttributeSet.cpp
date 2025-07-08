// All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitcurrentHealth(1.f); //macro already created init functions for us
	InitmaxHealth(1.f);
	InitcurrentRage(1.f);
	InitmaxRage(1.f);
	InitattackPower(1.f);
	InitdefensePower(1.f);
}
