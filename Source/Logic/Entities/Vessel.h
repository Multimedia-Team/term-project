/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: VesselClass.cpp
--
-- PROGRAM: 4981 game project
--
-- FUNCTIONS:
--     Vessel( job_class jobclass, GameMap gmap, int x, int y );
--     ~Vessel();
--
--     void setPosition( int x, int y );
--
--     void resetEXP();
--     void increaseEXP( int exp );
--     int  getEXP();
--     int  getNextLevelEXP();
--
--     int  getLevel();
--     void increaseLevel();
--
--     void resetHP();
--     void increaseHP( int hp );
--     void decreaseHP( int hp );
--     int  getHP();
--     int  getMaxHP();
--
--     void resetAttackPower();
--     void attackPowerUp( int attackpower );
--     void attackPowerDown( int attackpower );
--     int  getAttackPower();
--     int  getDefaultAttackPower();
--
--     void resetSpeed();
--     void speedUp( int speed );
--     void speedDown( int speed );
--     int  getSpeed();
--     int  getDefaultSpeed();
--
--     bool checkDeath();
--     void die();
--
--     void move( int direction );
--
--     void normalAttack( int x, int y );
--     void useAbility( int abilityNum );
--
-- DATE: February 15, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Sanders Lee
--
-- PROGRAMMER: Sanders Lee
--
-- NOTES:
-- This class file provides the functions needed to set up and modify Vessel status.
-- It also provides the functions to control a Vessel (movement and attack).
----------------------------------------------------------------------------------------------------------------------*/
#ifndef VESSEL_H
#define VESSEL_H

#include <SFML/Graphics.hpp>
#include "../../Engine/Map.h"
#include "../../Engine/VEntity.h"
#include "../../Engine/Cell.h"
#include "../../Engine/Controller.h"
#include "../Creature.h"
#include "../../Engine/TileManager.h"

#define MAX_LEVEL 10;

typedef char Weapon;
typedef char Ability;

typedef enum job_class { WARRIOR, SHAMAN, HUNTER, SCOUT, TEGUH } job_class;

class Vessel : public Marx::VEntity, public Creature
{
	protected:
		job_class jobClass;
		int currentHealth;
		int maxHealth;
		int currentEXP;
		int nextLevelEXP;
		int currentLevel;
		int defaultSpeed;
		int travelSpeed;
		int attackPower;
		float xSpeed;
		float ySpeed;
		float xPos;
		float yPos;
		float myX;
		float myY;
		float servX;
		float servY;
		int direction;	//0 = right, 1 = left //why not a bool?
		bool movingLeft;
	  bool movingRight;
		bool movingUp;
	  bool movingDown;
		Ability* abilities;	//3 abilities for each Vessel
		SGO &mask_sprite;
		SGO &weapon_sprite;
		id_resource grassWalkSound, stoneWalkSound, hurtSound, attackSound;
		//TO DO: pointer to the game map needed in the future

	public:
        float newXSpeed;
        float newYSpeed;
		Vessel( SGO &_sprite, SGO &_mask, SGO &_weapon,
						Marx::Map * gmap,
						float x,
						float y,
						Marx::Controller* controller,
						float height,
						float width
						/*, job_class jobClass, Ability* abilityList*/ );
		//inherited methods
		virtual ~Vessel();
		virtual void onUpdate(float);
		//virtual void draw(Renderer& renderer, sf::RenderStates states) const override;

		void setPosition( float x, float y );
		float getXPosition();
		float getYPosition();

		int getXSpeed();
		int getYSpeed();
		bool isMoving();
		int getDirection();

		void resetEXP();
		void increaseEXP( int exp );
		int  getEXP();
		int  getNextLevelEXP();
		int  getLevel();
		void increaseLevel();

		job_class getJobClass();

		void resetHP();
		void increaseHP( int hp );
		void decreaseHP( int hp );
		int  getHP();
		int  getMaxHP();

		void resetAttackPower();
		void attackPowerUp( int attackpower );
		void attackPowerDown( int attackpower );
		int  getAttackPower();
		int  getDefaultAttackPower();

		void resetSpeed();
		void speedUp( int speed );
		void speedDown( int speed );
		int  getSpeed();
		int  getDefaultSpeed();

		bool checkDeath();
		void die();

		void move();
		void stop(int key);

		void normalAttack( int x, int y );
		void useAbility( int abilityNum, int x, int y );

		virtual void setHealth(int health);
		virtual void setAttack(int attack);
		virtual Entity *getEntity();
};

#endif
