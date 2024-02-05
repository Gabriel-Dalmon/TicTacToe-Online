#include "EventsManager.h"
#include "../../engine/core/GameManager.h"
#include "../rendering/Window.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "AbstractCommand.h"
#include <iostream>

EventsManager::EventsManager()
{
}

EventsManager::~EventsManager()
{
}

void EventsManager::handleSFMLEvents()
{
    while (GameManager::instance().getWindow()->getSFMLObject()->pollEvent(this->event))
    {
        this->trigger((this->*(EventsManager::SFMLMapper[this->event.type]))());
    }
}

void EventsManager::trigger(EventName eventName)
{
    //std::cout << (eventName == KEY_ESCAPE_PRESSED ? eventCallbacksMap[eventName].size() : NULL);
    for (AbstractCommand* command : eventCallbacksMap[eventName])
    {
        command->execute(&(this->context));
    }
}

std::unordered_map <sf::Event::EventType, EventName(EventsManager::*)()> EventsManager::SFMLMapper = {
    { sf::Event::Closed, &getSFMLClosedEventName },
    { sf::Event::Resized, &ignoreSFMLEventType },
    { sf::Event::LostFocus, &ignoreSFMLEventType },
    { sf::Event::GainedFocus, &ignoreSFMLEventType },
    { sf::Event::TextEntered, &ignoreSFMLEventType },
    { sf::Event::KeyPressed, &getSFMLKeyPressedEventName},
    { sf::Event::KeyReleased, &getSFMLKeyReleasedEventName },
    { sf::Event::MouseWheelMoved, &ignoreSFMLEventType },
    { sf::Event::MouseWheelScrolled, &ignoreSFMLEventType },
    { sf::Event::MouseButtonPressed, &getSFMLMouseButtonPressedEventName },
    { sf::Event::MouseButtonReleased, &getSFMLMouseButtonReleasedEventName },
    { sf::Event::MouseMoved, &ignoreSFMLEventType },
    { sf::Event::MouseEntered, &ignoreSFMLEventType },
    { sf::Event::MouseLeft, &ignoreSFMLEventType },
    { sf::Event::JoystickButtonPressed, &ignoreSFMLEventType },
    { sf::Event::JoystickButtonReleased, &ignoreSFMLEventType },
    { sf::Event::JoystickMoved, &ignoreSFMLEventType },
    { sf::Event::JoystickConnected, &ignoreSFMLEventType },
    { sf::Event::JoystickDisconnected, &ignoreSFMLEventType },
    { sf::Event::TouchBegan, &ignoreSFMLEventType },
    { sf::Event::TouchMoved, &ignoreSFMLEventType },
    { sf::Event::TouchEnded, &ignoreSFMLEventType },
    { sf::Event::SensorChanged, &ignoreSFMLEventType }

};

