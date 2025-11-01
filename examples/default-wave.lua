--- Default Wave Script

local shieldItemSpawnTimer = 0
local forceItemSpawnTimer = 0
local missileLaunchTimer = 0
local missileMoveTimer = 0
local enemySpawnTimer = 0
local enemyMoveTimer = 0
local waveTimer = 0

local gameHeight = GetHeight()
local gameWidth = GetWidth()
local duration = 20.0

-- Sinusoidal movement state for flying enemies:
-- flyingPhases[id] = { phase = number, baseY = number, amplitude = number, freq = number }
local flyingPhases = {}
-- A running time value used to evaluate the sine wave
local sinTime = 0.0

local function toInt(n)
    if n == nil then
        return 0
    end
    if n >= 0 then
        return math.floor(n + 0.5)
    else
        return math.ceil(n - 0.5)
    end
end

--- Optional initialization function called when wave loads
function OnInit()
    Log("Hello from OnInit function!")

    local existingFlying = GetEnemiesByType(EnemyType.Flying)
    for _, e in ipairs(existingFlying) do
        flyingPhases[e.id] = {
            phase = (Random(0, 6283) / 1000.0),
            baseY = e.position.y,
            amplitude = Random(12, 28),
            freq = Random(1, 3)
        }
    end
end

--- Main process function called every frame
--- @param dt number The delta time in seconds since last frame
function Process(dt)
    -- Update timers
    shieldItemSpawnTimer = shieldItemSpawnTimer + dt
    forceItemSpawnTimer = forceItemSpawnTimer + dt
    missileLaunchTimer = missileLaunchTimer + dt
    missileMoveTimer = missileMoveTimer + dt
    enemySpawnTimer = enemySpawnTimer + dt
    enemyMoveTimer = enemyMoveTimer + dt
    waveTimer = waveTimer + dt

    -- advance sinusoidal time
    sinTime = sinTime + dt

    if waveTimer < 5.0 then
        return false
    end

    -- Get current enemies
    local enemies = GetEnemies()

    -- Enemy missile launching logic
    if missileLaunchTimer >= 4.0 then
        local missileLaunchProbability = Random(1, 100)
        if missileLaunchProbability <= 70 then
            for _, enemy in ipairs(enemies) do
                local newPosition = Position.new(enemy.position.x - 5, enemy.position.y)
                CreateMissile(MissileType.Enemy, newPosition)
            end
        end
        missileLaunchTimer = 0
    end

    -- Move enemy missiles
    if missileMoveTimer >= 0.2 then
        local missiles = GetMissilesByType(MissileType.Enemy)
        for _, missile in ipairs(missiles) do
            MoveMissile(missile.id, MissileType.Enemy, toInt(-10), toInt(0))
        end
        missileMoveTimer = 0
    end

    -- Spawn enemies periodically
    if waveTimer <= duration and enemySpawnTimer >= 3.0 then
        if #enemies < 5 then
            local types = { EnemyType.Generic, EnemyType.Walking, EnemyType.Flying }
            local type = types[Random(1, #types)]

            local y = 0
            if type ~= EnemyType.Walking then
                y = Random(30, 320)
            else
                y = 380
            end
            local position = Position.new(gameWidth - 50, y)

            local newId = CreateEnemy(type, position)
            if type == EnemyType.Flying and newId ~= nil then
                flyingPhases[newId] = {
                    phase = (Random(0, 6283) / 1000.0),
                    baseY = position.y,
                    amplitude = Random(12, 28),
                    freq = Random(1, 3)
                }
            end
        end
        enemySpawnTimer = 0
    end

    -- Move enemies towards the left
    if enemyMoveTimer >= 0.2 then
        local genericEnemies = GetEnemiesByType(EnemyType.Generic)
        for _, enemy in ipairs(genericEnemies) do
            local dyf = Random(-2, 2)
            MoveEnemy(enemy.id, toInt(-5), toInt(dyf), EnemyType.Generic)
        end

        local walkingEnemies = GetEnemiesByType(EnemyType.Walking)
        for _, enemy in ipairs(walkingEnemies) do
            MoveEnemy(enemy.id, toInt(-5), toInt(0), EnemyType.Walking)
        end

        local flyingEnemies = GetEnemiesByType(EnemyType.Flying)
        local present = {}
        for _, enemy in ipairs(flyingEnemies) do
            present[enemy.id] = true

            local info = flyingPhases[enemy.id]
            if not info then
                info = {
                    phase = (Random(0, 6283) / 1000.0),
                    baseY = enemy.position.y,
                    amplitude = Random(12, 28),
                    freq = Random(1, 3)
                }
                flyingPhases[enemy.id] = info
            end

            local targetY = info.baseY + info.amplitude * math.sin(info.phase + sinTime * info.freq)
            local dy = targetY - enemy.position.y
            local dx = -5

            MoveEnemy(enemy.id, toInt(dx), toInt(dy), EnemyType.Flying)
        end

        for id, _ in pairs(flyingPhases) do
            if not present[id] then
                flyingPhases[id] = nil
            end
        end

        enemyMoveTimer = 0
    end

    -- Spawn shield items periodically
    if shieldItemSpawnTimer >= 30.0 then
        local shieldItemSpawnProbability = Random(1, 100)
        if shieldItemSpawnProbability <= 30 then
            local count = GetPlayerCount()
            if count > 0 then
                local itemPosition = Position.new(Random(100, gameWidth - 100), Random(50, gameHeight - 50))
                CreateItem(itemPosition, ItemType.Shield)
            end
        end
        shieldItemSpawnTimer = 0
    end

    -- Spawn force items periodically
    if forceItemSpawnTimer >= 45.0 then
        local forceItemSpawnProbability = Random(1, 100)
        if forceItemSpawnProbability <= 20 then
            local count = GetPlayerCount()
            if count > 0 then
                local itemPosition = Position.new(Random(100, gameWidth - 100), Random(50, gameHeight - 50))
                CreateItem(itemPosition, ItemType.Force)
            end
        end
        forceItemSpawnTimer = 0
    end

    -- Check for wave completion
    if waveTimer >= duration and #enemies <= 0 then
        return true
    end
    return false
end
