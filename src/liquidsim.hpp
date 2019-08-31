/* Liquid Simulation */

#ifndef LIQUIDSIM_H
#define LIQUIDSIM_H

#include <unordered_map>
#include <vector>

#include <Godot.hpp>
#include <Node2D.hpp>
#include <Vector2.hpp>
#include <TileMap.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>

#include <cell.hpp>

namespace godot {
class LiquidSim : public Node2D {
	GODOT_CLASS(LiquidSim, Node2D)

	private:
		int iteration;
		float time_passed;
		float refresh_rate;
		float max_liquid_value;
		float min_liquid_value;
		float max_liquid_compression;
		float max_flow_value;
		float min_flow_value;
		float flow_speed;

		float total_liquid_amount;

		TileMap *world;
		std::vector<Cell *> cells;
		std::unordered_map<int, Cell *> cells_positions;

		Ref<PackedScene> liquid_scene;
		int hash_cell_position(int x, int y);
		Cell *get_cell_by_position(int x, int y);
		Cell *create_cell(int x, int y, float amount);
		float get_vertical_flow_value(float src_liquid_amount, float dst_liquid_amount);
		void refresh_cell_sprite(Cell *cell);
		void refresh_all();

	public:
		bool started = false;
		static void _register_methods();

		LiquidSim();
		~LiquidSim();

		void _init(); // our initializer called by Godot

		void _ready();
		void _process(float delta);

		void start();
		void stop();
		void clear();
		void update_simulation();
		int cleanup_empty_cells(int limit);
		float add_liquid(int x, int y, float amount);
		void del_liquid(int x, int y);
		void set_liquid(int x, int y, float amount);
		float get_liquid(int x, int y);
};

}

#endif