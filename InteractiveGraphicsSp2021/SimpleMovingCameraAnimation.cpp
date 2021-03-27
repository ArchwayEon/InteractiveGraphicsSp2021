#include "SimpleMovingCameraAnimation.h"
#include "ReferenceFrame.h"
#include "BaseCamera.h"
#include "AbstractInputSystem.h"

SimpleMovingCameraAnimation::SimpleMovingCameraAnimation()
{
   _state = SimpleMovingState::Not_Moving;
   _speed = 20.0f;
}

inline SimpleMovingState SimpleMovingCameraAnimation::GetState()
{
   return _state;
}

inline void SimpleMovingCameraAnimation::SetState(SimpleMovingState state)
{
   _state = state;
}

void SimpleMovingCameraAnimation::Update(double elapsedSeconds)
{
   _camera->SetupLookingForward();
   CheckInputSystem();
   ReferenceFrame& frame = _camera->frame;
   switch (_state) {
   case SimpleMovingState::Not_Moving:
      break;
   case SimpleMovingState::Moving_Forward: {
      glm::vec3 forward = -frame.GetZAxis(); // Forward is -ve Z
      glm::vec3 moveDelta = forward * (_speed * (float)elapsedSeconds);
      frame.TranslateWorld(moveDelta);
      _camera->UpdateView();
      break;
   }
   case SimpleMovingState::Moving_Backward:
      glm::vec3 backward = frame.GetZAxis(); 
      glm::vec3 moveDelta = backward * (_speed * (float)elapsedSeconds);
      frame.TranslateWorld(moveDelta);
      _camera->UpdateView();
      break;
      break;
   case SimpleMovingState::Moving_Left:
      break;
   case SimpleMovingState::Moving_Right:
      break;
   }
}

void SimpleMovingCameraAnimation::CheckInputSystem()
{
   _state = SimpleMovingState::Not_Moving;
   KeyState keyState = _inputSystem->GetKeyState("W");
   if (keyState == KeyState::Pressed) {
      _state = SimpleMovingState::Moving_Forward;
      return;
   }
   keyState = _inputSystem->GetKeyState("S");
   if (keyState == KeyState::Pressed) {
      _state = SimpleMovingState::Moving_Backward;
      return;
   }
}
