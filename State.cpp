#include "State.h"

State::State()
{

}

bool State::requestingMusic()
{
    bool musicReturn = musicRequest;
    musicRequest = false;
    return musicReturn;
}

