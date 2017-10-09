#ifndef INCLUDE_GAMESAVE_H
#define INCLUDE_GAMESAVE_H

#include <fstream>
#include "Player.h"
#include "Enemy.h"

class GameSave {

	public:
		GameSave();

		void setSlot ( int saveSelection );
		void createSave ();
		void saveLevel ( unsigned int level_, Player* player, std::vector < Enemy * > enemies, 
			unsigned int SLOT);

		int get_saved_level ( int continueSelection_ );
		void get_player_position ( double &player_x, double &player_y, const int SLOT );

		bool is_enemy_dead ( const int NUMBER_ENEMY, const int SLOT );

		bool is_saved ( const int SAVE_SLOT );

		std::string filePath;
		int saveSelection;
		unsigned int CURRENT_LEVEL;

		std::ofstream saveFile;
		std::ifstream continueFile;

		enum Selection : uint8_t 
		{
			SLOT_1 = 0,
			SLOT_2,
			SLOT_3,
			TOTAL
		};

};

#endif //INCLUDE_GAMESAVE_H