/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Dispatcher.hpp
*/

#include "Action/Dispatcher.hpp"
#include "Action/List/CRE.hpp"
#include "Action/List/DFY.hpp"
#include "Action/List/DIE.hpp"
#include "Action/List/ERR.hpp"
#include "Action/List/JON.hpp"
#include "Action/List/LVE.hpp"
#include "Action/List/OVE.hpp"
#include "Action/List/POS.hpp"
#include "Action/List/STS.hpp"
#include "Action/List/SHT.hpp"
#include "Action/List/SPW.hpp"
#include "Action/List/STR.hpp"
#include "Action/List/STP.hpp"

void Action::Dispatcher::ReceiveMessage(const ActionType type, const std::uint32_t id, const std::vector<std::uint8_t>& body)
{
    Action::IAction* action = GetAction(type);

    if (action) {
        action->ReceiveMessage(id, body);
    }
}

void Action::Dispatcher::SendMessage(const ActionType type, const std::uint32_t id, const std::any& content)
{
    Action::IAction* action = GetAction(type);

    if (action) {
        action->SendMessage(id, content);
    }
}

Action::IAction* Action::Dispatcher::GetAction(const ActionType type)
{
    static const std::unordered_map<ActionType, std::unique_ptr<Action::IAction>> actions = []() {
        std::unordered_map<ActionType, std::unique_ptr<Action::IAction>> elements = {};

        elements[ActionType::CRE] = std::make_unique<Action::List::CRE>();
        elements[ActionType::DFY] = std::make_unique<Action::List::DFY>();
        elements[ActionType::DIE] = std::make_unique<Action::List::DIE>();
        elements[ActionType::ERR] = std::make_unique<Action::List::ERR>();
        elements[ActionType::JON] = std::make_unique<Action::List::JON>();
        elements[ActionType::LVE] = std::make_unique<Action::List::LVE>();
        elements[ActionType::OVE] = std::make_unique<Action::List::OVE>();
        elements[ActionType::POS] = std::make_unique<Action::List::POS>();
        elements[ActionType::STS] = std::make_unique<Action::List::STS>();
        elements[ActionType::SHT] = std::make_unique<Action::List::SHT>();
        elements[ActionType::SPW] = std::make_unique<Action::List::SPW>();
        elements[ActionType::STR] = std::make_unique<Action::List::STR>();
        elements[ActionType::STP] = std::make_unique<Action::List::STP>();

        return elements;
    }();

    auto it = actions.find(type);
    if (it != actions.end()) {
        return it->second.get();
    }
    return nullptr;
}