std::unordered_map<sf::Keyboard::Key, EventName> EventsManager::SFMLKeyPressedTranslationMap = {
    {sf::Keyboard::A, EventName::KEY_A_PRESSED},
    {sf::Keyboard::B, EventName::KEY_B_PRESSED},
    {sf::Keyboard::C, EventName::KEY_C_PRESSED},
    {sf::Keyboard::D, EventName::KEY_D_PRESSED},
    {sf::Keyboard::E, EventName::KEY_E_PRESSED},
    {sf::Keyboard::F, EventName::KEY_F_PRESSED},
    {sf::Keyboard::G, EventName::KEY_G_PRESSED},
    {sf::Keyboard::H, EventName::KEY_H_PRESSED},
    {sf::Keyboard::I, EventName::KEY_I_PRESSED},
    {sf::Keyboard::J, EventName::KEY_J_PRESSED},
    {sf::Keyboard::K, EventName::KEY_K_PRESSED},
    {sf::Keyboard::L, EventName::KEY_L_PRESSED},
    {sf::Keyboard::M, EventName::KEY_M_PRESSED},
    {sf::Keyboard::N, EventName::KEY_N_PRESSED},
    {sf::Keyboard::O, EventName::KEY_O_PRESSED},
    {sf::Keyboard::P, EventName::KEY_P_PRESSED},
    {sf::Keyboard::Q, EventName::KEY_Q_PRESSED},
    {sf::Keyboard::R, EventName::KEY_R_PRESSED},
    {sf::Keyboard::S, EventName::KEY_S_PRESSED},
    {sf::Keyboard::T, EventName::KEY_T_PRESSED},
    {sf::Keyboard::U, EventName::KEY_U_PRESSED},
    {sf::Keyboard::V, EventName::KEY_V_PRESSED},
    {sf::Keyboard::W, EventName::KEY_W_PRESSED},
    {sf::Keyboard::X, EventName::KEY_X_PRESSED},
    {sf::Keyboard::Y, EventName::KEY_Y_PRESSED},
    {sf::Keyboard::Z, EventName::KEY_Z_PRESSED},

    {sf::Keyboard::Num0, EventName::KEY_0_PRESSED},
    {sf::Keyboard::Num1, EventName::KEY_1_PRESSED},
    {sf::Keyboard::Num2, EventName::KEY_2_PRESSED},
    {sf::Keyboard::Num3, EventName::KEY_3_PRESSED},
    {sf::Keyboard::Num4, EventName::KEY_4_PRESSED},
    {sf::Keyboard::Num5, EventName::KEY_5_PRESSED},
    {sf::Keyboard::Num6, EventName::KEY_6_PRESSED},
    {sf::Keyboard::Num7, EventName::KEY_7_PRESSED},
    {sf::Keyboard::Num8, EventName::KEY_8_PRESSED},
    {sf::Keyboard::Num9, EventName::KEY_9_PRESSED},
    {sf::Keyboard::Escape, EventName::KEY_ESCAPE_PRESSED},
    {sf::Keyboard::LControl, EventName::KEY_LCTRL_PRESSED},
    {sf::Keyboard::LShift, EventName::KEY_LSHIFT_PRESSED},
    {sf::Keyboard::LAlt, EventName::KEY_LALT_PRESSED},
    {sf::Keyboard::LSystem, EventName::KEY_LSYSTEM_PRESSED},
    {sf::Keyboard::RControl, EventName::KEY_RCTRL_PRESSED},
    {sf::Keyboard::RShift, EventName::KEY_RSHIFT_PRESSED},
    {sf::Keyboard::RAlt, EventName::KEY_RALT_PRESSED},
    {sf::Keyboard::RSystem, EventName::KEY_RSYSTEM_PRESSED},
    {sf::Keyboard::Menu, EventName::KEY_MENU_PRESSED},
    {sf::Keyboard::LBracket, EventName::KEY_LBRACKET_PRESSED},
    {sf::Keyboard::RBracket, EventName::KEY_RBRACKET_PRESSED},
    {sf::Keyboard::Semicolon, EventName::KEY_SEMICOLON_PRESSED},
    {sf::Keyboard::Comma, EventName::KEY_COMMA_PRESSED},
    {sf::Keyboard::Period, EventName::KEY_PERIOD_PRESSED},
    {sf::Keyboard::Apostrophe, EventName::KEY_APOSTROPHE_PRESSED},
    {sf::Keyboard::Slash, EventName::KEY_SLASH_PRESSED},
    {sf::Keyboard::Backslash, EventName::KEY_BACKSLASH_PRESSED},
    {sf::Keyboard::Grave, EventName::KEY_GRAVE_PRESSED},
    {sf::Keyboard::Equal, EventName::KEY_EQUAL_PRESSED},
    {sf::Keyboard::Hyphen, EventName::KEY_HYPHEN_PRESSED},
    {sf::Keyboard::Space, EventName::KEY_SPACE_PRESSED},
    {sf::Keyboard::Enter, EventName::KEY_ENTER_PRESSED},
    {sf::Keyboard::Backspace, EventName::KEY_BACKSPACE_PRESSED},
    {sf::Keyboard::Tab, EventName::KEY_TAB_PRESSED},
    {sf::Keyboard::PageUp, EventName::KEY_PAGE_UP_PRESSED},
    {sf::Keyboard::PageDown, EventName::KEY_PAGE_DOWN_PRESSED},
    {sf::Keyboard::End, EventName::KEY_END_PRESSED},
    {sf::Keyboard::Home, EventName::KEY_HOME_PRESSED},
    {sf::Keyboard::Insert, EventName::KEY_INSERT_PRESSED},
    {sf::Keyboard::Delete, EventName::KEY_DELETE_PRESSED},
    {sf::Keyboard::Add, EventName::KEY_ADD_PRESSED},
    {sf::Keyboard::Subtract, EventName::KEY_SUBTRACT_PRESSED},
    {sf::Keyboard::Multiply, EventName::KEY_MULTIPLY_PRESSED},
    {sf::Keyboard::Divide, EventName::KEY_DIVIDE_PRESSED},
    {sf::Keyboard::Left, EventName::KEY_ARROW_LEFT_PRESSED},
    {sf::Keyboard::Right, EventName::KEY_ARROW_RIGHT_PRESSED},
    {sf::Keyboard::Up, EventName::KEY_ARROW_UP_PRESSED},
    {sf::Keyboard::Down, EventName::KEY_ARROW_DOWN_PRESSED},
    {sf::Keyboard::Numpad0, EventName::KEY_NUMPAD_0_PRESSED},
    {sf::Keyboard::Numpad1, EventName::KEY_NUMPAD_1_PRESSED},
    {sf::Keyboard::Numpad2, EventName::KEY_NUMPAD_2_PRESSED},
    {sf::Keyboard::Numpad3, EventName::KEY_NUMPAD_3_PRESSED},
    {sf::Keyboard::Numpad4, EventName::KEY_NUMPAD_4_PRESSED},
    {sf::Keyboard::Numpad5, EventName::KEY_NUMPAD_5_PRESSED},
    {sf::Keyboard::Numpad6, EventName::KEY_NUMPAD_6_PRESSED},
    {sf::Keyboard::Numpad7, EventName::KEY_NUMPAD_7_PRESSED},
    {sf::Keyboard::Numpad8, EventName::KEY_NUMPAD_8_PRESSED},
    {sf::Keyboard::Numpad9, EventName::KEY_NUMPAD_9_PRESSED},
    {sf::Keyboard::F1, EventName::KEY_F1_PRESSED},
    {sf::Keyboard::F2, EventName::KEY_F2_PRESSED},
    {sf::Keyboard::F3, EventName::KEY_F3_PRESSED},
    {sf::Keyboard::F4, EventName::KEY_F4_PRESSED},
    {sf::Keyboard::F5, EventName::KEY_F5_PRESSED},
    {sf::Keyboard::F6, EventName::KEY_F6_PRESSED},
    {sf::Keyboard::F7, EventName::KEY_F7_PRESSED},
    {sf::Keyboard::F8, EventName::KEY_F8_PRESSED},
    {sf::Keyboard::F9, EventName::KEY_F9_PRESSED},
    {sf::Keyboard::F10, EventName::KEY_F10_PRESSED},
    {sf::Keyboard::F11, EventName::KEY_F11_PRESSED},
    {sf::Keyboard::F12, EventName::KEY_F12_PRESSED},
    {sf::Keyboard::F13, EventName::KEY_F13_PRESSED},
    {sf::Keyboard::F14, EventName::KEY_F14_PRESSED},
    {sf::Keyboard::F15, EventName::KEY_F15_PRESSED},
    {sf::Keyboard::Pause, EventName::KEY_PAUSE_PRESSED}
    // Add other keys as needed
};

