--- Boss Wave Script

-- Timers
local shieldItemSpawnTimer = 0
local forceItemSpawnTimer = 0
local missileLaunchTimer = 0
local missileMoveTimer = 0
local waveTimer = 0
local bossMoveTimer = 0

-- Game constants
local gameHeight = GetHeight()
local gameWidth = GetWidth()
local duration = 180.0

-- Boss control
local spawned = false
local bossTarget = nil
local bossId = nil

local BOSS_ARRIVE_THRESHOLD = 6
local BOSS_MOVE_INTERVAL = 30.0
local BOSS_MOVE_SPEED = 220.0

-- Missile salvo control
local MISSILE_SALVO_INTERVAL = 6.0
local MISSILE_MIN_COUNT = 4
local MISSILE_MAX_COUNT = 8

-- Per-missile state to move missiles in straight lines
-- keyed by missile id => { vx = float, vy = float }
local bossMissiles = {}

local function GetBossEntity()
    local bosses = GetEnemiesByType(EnemyType.Boss)
    if #bosses > 0 then
        if bossId ~= nil then
            for _, b in ipairs(bosses) do
                if b.id == bossId then
                    return b
                end
            end
            return bosses[1]
        end
        return bosses[1]
    end
    return nil
end

local function PickNewBossTarget()
    local minX = math.max(300, 50)
    local maxX = math.min(900, gameWidth - 50)
    local tx = Random(minX, maxX)
    local ty = Random(50, gameHeight - 50)
    bossTarget = Position.new(tx, ty)
end

local function UpdateBossMovement(dt)
    if bossTarget == nil then
        return
    end

    local bossEntity = GetBossEntity()
    if bossEntity == nil then
        return
    end

    local bx = bossEntity.position.x
    local by = bossEntity.position.y
    local dx = bossTarget.x - bx
    local dy = bossTarget.y - by
    local dist = math.sqrt(dx * dx + dy * dy)

    if dist <= BOSS_ARRIVE_THRESHOLD then
        bossTarget = nil
        return
    end

    local moveDistance = BOSS_MOVE_SPEED * dt
    local ratio = moveDistance / dist
    if ratio > 1 then ratio = 1 end

    local moveX = math.floor(dx * ratio + 0.5)
    local moveY = math.floor(dy * ratio + 0.5)

    if moveX == 0 and math.abs(dx) >= 1 then
        moveX = dx > 0 and 1 or -1
    end
    if moveY == 0 and math.abs(dy) >= 1 then
        moveY = dy > 0 and 1 or -1
    end

    MoveEnemy(bossEntity.id, moveX, moveY, EnemyType.Boss)
end

local function FireBossSalvo()
    local bossEntity = GetBossEntity()
    if bossEntity == nil then
        return
    end

    local count = Random(MISSILE_MIN_COUNT, MISSILE_MAX_COUNT)
    local angleOffset = Random(0, 359)
    local angleStep = 360.0 / count

    for i = 0, count - 1 do
        local angleDeg = angleOffset + i * angleStep + Random(-8, 8)
        local angle = math.rad(angleDeg)

        local spawnX = math.floor(bossEntity.position.x - 20)
        local spawnY = math.floor(bossEntity.position.y)
        local mid = CreateMissile(MissileType.Boss, Position.new(spawnX, spawnY))
        if mid ~= 0 then
            local speed = Random(160, 260)
            local vx = math.cos(angle) * speed
            local vy = math.sin(angle) * speed
            bossMissiles[mid] = { vx = vx, vy = vy }
        end
    end
end

