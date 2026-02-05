# UE5 AI Chase Prototype

A first-person action platformer built in Unreal Engine 5 exploring dynamic AI navigation and player-driven level generation.

## Overview
Players spawn platforms via raycast while an AI enemy continuously pursues them through evolving geometry. The navmesh updates in real-time to accommodate newly created paths.

## Technical Highlights
- **AI Navigation:** Behavior Trees with custom tasks for player sensing and pursuit
- **Dynamic Navmesh:** Runtime navmesh generation and navlink updates
- **Platform Spawning:** Raycast-based placement system with Blueprint/C++ integration
- **Pathfinding:** A* pathfinding adapts to geometry changes during gameplay

## Implementation Details
- **Language:** C++, Blueprints
- **Engine:** Unreal Engine 5.x
- **Key Systems:**
  - AI Perception component for player detection
  - Dynamic Nav Link Proxy for automatic connection generation
  - Line trace (raycast) system for platform instantiation
  - Recast Navigation for runtime mesh updates

## Development Notes
Built as a two-week solo sprint exploring the intersection of procedural level generation and AI agent behavior under dynamic constraints.

## Future Improvements
- Implement difficulty scaling based on platform density
- Add multiple enemy types with varied behavior patterns
- Add some light player-enemy combat with enhanced AI logic for enemies
- Optimize navmesh rebuild frequency for performance
