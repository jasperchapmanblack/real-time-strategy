#pragma once
#include <memory>

class WeaponTemplate;
class Unit;

class Weapon
{
public:
	Weapon(std::shared_ptr<WeaponTemplate> _weaponTemplate, Unit* _owner);

	void fire(Unit& target);
	virtual void update();
	const std::shared_ptr<WeaponTemplate> weaponTemplate;
	const std::shared_ptr<Unit> owner;

	int ticksUntilCanFire;
};