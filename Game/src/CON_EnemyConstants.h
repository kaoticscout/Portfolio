#ifndef ENEMY_CONSTANTS_H
#define ENEMY_CONSTANTS_H

const Ogre::String DEFAULT_ENEMY_MESH  = "robot.mesh";
const Ogre::String DEFAULT_ENEMY_TEXTURE = "Rockwall";

const float MINION_MOVE_SPEED = 0.2f;
const float ENEMY_CLOSE_DISTANCE = 10.0f;

const enum minionStates
{
	PURSUE = 1,
	WANDER = 2,
	ATTACK = 3,
	DEAD = 4
};


const enum ENEMY_STATE
{
	ENEMY_IDLE,
	ENEMY_MOVING,
	ENEMY_ATTACKING,
	ENEMY_SEEKING
};

#endif 