local function UpdateBossMissiles(dt)
    local active = {}
    local missiles = GetMissilesByType(MissileType.Boss)
    for _, m in ipairs(missiles) do
        active[m.id] = m
    end

    for id, data in pairs(bossMissiles) do
        local m = active[id]
        if m == nil then
            bossMissiles[id] = nil
        else
            local dx = data.vx * dt
            local dy = data.vy * dt

            local moveX = math.floor(dx + 0.5)
            local moveY = math.floor(dy + 0.5)

            if moveX == 0 and math.abs(dx) >= 0.5 then
                moveX = dx > 0 and 1 or -1
            end
            if moveY == 0 and math.abs(dy) >= 0.5 then
                moveY = dy > 0 and 1 or -1
            end

            if moveX ~= 0 or moveY ~= 0 then
                MoveMissile(id, MissileType.Boss, moveX, moveY)
            end

            local pos = m.position
            if pos.x < -100 or pos.x > gameWidth + 100 or pos.y < -100 or pos.y > gameHeight + 100 then
                RemoveMissile(id, MissileType.Boss)
                bossMissiles[id] = nil
            end
        end
    end
end

--- Main process function called every frame
--- @param dt number delta time in seconds
function Process(dt)
    -- update timers
    shieldItemSpawnTimer = shieldItemSpawnTimer + dt
    forceItemSpawnTimer = forceItemSpawnTimer + dt
    missileLaunchTimer = missileLaunchTimer + dt
    missileMoveTimer = missileMoveTimer + dt
    waveTimer = waveTimer + dt
    bossMoveTimer = bossMoveTimer + dt

    -- short initial delay
    if waveTimer < 3.0 then
        return false
    end

    -- spawn boss once
    if not spawned then
        local bossPosition = Position.new(math.floor(gameWidth - 100), math.floor(gameHeight / 2))
        bossId = CreateEnemy(EnemyType.Boss, bossPosition)
        spawned = true
        bossTarget = nil
        bossMoveTimer = BOSS_MOVE_INTERVAL
    end

    -- choose new boss target at interval
    if bossMoveTimer >= BOSS_MOVE_INTERVAL then
        bossMoveTimer = bossMoveTimer - BOSS_MOVE_INTERVAL
        PickNewBossTarget()
    end

    -- update boss movement
    UpdateBossMovement(dt)

    -- fire straight salvos on interval
    if missileLaunchTimer >= MISSILE_SALVO_INTERVAL then
        missileLaunchTimer = missileLaunchTimer - MISSILE_SALVO_INTERVAL
        FireBossSalvo()
    end

    -- move boss missiles in small timesteps for smoothness
    if missileMoveTimer >= 0.01 then
        while missileMoveTimer >= 0.01 do
            UpdateBossMissiles(0.01)
            missileMoveTimer = missileMoveTimer - 0.01
        end
    end

    -- optional: spawn items similar to default example
    if shieldItemSpawnTimer >= 45.0 then
        local shieldItemSpawnProbability = Random(1, 100)
        if shieldItemSpawnProbability <= 20 then
            local count = GetPlayerCount()
            if count > 0 then
                local itemPosition = Position.new(math.floor(Random(100, gameWidth - 100)),
                    math.floor(Random(50, gameHeight - 50)))
                CreateItem(itemPosition, ItemType.Shield)
            end
        end
        shieldItemSpawnTimer = 0
    end

    if forceItemSpawnTimer >= 50.0 then
        local forceItemSpawnProbability = Random(1, 100)
        if forceItemSpawnProbability <= 10 then
            local count = GetPlayerCount()
            if count > 0 then
                local itemPosition = Position.new(math.floor(Random(100, gameWidth - 100)),
                    math.floor(Random(50, gameHeight - 50)))
                CreateItem(itemPosition, ItemType.Force)
            end
        end
        forceItemSpawnTimer = 0
    end

    -- cleanup missile state (in case some missiles were removed externally)
    local realMissiles = GetMissilesByType(MissileType.Boss)
    local existing = {}
    for _, m in ipairs(realMissiles) do existing[m.id] = true end
    for id, _ in pairs(bossMissiles) do
        if not existing[id] then bossMissiles[id] = nil end
    end

    -- end condition: wave duration elapsed and no enemies left
    local enemies = GetEnemies()
    if waveTimer >= duration and #enemies <= 0 then
        return true
    end
    return false
end
