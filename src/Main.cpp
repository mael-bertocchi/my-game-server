/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Main.cpp
*/

#include "Miscellaneous/Environment.hpp"
#include "Exception/Game/NotExists.hpp"
#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Network/Transceiver.hpp"
#include "Exception/Generic.hpp"
#include "Storage/Database.hpp"
#include "Storage/Game.hpp"
#include "Variables.hpp"

#include <algorithm>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <array>

/**
 * @brief Atomic boolean to control the running state of the application.
 */
std::atomic<bool> isRunning(true);

/**
 * @brief Signal handler to gracefully stop the application.
 *
 * @param signal The signal number received.
 */
static void SignalHandler(std::int32_t signal)
{
    switch (signal) {
        case SIGINT:
            isRunning = false;
            break;
        default:
            break;
    }
}

/**
 * @brief Create and start transceiver threads.
 *
 * @return An array of threads for the transceiver operations.
 */
static std::pair<std::shared_ptr<Network::Transceiver>, std::array<std::thread, 4>> CreateTransciever()
{
    std::shared_ptr<Network::Transceiver> transciever = std::make_shared<Network::Transceiver>();
    std::array<std::thread, 4> threads = {
        std::thread(&Network::Transceiver::SecureReceive, transciever.get(), std::cref(isRunning)),
        std::thread(&Network::Transceiver::SecureSend, transciever.get(), std::cref(isRunning)),
        std::thread(&Network::Transceiver::Receive, transciever.get(), std::cref(isRunning)),
        std::thread(&Network::Transceiver::Send, transciever.get(), std::cref(isRunning))
    };

    return {transciever, std::move(threads)};
}

/**
 * @brief Clean up and join transceiver threads.
 *
 * @param threads The array of threads to clean up.
 */
static void CleanTransciever(std::array<std::thread, 4>& threads)
{
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

/**
 * @brief Run the server with separate threads for networking and game processing.
 */
static void Run()
{
    Misc::Clock clock;

    try {
        while (isRunning.load()) {
            if (clock.HasElapsed(GAME_PROCESS_INTERVAL_MS)) {
                std::vector<std::uint32_t> ids = Storage::Cache::Game::GetInstance().GetGameIds();
                std::vector<std::uint32_t> inactives = {};

                for (const std::uint32_t& id : ids) {
                    try {
                        std::shared_ptr<Engine::Game> game = Storage::Cache::Game::GetInstance().GetGameById(id);
                        if (game->IsInactive()) {
                            inactives.push_back(id);
                        } else {
                            game->Process();
                        }
                        game.reset();
                    } catch (const Exception::Game::NotExistsError&) {
                        continue;
                    }
                }

                for (const auto& id : inactives) {
                    try {
                        Storage::Cache::Game::GetInstance().RemoveGame(id);
                    } catch (const Exception::Game::NotExistsError&) {
                        continue;
                    }
                }

                clock.Reset();
            }
        }
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process main loop: {}", ex.what()), Misc::Logger::LogLevel::Critical);
        isRunning = false;
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process main loop: Unknown error"), Misc::Logger::LogLevel::Critical);
        isRunning = false;
    }
}

/**
 * @brief Retrieve command-line arguments and start the application.
 *
 * @return Exit status code.
 */
std::int32_t main(std::int32_t argc, char **argv)
{
    try {
        if (argc > 2) {
            std::string file = Misc::Utils::GetTextOption(argv, argv + argc, "--file");

            if (Misc::Utils::HasOption(argv, argv + argc, "--debug")) {
                Misc::Logger::GetInstance().SetDebugMode(true);
            }

            Misc::Env::GetInstance().LoadFromFile(file);
            Storage::Database::GetInstance().Connect();

            auto [transciever, threads] = CreateTransciever();

            std::srand(static_cast<std::uint32_t>(std::time(nullptr)));
            std::signal(SIGINT, SignalHandler);

            Run();
            CleanTransciever(threads);

            Storage::Database::GetInstance().Disconnect();
        }
    } catch (const Exception::GenericError& ex) {
        std::cerr << ex << std::endl;
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
}
