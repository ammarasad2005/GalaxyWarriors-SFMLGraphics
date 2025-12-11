# Galaxy Warriors - SFML Graphics Game

An advanced space shooter game built with C++ and SFML, demonstrating Object-Oriented Programming principles and custom memory management.

## Features

### Game Mechanics
- Multiple enemy types with unique AI patterns
- Boss battles with multi-phase attacks
- Power-ups and collectibles
- Wave-based progression system
- Dynamic difficulty adjustment
- Achievement system with combo tracking
- High score persistence

### Technical Highlights

#### Custom Memory Management
This project demonstrates **manual memory management** without relying on STL containers:

- **DynamicArray<T>**: Custom dynamic array template replacing `std::vector`
  - Manual memory allocation with `new[]` and `delete[]`
  - Deep copy constructor and assignment operator
  - Dynamic resizing with capacity doubling
  - Index-based access with `operator[]`

- **HashMap<K,V>**: Custom hash map replacing `std::map`
  - Chaining collision resolution
  - Manual memory management for linked list nodes
  - Custom hash function for string keys
  - Key-value pair storage

#### OOP Principles Demonstrated

1. **Inheritance & Polymorphism**
   - Multi-level inheritance: `GameObject → Entity → Ship → PlayerShip/EnemyShip`
   - Pure virtual functions for attack patterns
   - Virtual destructors for proper cleanup

2. **Encapsulation**
   - Private data members with public accessors
   - Protected members for derived class access
   - Friend classes for controlled access

3. **Design Patterns**
   - **Singleton Pattern**: `ColorPalette`, `DifficultyManager`
   - **Strategy Pattern**: `AttackPattern` hierarchy
   - **Factory Pattern**: Enemy creation in `WaveManager`
   - **Object Pool Pattern**: Particle system optimization

4. **Templates**
   - Generic `DynamicArray<T>` container
   - Generic `HashMap<K,V>` container
   - Generic `ObjectPool<T>` for particle management

5. **Exception Handling**
   - Custom `FileException` class
   - Try-catch blocks for error recovery
   - Safe file I/O operations

6. **Operator Overloading**
   - Array subscript operator `[]` in DynamicArray
   - Assignment operator with deep copy
   - Comparison operators for score sorting

7. **Static Members & Methods**
   - Static factory methods for Singletons
   - Static collision counter in `CollisionManager`
   - Static particle tracking

## Custom Container Implementation

### DynamicArray Features
```cpp
- void pushBack(const T& value)      // Add element
- void removeAt(int index)           // Remove element
- int getSize() const                // Get size
- bool isEmpty() const               // Check if empty
- void clear()                       // Remove all elements
- T& operator[](int index)           // Access element
```

### HashMap Features
```cpp
- void insert(const K& key, const V& value)  // Insert key-value
- V* find(const K& key)                      // Find value
- bool contains(const K& key)                // Check existence
- void remove(const K& key)                  // Remove pair
- V& operator[](const K& key)                // Access/insert
```

## Building the Game

### Prerequisites
- C++14 or later
- SFML 2.5 or later
- A C++ compiler (g++, clang++, MSVC)

### Compilation Example
```bash
g++ -std=c++14 *.cpp -lsfml-graphics -lsfml-window -lsfml-system -o galaxy_warriors
```

## Memory Management Details

All STL containers have been replaced with custom implementations:

- **No `std::vector`** - Uses `DynamicArray<T>`
- **No `std::map`** - Uses `HashMap<K,V>`
- **No `std::algorithm`** - Custom sorting and algorithms
- **Manual cleanup** - All `new` paired with `delete`
- **Deep copies** - Proper copy constructors implemented

### Cleanup Pattern
Instead of STL erase-remove idiom:
```cpp
// Before (with STL)
enemies.erase(
    std::remove_if(enemies.begin(), enemies.end(), 
        [](Enemy* e) { return !e->isActive(); }
    ), enemies.end()
);

// After (with DynamicArray)
for (int i = 0; i < enemies.getSize();) {
    if (!enemies[i]->isActive()) {
        delete enemies[i];
        enemies.removeAt(i);
    } else {
        i++;
    }
}
```

## Project Structure

- **Core Classes**: GameObject, Entity, Ship hierarchy
- **Enemies**: BasicEnemy, PatternEnemy, BossEnemy
- **Weapons**: Projectile, PlayerBullet, EnemyBullet
- **Systems**: GameEngine, CollisionManager, WaveManager, ScoreManager
- **Patterns**: Various AttackPattern implementations
- **Utilities**: ColorPalette, DynamicArray, HashMap, Vector2D
- **Effects**: ParticleSystem with object pooling

## Game Controls

- **Arrow Keys**: Move ship
- **Space**: Fire weapons
- **ESC**: Pause/Menu
- **Mouse**: Menu navigation

## Educational Value

This project serves as a comprehensive demonstration of:
- Custom data structure implementation
- Manual memory management in C++
- Object-oriented design principles
- Game development architecture
- Resource management without STL

## License

See LICENSE file for details.

## Author

Developed as an OOP demonstration project showcasing advanced C++ concepts and game programming techniques.
