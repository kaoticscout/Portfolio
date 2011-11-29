#ifndef CON_HUT_H
#define CON_HUT_H

const int DEFAULT_SPAWN_RADIUS = 25;
const int DEFAULT_AGGRO_RADIUS  = 20;
const int DEFAULT_TIME_BETWEEN_SPAWNS = 2;
const int DEFAULT_GUARDIAN_SPAWN_AMOUNT = 3;
const int MAX_RAND_SPAWN_TIME_DIFF = 10;

const enum HUT_STATE
{
	HUT_IDLE,
	HUT_SPAWNING_MINION,
	HUT_DESTROYED
};


#endif