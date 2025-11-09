Survival Game Framework

This project is a modular multiplayer survival game built in Unreal Engine 5. It features replicated weapon systems, stat tracking via gameplay tags, and client-side UI integration. Player character has Weapons manager which manages adding, removing and swapping. Weapons are UObjects stored in FastArraySerializer for fast replication. They are highly extensible because of fragment system. For example UI assets can be stored in a simple class deriving from UWeaponFragment. Implemented fragments are used to set weapon default stats with GameplayTags to ensure fast replication. All properties needed for customization are exposed to default editing. WeaponInstance class is also prepared for adding melee or other types of weapons. Stats are handled by StatsComponent attached to character. You just need to select stats in editor. They are also GameplayTags so fast replication and extensibility is checked. It is also modular, because both components can be added to other actors, without complicated initialization. Creating new weapons is also adjusted for people with no programming background. All You need to do is create WeaponDefinition, WeaponInstance and optionally actor for visual representation.

All points required from instruction are made.

Features
WeaponsManager Component

FastArraySerializer-based weapon replication
Equip, swap, and use weapons with server authority
Client-side dispatcher for UI feedback
StatsComponent

GameplayTag-based stat tracking (e.g. Health, MaxHealth)
Server-authoritative damage handling
Client RPCs and dispatchers for responsive UI
Replication Architecture

Subobject replication for weapon instances
OnRep functions for client-only logic
Clean separation of server vs client responsibilities
#@TODO Getting variables for UI could have been done better using dispatchers.

#How to test I deleted all unnecessary files, so You need to Generate Project Files using Your IDE.

After recompiling, if You want to test this project You need to enter "Lvl_Game" select play as listen server and 2 players. Preferably in new editor window.
