#include "keymaps.hpp"

auto
initControls() -> std::pair<MomentaryActionsMap, StateMap>
{
    using namespace Input;

    auto momentaryMap = MomentaryActionsMap();
    momentaryMap.add({Key::C}, Trigger::ToggleAutoWalk);
    momentaryMap.add({Key::O}, Trigger::ToggleAutoZoom);
    momentaryMap.add({Key::K}, Trigger::IncreaseIterations);
    momentaryMap.add({Key::J}, Trigger::DecreaseIterations);
    momentaryMap.add({Key::F}, Trigger::ToggleFastMode);
    momentaryMap.add({Key::H}, Trigger::SwitchShader);
    momentaryMap.add({Key::P}, Trigger::TogglePause);
    momentaryMap.add({Key::X}, Trigger::TakeScreenshot);
    momentaryMap.add({Key::Q}, Trigger::CloseWindow);
    momentaryMap.add({Key::Escape}, Trigger::CloseWindow);
    momentaryMap.add({Key::S, (int)Mod::Control}, Trigger::Save);
    momentaryMap.add({Key::O, (int)Mod::Control}, Trigger::Load);
    momentaryMap.add(
            {Key::L, (int)Mod::Control},
            Trigger::LoadTerrainFunctions);

    auto stateMap = StateMap();
    stateMap.add({Key::W}, State::MovingForwards);
    stateMap.add({Key::S}, State::MovingBackwards);
    stateMap.add({Key::A}, State::MovingLeft);
    stateMap.add({Key::D}, State::MovingRight);
    stateMap.add({Key::W, (int)Mod::Shift}, State::RunningForwards);
    stateMap.add({Key::S, (int)Mod::Shift}, State::RunningBackwards);
    stateMap.add({Key::A, (int)Mod::Shift}, State::RunningLeft);
    stateMap.add({Key::D, (int)Mod::Shift}, State::RunningRight);
    stateMap.add({MouseButton::Left}, State::ZoomingIn);
    stateMap.add({MouseButton::Right}, State::ZoomingOut);
    stateMap.add({Key::Up}, State::IncreasingParameter);
    stateMap.add({Key::Down}, State::DecreasingParameter);
    stateMap.add({Key::Key1}, State::ChangingFrequency);
    stateMap.add({Key::Key2}, State::ChangingTotalOffset);
    stateMap.add({Key::Key3}, State::ChangingGreenOffset);
    stateMap.add({Key::Key4}, State::ChangingBlueOffset);
    stateMap.add({Key::Key5}, State::ChangingYScale);

    return {momentaryMap, stateMap};
}

auto
initControlsDvorak() -> std::pair<MomentaryActionsMap, StateMap>
{
    using namespace Input;

    auto momentaryMap = MomentaryActionsMap();
    momentaryMap.add({Key::J}, Trigger::ToggleAutoWalk);
    momentaryMap.add({Key::R}, Trigger::ToggleAutoZoom);
    momentaryMap.add({Key::T}, Trigger::IncreaseIterations);
    momentaryMap.add({Key::H}, Trigger::DecreaseIterations);
    momentaryMap.add({Key::F}, Trigger::ToggleFastMode);
    momentaryMap.add({Key::D}, Trigger::SwitchShader);
    momentaryMap.add({Key::P}, Trigger::TogglePause);
    momentaryMap.add({Key::X}, Trigger::TakeScreenshot);
    momentaryMap.add({Key::Q}, Trigger::CloseWindow);
    momentaryMap.add({Key::Escape}, Trigger::CloseWindow);
    momentaryMap.add({Key::S, (int)Mod::Control}, Trigger::Save);
    momentaryMap.add({Key::O, (int)Mod::Control}, Trigger::Load);
    momentaryMap.add(
            {Key::L, (int)Mod::Control},
            Trigger::LoadTerrainFunctions);
    momentaryMap.add({Key::L}, Trigger::ToggleLighting);

    auto stateMap = StateMap();
    stateMap.add({Key::Comma}, State::MovingForwards);
    stateMap.add({Key::O}, State::MovingBackwards);
    stateMap.add({Key::A}, State::MovingLeft);
    stateMap.add({Key::E}, State::MovingRight);
    stateMap.add({Key::Comma, (int)Mod::Shift}, State::RunningForwards);
    stateMap.add({Key::O, (int)Mod::Shift}, State::RunningBackwards);
    stateMap.add({Key::A, (int)Mod::Shift}, State::RunningLeft);
    stateMap.add({Key::E, (int)Mod::Shift}, State::RunningRight);
    stateMap.add({MouseButton::Left}, State::ZoomingIn);
    stateMap.add({MouseButton::Right}, State::ZoomingOut);
    stateMap.add({Key::Up}, State::IncreasingParameter);
    stateMap.add({Key::Down}, State::DecreasingParameter);
    stateMap.add({Key::Key1}, State::ChangingFrequency);
    stateMap.add({Key::Key2}, State::ChangingTotalOffset);
    stateMap.add({Key::Key3}, State::ChangingGreenOffset);
    stateMap.add({Key::Key4}, State::ChangingBlueOffset);
    stateMap.add({Key::Key5}, State::ChangingYScale);

    return {momentaryMap, stateMap};
}
