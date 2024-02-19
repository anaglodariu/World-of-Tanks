#pragma once

#include <random>
#include <map>
#include <string>

namespace stateMachine
{
    enum class MovementState
    {
        GoingForward,
        GoingBackward,
        InPlaceRotationLeft,
        InPlaceRotationRight
    };

    class StateMachine
    {
    public:
        StateMachine() {
			currentState = MovementState::GoingForward;
        }

        int GetRandomNumberInRange(const int minInclusive, const int maxInclusive) {
            static std::random_device randomDevice;
            static std::default_random_engine randomEngine(randomDevice());

            std::uniform_int_distribution<int> uniformDist(minInclusive, maxInclusive);

            return uniformDist(randomEngine);
        }


        MovementState GetNextMovementState(const MovementState currentState)
        {
            int randomChange = GetRandomNumberInRange(0, 1);
            MovementState nextState = currentState;

            switch (currentState)
            {
            case MovementState::GoingForward:
            case MovementState::GoingBackward:
                nextState = (randomChange == 1)
                    ? MovementState::InPlaceRotationLeft
                    : MovementState::InPlaceRotationRight;
                break;

            case MovementState::InPlaceRotationLeft:
            case MovementState::InPlaceRotationRight:
                nextState = (randomChange == 1)
                    ? MovementState::GoingForward
                    : MovementState::GoingBackward;
                break;

            default:
                break;
            }

            return nextState;
        }
    public:
		MovementState currentState;
        float TimeInCurrentState = 0.0f;
    };

}