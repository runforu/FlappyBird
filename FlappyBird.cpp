#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "Environment.h"
#include "FiniteStateMachine.h"
#include "Loger.h"

int main(int argc, char **argv) {
    { FSM::FiniteStateMachine::Instance().Start(); }
    _CrtDumpMemoryLeaks();
    return 0;
}
