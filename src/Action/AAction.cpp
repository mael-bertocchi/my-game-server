/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AAction.cpp
*/

#include "Action/AAction.hpp"

void Action::AAction::ReceiveMessage(const std::uint32_t, const std::vector<std::uint8_t>&) {}

void Action::AAction::SendMessage(const std::uint32_t, const std::any&) const {}

std::optional<std::string> Action::AAction::ValidatePlayer(const std::shared_ptr<Network::Player>& player, PlayerValidation flags) const
{
    if (player) {
        if ((flags & PlayerValidation::Administrator) != PlayerValidation::None && player->GetRole() != Role::Administrator) {
            return "Player is not an administrator";
        }
        if ((flags & PlayerValidation::Connected) != PlayerValidation::None && !player->IsConnected()) {
            return "Player not connected";
        }
        if ((flags & PlayerValidation::NotConnected) != PlayerValidation::None && player->IsConnected()) {
            return "Player already connected";
        }
        if ((flags & PlayerValidation::Playing) != PlayerValidation::None && !player->IsPlaying()) {
            return "Player not in a game";
        }
        if ((flags & PlayerValidation::NotPlaying) != PlayerValidation::None && player->IsPlaying()) {
            return "Player already in a game";
        }
    }
    return std::nullopt;
}

std::optional<std::string> Action::AAction::ValidateGame(const std::shared_ptr<Engine::Game>& game, GameValidation flags) const
{
    if (game) {
        const std::uint8_t count = game->GetPlayerCount();

        if ((flags & GameValidation::Started) != GameValidation::None && !game->IsStarted()) {
            return "Game not started";
        }
        if ((flags & GameValidation::NotStarted) != GameValidation::None && game->IsStarted()) {
            return "Game already started";
        }
        if ((flags & GameValidation::Full) != GameValidation::None && count < MAX_PLAYER_PER_GAMES) {
            return "Game not full";
        }
        if ((flags & GameValidation::NotFull) != GameValidation::None && count >= MAX_PLAYER_PER_GAMES) {
            return "Game already full";
        }
        if ((flags & GameValidation::Min) != GameValidation::None && count < MIN_PLAYER_PER_GAMES) {
            return "Not enough players to start the game";
        }
    }
    return std::nullopt;
}
