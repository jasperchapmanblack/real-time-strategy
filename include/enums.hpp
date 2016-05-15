#pragma once

namespace AnimationState {
	enum Enum {
		Idle,
		Walking,
		Attacking,
		Dying
	};
};

namespace UpdateStatus {
	enum Enum {
		OK,
		Remove
	};
};

namespace TeamColor {
	enum Enum {
		Null, // for when we don't want to recolor
		Pink,
		Red,
		Blue,
		Green,
		Purple,
		Orange,
		Brown,
		Yellow,
		Teal,
		Pale,
		Iris,
		Olive,
		num
	};
};

namespace DamageType {
	enum Enum {
		Electrical, // good against shields, real bad against armor
		Thermal,    // mediocre against shields and armor
		Kinetic,    // bad against shields, pretty good against armor
		Explosive,  // real bad against shields, great against armor
		num
	};
};

namespace ResourceType {
	enum Enum {
		Metal,   // consumed on build order
		Crystal, // consumed on build order
		Supply,  // You get supply back when a unit is destroyed
		num
	};
};

namespace CommandType {
	enum Enum {
		Idle, 
		Gotocoord,
		Attack,
		Attackmove,
		Halt,
		Build
	};
};

namespace QueueSetting {
	enum Enum {
		Overwrite,
		Front,
		Back
	};
};


namespace TerrainType {
	enum Enum {
		None,
		Grass,
		Water,
		Road,
		num
	};
};

