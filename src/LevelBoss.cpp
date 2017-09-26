#include "LevelBoss.h"
#include "Game.h"
#include "LuaScript.h"
#include "Logger.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "TileMap.h"
#include "Collision.h"

LevelBoss::LevelBoss() :
	Level()
{

}

LevelBoss::~LevelBoss(){

}

void LevelBoss::load(){
	Log(DEBUG) << "Loading level boss...";

	Game::instance().get_audio_handler().change_music("res/audio/Tema_Boss_02.wav");

	// Loading the tile/tilemap.
	this->tile_map = new TileMap("res/maps/levelBoss.tmx");

	// Setting the level width/height.
	this->width = this->tile_map->get_map_width();
	this->height = this->tile_map->get_map_height();
	SDL_Rect bounds = {0, 0, (int)this->width, (int)this->height};
	this->quadTree = new QuadTree(0, bounds);

	this->background = Game::instance().get_resources().get("res/images/lv1_background.png");

	// Getting information from lua script.
	LuaScript luaLevel1("lua/Level1.lua");
	const std::string PATH_PLAYER_SPRITE_SHEET = luaLevel1.unlua_get<std::string>(
		"level.player.spriteSheet");
	const std::string PATH_BACKGROUND_AUDIO = luaLevel1.unlua_get<std::string>(
		"level.audio.background");
	const std::string PATH_ENEMY = luaLevel1.unlua_get<std::string>("level.enemy");

	// Changing the music.
	Game::instance().get_audio_handler().change_music(PATH_BACKGROUND_AUDIO);

	// Loading the player and the camera.
	Player* level_player = new Player(this->tile_map->get_initial_x(), this->tile_map->get_initial_y(), PATH_PLAYER_SPRITE_SHEET);
	Camera* level_camera = new Camera(level_player); 

	this->player_Hud = new PlayerHUD(level_player);

	Boss* lBoss = new Boss(1200, 684.0, "res/images/boss_sheet.png", level_player);
	lBoss->getAnimation()->changeAnimation(0,0,1,false,0.0);
	
	// Test text.
	// Text* text = new Text(200.0, 900.0, "res/fonts/KGFeeling22.ttf", 50, "dauphine");
	// addEntity(text);

	// Finally, setting the player, the boss and the camera.
	set_player(level_player);
	setBoss(lBoss);
	set_camera(level_camera);

	Game::instance().get_fade().fade_out(0, 0.002);
}

void LevelBoss::unload(){
	Log(DEBUG) << "\tUnloading level boss...";

	clean_entities();
	clear_enemies();
	clear_documents();
}

void LevelBoss::update(const double DELTA_TIME){

	// Populating the QuadTree.
	this->quadTree->set_objects(this->tile_map->get_collision_rects());

	// Updating the entities, using the QuadTree.
	std::vector<CollisionRect> return_objects;
	for (auto entity : this->entities) {
		return_objects.clear();
		this->quadTree->retrieve(return_objects, entity->get_bounding_box());
		entity->set_collision_rects(return_objects);
		entity->update(DELTA_TIME);
	}

	// Set to GameOver if the player is dead.
	if(this->player->is_dead()){
		Game::instance().set_state(Game::GStates::GAMEOVER);
		return;
	}

	// Updating the potions.
	for(auto potion : this->player->potions){
		return_objects.clear();
		this->quadTree->retrieve(return_objects, potion->get_bounding_box());
		potion->set_collision_rects(return_objects);
	}

	if(this->boss->x < this->player->x + 10 && this->boss->x > this->player->x - 10){
		this->boss->saw_player = true;
	}

	// Updating the HUD.
	this->player_Hud->update();

	// Updating the boss.
	this->boss->update(DELTA_TIME);

	// Updating the camera.
	this->camera->update();

	if(this->boss->life <= 0){
		this->player->reached_level_end = true;
	}

	// Set next level if end is reached.
	if(this->player->reached_level_end){
		Game::instance().set_state(Game::GStates::VICTORY);
		return;
	}

	// Updating the potion/boss collision.
	for(auto potion : this->player->potions){
		if(Collision::rects_collided(potion->get_bounding_box(), this->boss->get_bounding_box())){
			if(potion->activated){
				this->boss->life--;
				potion->activated = false;
			}
		}
	}

	// Updating the player attack/boss collision.
	if(Collision::rects_collided(this->player->get_bounding_box(), this->boss->get_bounding_box())){
		if(this->player->is_current_state(Player::player_states::ATTACK) || this->player->is_current_state(Player::player_states::ATTACKMOVING)
			|| this->player->is_current_state(Player::player_states::ATTACKJUMPING)){
			if(this->boss->has_shield && this->player->can_attack){
				this->boss->has_shield = false;
				this->player->can_attack = false;
			}
			else if(this->player->can_attack){
				this->boss->life -= 1;
				this->player->can_attack = false;
			}
		}
	}

	Log(DEBUG) << "Boss life: " << this->boss->life;

	// Documents check
	for(auto document : this->documents){
		if(Collision::rects_collided(this->player->get_bounding_box(), document->get_bounding_box())){
			document->should_render = true;
		}
		else {
			document->should_render = false;
		}
	}
}

void LevelBoss::render(){
	const int CAMERA_X = this->camera->getClip().x;
	const int CAMERA_Y = this->camera->getClip().y;

	this->background->render(0, 0);

	// Render the tiles in the TileMap.
	this->tile_map->render(CAMERA_X, CAMERA_Y);

	this->player_Hud->render();

	this->boss->render(CAMERA_X, CAMERA_Y);

	// Render all the entities in the list.
	for(auto entity : this->entities){
        entity->render(CAMERA_X, CAMERA_Y);
	}

	// Document text image
	for(auto document : this->documents){
		document->render(CAMERA_X, CAMERA_Y);
		if(document->should_render){
			document->render_document_text();
		}
	}
}

