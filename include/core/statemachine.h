#pragma once

#include "utilities/types.h"

namespace nemesis
{
    template<typename _DerType, typename StateType, typename Rtn = void, typename... Args>
    struct StateMachine
    {
    private:
        StateType current_state;

    protected:
        using StateFunc = Rtn (_DerType::*)(Args...);
        UMap<StateType, StateFunc>* trans_map_ptr;
        
    public:
        StateMachine() = default;

        StateMachine(StateType start_state)
            : current_state(start_state)
        {
        }

        virtual ~StateMachine() = 0;

        void GoTo(StateType state)
        {
            current_state = state;
        }

        StateType GetState()
        {
            return current_state;
        }

        Rtn Execute(Args&&... args)
        {
            StateFunc& func = trans_map_ptr->at(current_state);
            _DerType* derobj = static_cast<_DerType*>(this);

            if constexpr (!std::is_same_v<void, Rtn>)
            {
                return (derobj->*func)(std::forward<Args>(args)...);
            }

            (derobj->*func)(std::forward<Args>(args)...);
        }

        static_assert(std::is_enum_v<StateType>);
    };

    template<typename _DerType, typename StateType, typename Rtn, typename... Args>
    StateMachine<_DerType, StateType, Rtn, Args...>::~StateMachine()
    {
    }
}
