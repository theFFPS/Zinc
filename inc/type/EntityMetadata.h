#pragma once

namespace zinc {

enum class Direction : int { Down, Up, North, South, West, East };
enum class Pose : int { 
    Standing, FallFlying, Sleeping, SpinAttack, Sneaking, LongJumping, Dying, 
    Croaking, UsingTongue, Sitting, Roaring, Sniffing, Emerging, Digging, Sliding, Shooting, Inhaling
};
struct EntityMetadata {

};

}