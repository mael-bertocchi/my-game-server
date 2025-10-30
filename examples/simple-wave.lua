--- Simple Wave Script Example
--- This demonstrates all available Lua bindings for controlling the game

local shieldItemSpawnTimer = 0
local forceItemSpawnTimer = 0
local missileLaunchTimer = 0
local missileMoveTimer = 0
local enemySpawnTimer = 0
local enemyMoveTimer = 0
local waveTimer = 0

local gameHeight = GetHeight()
local gameWidth = GetWidth()
local duration = 120.0

--- Optional initialization function called when wave loads
function OnInit()
    Log("Hello from OnInit function!")
end

--- Main process function called every frame
--- @param dt number The delta time in seconds since last frame
function Process(dt)
    --- Update timers
    shieldItemSpawnTimer = shieldItemSpawnTimer + dt
    forceItemSpawnTimer = forceItemSpawnTimer + dt
    missileLaunchTimer = missileLaunchTimer + dt
    missileMoveTimer = missileMoveTimer + dt
    enemySpawnTimer = enemySpawnTimer + dt
    enemyMoveTimer = enemyMoveTimer + dt
    waveTimer = waveTimer + dt

    --- Get current enemies
    local enemies = GetEnemies()

    --- Enemy missile launching logic
    if missileLaunchTimer >= 4.0 then
        local missileLaunchProbability = Random(1, 100)
        if missileLaunchProbability <= 70 then
            for _, enemy in ipairs(enemies) do
                local newPosition = Position.new(enemy.position.x - 5, enemy.position.y)
                CreateEnemyMissile(newPosition)
            end
        end
        missileLaunchTimer = 0
    end

    --- Move enemy missiles
    if missileMoveTimer >= 0.2 then
        local missiles = GetEnemyMissiles()
        for _, missile in ipairs(missiles) do
            MoveEnemyMissile(missile.id, -20, 0)
        end
        missileMoveTimer = 0
    end

    --- Spawn enemies periodically
    if waveTimer <= duration and enemySpawnTimer >= 3.0 then
        if #enemies < 5 then
            local spawnY = Random(50, gameHeight - 50)
            local spawnPosition = Position.new(gameWidth - 50, spawnY)

            local enemyTypes = { EnemyType.Generic, EnemyType.Walking, EnemyType.Flying }
            local randomType = enemyTypes[Random(1, #enemyTypes)]

            CreateEnemy(spawnPosition, randomType)
        end
        enemySpawnTimer = 0
    end

    --- Move enemies towards the left
    if enemyMoveTimer >= 0.2 then
        local genericEnemies = GetEnemiesByType(EnemyType.Generic)
        for _, enemy in ipairs(genericEnemies) do
            local dy = Random(-2, 2)
            MoveEnemy(enemy.id, -5, dy, EnemyType.Generic)
        end

        local walkingEnemies = GetEnemiesByType(EnemyType.Walking)
        for _, enemy in ipairs(walkingEnemies) do
            MoveEnemy(enemy.id, -5, 0, EnemyType.Walking)
        end

        local flyingEnemies = GetEnemiesByType(EnemyType.Flying)
        for _, enemy in ipairs(flyingEnemies) do
            local dy = Random(-2, 2)
            MoveEnemy(enemy.id, -5, dy, EnemyType.Flying)
        end

        enemyMoveTimer = 0
    end

    --- Spawn shield items periodically
    if shieldItemSpawnTimer >= 30.0 then
        local shieldItemSpawnProbability = Random(1, 100)
        if shieldItemSpawnProbability <= 30 then
            local playerCount = GetPlayerCount()
            if playerCount > 0 then
                local itemPosition = Position.new(Random(100, gameWidth - 100), Random(50, gameHeight - 50))
                CreateItem(itemPosition, ItemType.Shield)
            end
        end
        shieldItemSpawnTimer = 0
    end

    --- Spawn force items periodically
    if forceItemSpawnTimer >= 15.0 then
        local forceItemSpawnProbability = Random(1, 100)
        if forceItemSpawnProbability <= 100 then
            local playerCount = GetPlayerCount()
            if playerCount > 0 then
                local itemPosition = Position.new(Random(100, gameWidth - 100), Random(50, gameHeight - 50))
                CreateItem(itemPosition, ItemType.Force)
            end
        end
        forceItemSpawnTimer = 0
    end

    --- Check for wave completion
    if waveTimer >= duration and #enemies <= 0 then
        return true
    end
    return false
end
