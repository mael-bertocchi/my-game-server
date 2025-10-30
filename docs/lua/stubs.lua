--- Auto-generated stubs for R-Type server Lua scripting (EmmyLua annotations)
--- Place this file in your workspace and add it to the Lua language server library to get autocomplete and signature help.
--- @meta

--- Position represents a 2D coordinate in the game world
--- @class Position
--- @field x number The X coordinate
--- @field y number The Y coordinate
Position = {}

--- Creates a new Position
--- @param x number The X coordinate
--- @param y number The Y coordinate
--- @return Position
function Position.new(x, y) end

--- Entity represents any game object with a position and identifier
--- @class Entity
--- @field id number Unique identifier of the entity
--- @field position Position The entity's position in the game world
Entity = {}

--- Enemy type enumeration
--- @enum EnemyType
EnemyType = {
    Generic = 3, -- Standard enemy
    Walking = 4, -- Ground-based enemy
    Flying = 5   -- Aerial enemy
}

--- Missile type enumeration
--- @enum MissileType
MissileType = {
    Player = 0, -- Player-fired missile
    Enemy = 1,  -- Enemy-fired missile
    Force = 2   -- Force missile (higher damage)
}

--- Item type enumeration
--- @enum ItemType
ItemType = {
    Shield = 6, -- Shield power-up
    Force = 7   -- Force power-up
}

--- Get all enemies of a specific type
--- @param type EnemyType The type of enemies to retrieve
--- @return Entity[] Array of enemy entities
function GetEnemiesByType(type) end

--- Get all enemies in the game (all types combined)
--- @return Entity[] Array of all enemy entities
function GetEnemies() end

--- Create a new enemy at the specified position
--- @param position Position Where to spawn the enemy
--- @param type? EnemyType The type of enemy to create (default: EnemyType.Generic)
--- @return number The unique identifier of the created enemy
function CreateEnemy(position, type) end

--- Move an enemy by delta values
--- @param id number The unique identifier of the enemy to move
--- @param dx number Delta X movement (negative = left, positive = right)
--- @param dy number Delta Y movement (negative = up, positive = down)
--- @param type EnemyType The type of enemy (required)
function MoveEnemy(id, dx, dy, type) end

--- Remove an enemy from the game
--- @param id number The unique identifier of the enemy to remove
--- @param type EnemyType The type of enemy (required)
function RemoveEnemy(id, type) end

--- Get all missiles of a specific type
--- @param type MissileType The type of missiles to retrieve
--- @return Entity[] Array of missile entities
function GetMissilesByType(type) end

--- Get all player missiles
--- @return Entity[] Array of player missile entities
function GetPlayerMissiles() end

--- Get all enemy missiles
--- @return Entity[] Array of enemy missile entities
function GetEnemyMissiles() end

--- Create a player missile at the specified position
--- @param position Position Where to spawn the missile
--- @return number The unique identifier of the created missile
function CreatePlayerMissile(position) end

--- Create an enemy missile at the specified position
--- @param position Position Where to spawn the missile
--- @return number The unique identifier of the created missile
function CreateEnemyMissile(position) end

--- Create a missile of any type
--- @param position Position Where to spawn the missile
--- @param type MissileType The type of missile to create
--- @return number The unique identifier of the created missile
function CreateMissile(position, type) end

--- Move an enemy missile by delta values
--- @param id number The unique identifier of the missile to move
--- @param dx number Delta X movement (negative = left, positive = right)
--- @param dy number Delta Y movement (negative = up, positive = down)
function MoveEnemyMissile(id, dx, dy) end

--- Remove a missile from the game
--- @param id number The unique identifier of the missile to remove
--- @param type MissileType The type of missile
function RemoveMissile(id, type) end

--- Get all items of a specific type
--- @param type ItemType The type of items to retrieve
--- @return Entity[] Array of item entities
function GetItemsByType(type) end

--- Get all items in the game (all types combined)
--- @return Entity[] Array of all item entities
function GetItems() end

--- Create a new item at the specified position
--- @param position Position Where to spawn the item
--- @param type ItemType The type of item to create
--- @return number The unique identifier of the created item
function CreateItem(position, type) end

--- Remove an item from the game
--- @param id number The unique identifier of the item to remove
--- @param type ItemType The type of item
function RemoveItem(id, type) end

--- Get the width of the game window
--- @return number The window width in pixels
function GetWidth() end

--- Get the height of the game window
--- @return number The window height in pixels
function GetHeight() end

--- Get the unique identifier of the current game
--- @return number The game identifier
function GetGameId() end

--- Get the current number of players in the game
--- @return number Number of active players (0-4)
function GetPlayerCount() end

--- Log a message to the server console
--- @param message string The message to log
function Log(message) end

--- Generate a random integer between min and max (inclusive)
--- @param min number Minimum value
--- @param max number Maximum value
--- @return number Random integer in range [min, max]
function Random(min, max) end

--- Optional initialization function called when the wave loads
--- Use this to set up initial state, spawn starting enemies, etc.
function OnInit() end

--- Main process function called every game frame
--- This is REQUIRED for every wave script
--- @param dt number Delta time in seconds since the last frame
--- @return boolean Return true to end the wave and switch to the next, false to continue
function Process(dt) end
