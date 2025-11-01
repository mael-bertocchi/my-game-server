/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Collision.cpp
*/

#include "Engine/Collision.hpp"
#include "Storage/Player.hpp"
#include "Variables.hpp"

Engine::Collision::Result Engine::Collision::Check(const std::array<std::uint32_t, MAX_PLAYER_PER_GAMES>& ids, const Enemies& enemies, const Missiles& missiles, const Items& items)
{
    std::unordered_map<std::uint32_t, BoundingBox> playerMissileBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> forceMissileBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> enemyMissileBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> genericEnemyBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> walkingEnemyBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> playerShieldBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> bossMissileBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> flyingEnemyBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> shieldItemBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> bossEnemyBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> forceItemBoxes = {};
    std::unordered_map<std::uint32_t, BoundingBox> playerBoxes = {};

    Result result = {};

    for (const auto& id : ids) {
        if (id != 0) {
            auto player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
            if (player && player->IsAlive()) {
                playerBoxes[id] = CreateBoundingBox(player->GetPosition(), 82, 70);
                if (!player->IsStatisticActive(Statistic::Shield)) {
                    playerShieldBoxes[id] = playerBoxes[id];
                }
            }
        }
    }

    for (const auto& [id, enemy] : enemies.generic) {
        genericEnemyBoxes[enemy.id] = CreateBoundingBox(enemy.position, 164, 164);
    }

    for (const auto& [id, enemy] : enemies.walking) {
        walkingEnemyBoxes[enemy.id] = CreateBoundingBox(enemy.position, 230, 166);
    }

    for (const auto& [id, enemy] : enemies.flying) {
        flyingEnemyBoxes[enemy.id] = CreateBoundingBox(enemy.position, 80, 44);
    }

    for (const auto& [id, enemy] : enemies.boss) {
        bossEnemyBoxes[enemy.id] = CreateBoundingBox(enemy.position, 300, 300);
    }

    for (const auto& [id, missile] : missiles.player) {
        playerMissileBoxes[missile.id] = CreateBoundingBox(missile.position, 60, 20);
    }

    for (const auto& [id, missile] : missiles.force) {
        forceMissileBoxes[missile.id] = CreateBoundingBox(missile.position, 28, 20);
    }

    for (const auto& [id, missile] : missiles.enemy) {
        enemyMissileBoxes[missile.id] = CreateBoundingBox(missile.position, 38, 30);
    }

    for (const auto& [id, missile] : missiles.boss) {
        bossMissileBoxes[missile.id] = CreateBoundingBox(missile.position, 100, 40);
    }

    for (const auto& [id, shield] : items.shield) {
        shieldItemBoxes[shield.id] = CreateBoundingBox(shield.position, 42, 44);
    }

    for (const auto& [id, force] : items.force) {
        forceItemBoxes[force.id] = CreateBoundingBox(force.position, 42, 44);
    }

    for (const auto& [pmId, pmBox] : playerMissileBoxes) {
        if (result.missiles.player.count(pmId)) {
            continue;
        }

        for (const auto& [emId, emBox] : enemyMissileBoxes) {
            if (result.missiles.enemy.count(emId)) {
                continue;
            }

            if (DoesCollide(pmBox, emBox)) {
                result.missiles.player.insert(pmId);
                result.missiles.enemy.insert(emId);
                break;
            }
        }
    }

    for (const auto& [pfmId, pfmBox] : forceMissileBoxes) {
        if (result.missiles.force.count(pfmId)) {
            continue;
        }

        for (const auto& [emId, emBox] : enemyMissileBoxes) {
            if (result.missiles.enemy.count(emId)) {
                continue;
            }

            if (DoesCollide(pfmBox, emBox)) {
                result.missiles.force.insert(pfmId);
                result.missiles.enemy.insert(emId);
                break;
            }
        }
    }

    for (const auto& [pmId, pmBox] : playerMissileBoxes) {
        if (result.missiles.player.count(pmId)) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : genericEnemyBoxes) {
            if (result.damaged.generic.count(enemyId)) {
                continue;
            }

            if (DoesCollide(pmBox, enemyBox)) {
                result.missiles.player.insert(pmId);
                result.damaged.generic[enemyId] = PLAYER_MISSILE_DAMAGE;
                break;
            }
        }
    }

    for (const auto& [pmId, pmBox] : playerMissileBoxes) {
        if (result.missiles.player.count(pmId)) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : walkingEnemyBoxes) {
            if (result.damaged.walking.count(enemyId)) {
                continue;
            }

            if (DoesCollide(pmBox, enemyBox)) {
                result.missiles.player.insert(pmId);
                result.damaged.walking[enemyId] = PLAYER_MISSILE_DAMAGE;
                break;
            }
        }
    }

    for (const auto& [pmId, pmBox] : playerMissileBoxes) {
        if (result.missiles.player.count(pmId)) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : flyingEnemyBoxes) {
            if (result.damaged.flying.count(enemyId)) {
                continue;
            }

            if (DoesCollide(pmBox, enemyBox)) {
                result.missiles.player.insert(pmId);
                result.damaged.flying[enemyId] = PLAYER_MISSILE_DAMAGE;
                break;
            }
        }
    }

    for (const auto& [pfmId, pfmBox] : forceMissileBoxes) {
        if (result.missiles.player.count(pfmId)) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : genericEnemyBoxes) {
            if (result.damaged.generic.count(enemyId)) {
                continue;
            }

            if (DoesCollide(pfmBox, enemyBox)) {
                result.missiles.force.insert(pfmId);
                result.damaged.generic[enemyId] = FORCE_MISSILE_DAMAGE;
                break;
            }
        }
    }

    for (const auto& [pfmId, pfmBox] : forceMissileBoxes) {
        if (result.missiles.force.count(pfmId)) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : walkingEnemyBoxes) {
            if (result.damaged.walking.count(enemyId)) {
                continue;
            }

            if (DoesCollide(pfmBox, enemyBox)) {
                result.missiles.force.insert(pfmId);
                result.damaged.walking[enemyId] = FORCE_MISSILE_DAMAGE;
                break;
            }
        }
    }

    for (const auto& [pfmId, pfmBox] : forceMissileBoxes) {
        if (result.missiles.force.count(pfmId)) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : flyingEnemyBoxes) {
            if (result.damaged.flying.count(enemyId)) {
                continue;
            }

            if (DoesCollide(pfmBox, enemyBox)) {
                result.missiles.force.insert(pfmId);
                result.damaged.flying[enemyId] = FORCE_MISSILE_DAMAGE;
                break;
            }
        }
    }

    for (const auto& [emId, emBox] : enemyMissileBoxes) {
        if (result.missiles.enemy.count(emId)) {
            continue;
        }

        for (const auto& [playerId, playerBox] : playerShieldBoxes) {
            if (std::find(result.players.begin(), result.players.end(), playerId) != result.players.end()) {
                continue;
            }

            if (DoesCollide(emBox, playerBox)) {
                result.missiles.enemy.insert(emId);
                result.players.push_back(playerId);
                break;
            }
        }
    }

    for (const auto& [bmId, bmBox] : bossMissileBoxes) {
        if (result.missiles.boss.count(bmId)) {
            continue;
        }

        for (const auto& [playerId, playerBox] : playerShieldBoxes) {
            if (std::find(result.players.begin(), result.players.end(), playerId) != result.players.end()) {
                continue;
            }

            if (DoesCollide(bmBox, playerBox)) {
                result.missiles.boss.insert(bmId);
                result.players.push_back(playerId);
                break;
            }
        }
    }

    for (const auto& [pmId, pmBox] : playerMissileBoxes) {
        if (result.missiles.player.count(pmId)) {
            continue;
        }

        for (const auto& [bmId, bmBox] : bossMissileBoxes) {
            if (result.missiles.boss.count(bmId)) {
                continue;
            }

            if (DoesCollide(pmBox, bmBox)) {
                result.missiles.player.insert(pmId);
                result.missiles.boss.insert(bmId);
                break;
            }
        }
    }

    for (const auto& [pfmId, pfmBox] : forceMissileBoxes) {
        if (result.missiles.force.count(pfmId)) {
            continue;
        }

        for (const auto& [bmId, bmBox] : bossMissileBoxes) {
            if (result.missiles.boss.count(bmId)) {
                continue;
            }

            if (DoesCollide(pfmBox, bmBox)) {
                result.missiles.force.insert(pfmId);
                result.missiles.boss.insert(bmId);
                break;
            }
        }
    }

    for (const auto& [pmId, pmBox] : playerMissileBoxes) {
        if (result.missiles.player.count(pmId)) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : bossEnemyBoxes) {
            if (result.damaged.boss.count(enemyId)) {
                continue;
            }

            if (DoesCollide(pmBox, enemyBox)) {
                result.missiles.player.insert(pmId);
                result.damaged.boss[enemyId] = PLAYER_MISSILE_DAMAGE;
                break;
            }
        }
    }

    for (const auto& [pfmId, pfmBox] : forceMissileBoxes) {
        if (result.missiles.force.count(pfmId)) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : bossEnemyBoxes) {
            if (result.damaged.boss.count(enemyId)) {
                continue;
            }

            if (DoesCollide(pfmBox, enemyBox)) {
                result.missiles.force.insert(pfmId);
                result.damaged.boss[enemyId] = FORCE_MISSILE_DAMAGE;
                break;
            }
        }
    }

    for (const auto& [playerId, playerBox] : playerShieldBoxes) {
        if (std::find(result.players.begin(), result.players.end(), playerId) != result.players.end()) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : bossEnemyBoxes) {
            if (result.enemies.boss.count(enemyId)) {
                continue;
            }

            if (DoesCollide(playerBox, enemyBox)) {
                result.players.push_back(playerId);
                result.enemies.boss.insert(enemyId);
                break;
            }
        }
    }

    for (const auto& [playerId, playerBox] : playerShieldBoxes) {
        if (std::find(result.players.begin(), result.players.end(), playerId) != result.players.end()) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : genericEnemyBoxes) {
            if (result.enemies.generic.count(enemyId)) {
                continue;
            }

            if (DoesCollide(playerBox, enemyBox)) {
                result.players.push_back(playerId);
                result.enemies.generic.insert(enemyId);
                break;
            }
        }
    }

    for (const auto& [playerId, playerBox] : playerShieldBoxes) {
        if (std::find(result.players.begin(), result.players.end(), playerId) != result.players.end()) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : walkingEnemyBoxes) {
            if (result.enemies.walking.count(enemyId)) {
                continue;
            }

            if (DoesCollide(playerBox, enemyBox)) {
                result.players.push_back(playerId);
                result.enemies.walking.insert(enemyId);
                break;
            }
        }
    }

    for (const auto& [playerId, playerBox] : playerShieldBoxes) {
        if (std::find(result.players.begin(), result.players.end(), playerId) != result.players.end()) {
            continue;
        }

        for (const auto& [enemyId, enemyBox] : flyingEnemyBoxes) {
            if (result.enemies.flying.count(enemyId)) {
                continue;
            }

            if (DoesCollide(playerBox, enemyBox)) {
                result.players.push_back(playerId);
                result.enemies.flying.insert(enemyId);
                break;
            }
        }
    }

    for (const auto& [playerId, playerBox] : playerBoxes) {
        if (result.shields.count(playerId)) {
            continue;
        }
        for (const auto& [shieldId, shieldBox] : shieldItemBoxes) {
            bool collected = false;
            for (const auto& [sid, pid] : result.shields) {
                if (sid != shieldId) {
                    continue;
                }
                collected = true;
                break;
            }
            if (collected) {
                continue;
            }
            if (DoesCollide(playerBox, shieldBox)) {
                result.shields[shieldId] = playerId;
                break;
            }
        }
    }

    for (const auto& [playerId, playerBox] : playerBoxes) {
        if (result.forces.count(playerId)) {
            continue;
        }
        for (const auto& [forceId, forceBox] : forceItemBoxes) {
            bool collected = false;
            for (const auto& [fid, pid] : result.forces) {
                if (fid != forceId) {
                    continue;
                }
                collected = true;
                break;
            }
            if (collected) {
                continue;
            }
            if (DoesCollide(playerBox, forceBox)) {
                result.forces[forceId] = playerId;
                break;
            }
        }
    }

    return result;
}

bool Engine::Collision::DoesCollide(const BoundingBox& first, const BoundingBox& second)
{
    return (first.x < second.x + second.width && first.x + first.width > second.x && first.y < second.y + second.height && first.y + first.height > second.y);
}

Engine::Collision::BoundingBox Engine::Collision::CreateBoundingBox(const Position& position, const std::uint16_t width, const std::uint16_t height)
{
    return { .height = height, .width = width, .x = position.x, .y = position.y };
}