std::unordered_map<sf::Keyboard::Key, EventName> EventsManager::SFMLKeyReleasedTranslationMap = {
    {sf::Keyboard::A, EventName::KEY_A_RELEASED},
    {sf::Keyboard::B, EventName::KEY_B_RELEASED},
    {sf::Keyboard::C, EventName::KEY_C_RELEASED},
    {sf::Keyboard::D, EventName::KEY_D_RELEASED},
    {sf::Keyboard::E, EventName::KEY_E_RELEASED},
    {sf::Keyboard::F, EventName::KEY_F_RELEASED},
    {sf::Keyboard::G, EventName::KEY_G_RELEASED},
    {sf::Keyboard::H, EventName::KEY_H_RELEASED},
    {sf::Keyboard::I, EventName::KEY_I_RELEASED},
    {sf::Keyboard::J, EventName::KEY_J_RELEASED},
    {sf::Keyboard::K, EventName::KEY_K_RELEASED},
    {sf::Keyboard::L, EventName::KEY_L_RELEASED},
    {sf::Keyboard::M, EventName::KEY_M_RELEASED},
    {sf::Keyboard::N, EventName::KEY_N_RELEASED},
    {sf::Keyboard::O, EventName::KEY_O_RELEASED},
    {sf::Keyboard::P, EventName::KEY_P_RELEASED},
    {sf::Keyboard::Q, EventName::KEY_Q_RELEASED},
    {sf::Keyboard::R, EventName::KEY_R_RELEASED},
    {sf::Keyboard::S, EventName::KEY_S_RELEASED},
    {sf::Keyboard::T, EventName::KEY_T_RELEASED},
    {sf::Keyboard::U, EventName::KEY_U_RELEASED},
    {sf::Keyboard::V, EventName::KEY_V_RELEASED},
    {sf::Keyboard::W, EventName::KEY_W_RELEASED},
    {sf::Keyboard::X, EventName::KEY_X_RELEASED},
    {sf::Keyboard::Y, EventName::KEY_Y_RELEASED},
    {sf::Keyboard::Z, EventName::KEY_Z_RELEASED},

    {sf::Keyboard::Num0, EventName::KEY_0_RELEASED},
    {sf::Keyboard::Num1, EventName::KEY_1_RELEASED},
    {sf::Keyboard::Num2, EventName::KEY_2_RELEASED},
    {sf::Keyboard::Num3, EventName::KEY_3_RELEASED},
    {sf::Keyboard::Num4, EventName::KEY_4_RELEASED},
    {sf::Keyboard::Num5, EventName::KEY_5_RELEASED},
    {sf::Keyboard::Num6, EventName::KEY_6_RELEASED},
    {sf::Keyboard::Num7, EventName::KEY_7_RELEASED},
    {sf::Keyboard::Num8, EventName::KEY_8_RELEASED},
    {sf::Keyboard::Num9, EventName::KEY_9_RELEASED},
    {sf::Keyboard::Escape, EventName::KEY_ESCAPE_RELEASED},
    {sf::Keyboard::LControl, EventName::KEY_LCTRL_RELEASED},
    {sf::Keyboard::LShift, EventName::KEY_LSHIFT_RELEASED},
    {sf::Keyboard::LAlt, EventName::KEY_LALT_RELEASED},
    {sf::Keyboard::LSystem, EventName::KEY_LSYSTEM_RELEASED},
    {sf::Keyboard::RControl, EventName::KEY_RCTRL_RELEASED},
    {sf::Keyboard::RShift, EventName::KEY_RSHIFT_RELEASED},
    {sf::Keyboard::RAlt, EventName::KEY_RALT_RELEASED},
    {sf::Keyboard::RSystem, EventName::KEY_RSYSTEM_RELEASED},
    {sf::Keyboard::Menu, EventName::KEY_MENU_RELEASED},
    {sf::Keyboard::LBracket, EventName::KEY_LBRACKET_RELEASED},
    {sf::Keyboard::RBracket, EventName::KEY_RBRACKET_RELEASED},
    {sf::Keyboard::Semicolon, EventName::KEY_SEMICOLON_RELEASED},
    {sf::Keyboard::Comma, EventName::KEY_COMMA_RELEASED},
    {sf::Keyboard::Period, EventName::KEY_PERIOD_RELEASED},
    {sf::Keyboard::Apostrophe, EventName::KEY_APOSTROPHE_RELEASED},
    {sf::Keyboard::Slash, EventName::KEY_SLASH_RELEASED},
    {sf::Keyboard::Backslash, EventName::KEY_BACKSLASH_RELEASED},
    {sf::Keyboard::Grave, EventName::KEY_GRAVE_RELEASED},
    {sf::Keyboard::Equal, EventName::KEY_EQUAL_RELEASED},
    {sf::Keyboard::Hyphen, EventName::KEY_HYPHEN_RELEASED},
    {sf::Keyboard::Space, EventName::KEY_SPACE_RELEASED},
    {sf::Keyboard::Enter, EventName::KEY_ENTER_RELEASED},
    {sf::Keyboard::Backspace, EventName::KEY_BACKSPACE_RELEASED},
    {sf::Keyboard::Tab, EventName::KEY_TAB_RELEASED},
    {sf::Keyboard::PageUp, EventName::KEY_PAGE_UP_RELEASED},
    {sf::Keyboard::PageDown, EventName::KEY_PAGE_DOWN_RELEASED},
    {sf::Keyboard::End, EventName::KEY_END_RELEASED},
    {sf::Keyboard::Home, EventName::KEY_HOME_RELEASED},
    {sf::Keyboard::Insert, EventName::KEY_INSERT_RELEASED},
    {sf::Keyboard::Delete, EventName::KEY_DELETE_RELEASED},
    {sf::Keyboard::Add, EventName::KEY_ADD_RELEASED},
    {sf::Keyboard::Subtract, EventName::KEY_SUBTRACT_RELEASED},
    {sf::Keyboard::Multiply, EventName::KEY_MULTIPLY_RELEASED},
    {sf::Keyboard::Divide, EventName::KEY_DIVIDE_RELEASED},
    {sf::Keyboard::Left, EventName::KEY_ARROW_LEFT_RELEASED},
    {sf::Keyboard::Right, EventName::KEY_ARROW_RIGHT_RELEASED},
    {sf::Keyboard::Up, EventName::KEY_ARROW_UP_RELEASED},
    {sf::Keyboard::Down, EventName::KEY_ARROW_DOWN_RELEASED},
    {sf::Keyboard::Numpad0, EventName::KEY_NUMPAD_0_RELEASED},
    {sf::Keyboard::Numpad1, EventName::KEY_NUMPAD_1_RELEASED},
    {sf::Keyboard::Numpad2, EventName::KEY_NUMPAD_2_RELEASED},
    {sf::Keyboard::Numpad3, EventName::KEY_NUMPAD_3_RELEASED},
    {sf::Keyboard::Numpad4, EventName::KEY_NUMPAD_4_RELEASED},
    {sf::Keyboard::Numpad5, EventName::KEY_NUMPAD_5_RELEASED},
    {sf::Keyboard::Numpad6, EventName::KEY_NUMPAD_6_RELEASED},
    {sf::Keyboard::Numpad7, EventName::KEY_NUMPAD_7_RELEASED},
    {sf::Keyboard::Numpad8, EventName::KEY_NUMPAD_8_RELEASED},
    {sf::Keyboard::Numpad9, EventName::KEY_NUMPAD_9_RELEASED},
    {sf::Keyboard::F1, EventName::KEY_F1_RELEASED},
    {sf::Keyboard::F2, EventName::KEY_F2_RELEASED},
    {sf::Keyboard::F3, EventName::KEY_F3_RELEASED},
    {sf::Keyboard::F4, EventName::KEY_F4_RELEASED},
    {sf::Keyboard::F5, EventName::KEY_F5_RELEASED},
    {sf::Keyboard::F6, EventName::KEY_F6_RELEASED},
    {sf::Keyboard::F7, EventName::KEY_F7_RELEASED},
    {sf::Keyboard::F8, EventName::KEY_F8_RELEASED},
    {sf::Keyboard::F9, EventName::KEY_F9_RELEASED},
    {sf::Keyboard::F10, EventName::KEY_F10_RELEASED},
    {sf::Keyboard::F11, EventName::KEY_F11_RELEASED},
    {sf::Keyboard::F12, EventName::KEY_F12_RELEASED},
    {sf::Keyboard::F13, EventName::KEY_F13_RELEASED},
    {sf::Keyboard::F14, EventName::KEY_F14_RELEASED},
    {sf::Keyboard::F15, EventName::KEY_F15_RELEASED},
    {sf::Keyboard::Pause, EventName::KEY_PAUSE_RELEASED}
    // Add other keys as needed
};


std::unordered_map<sf::Mouse::Button, EventName> EventsManager::SFMLMouseButtonPressedTranslationMap = {
    {sf::Mouse::Left, EventName::MOUSE_LEFT_PRESSED},
    {sf::Mouse::Right, EventName::MOUSE_RIGHT_PRESSED},
    {sf::Mouse::Middle, EventName::MOUSE_MIDDLE_PRESSED}
};

std::unordered_map<sf::Mouse::Button, EventName> EventsManager::SFMLMouseButtonReleasedTranslationMap = {
    {sf::Mouse::Left, EventName::MOUSE_LEFT_RELEASED},
    {sf::Mouse::Right, EventName::MOUSE_RIGHT_RELEASED},
    {sf::Mouse::Middle, EventName::MOUSE_MIDDLE_RELEASED}
};
