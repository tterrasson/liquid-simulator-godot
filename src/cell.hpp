/* Cell */

#ifndef LIQUIDSIM_CELL_H
#define LIQUIDSIM_CELL_H

#include <Godot.hpp>
#include <Node2D.hpp>
#include <Sprite.hpp>
#include <Texture.hpp>

#include "utils.hpp"

class Cell {
  public:
    int uid;
    int x;
    int y;
    int sprite_height;
    float liquid_amount;
    float new_liquid_amount; // Used during computation
    godot::Node2D *node;
    godot::Sprite *sprite_node;
    bool dir_bottom;
    int iteration_since_last_change;

    Cell(godot::Node2D *node_ptr, int pos_x, int pos_y, float amount) {
        x = pos_x;
        y = pos_y;
        node = node_ptr;
        liquid_amount = amount;
        new_liquid_amount = amount;
        sprite_node =
            godot::Object::cast_to<godot::Sprite>(node->get_node("Sprite"));
        sprite_height = sprite_node->get_texture()->get_height();
        dir_bottom = false;
        uid = hash_position(pos_x, pos_y);
        iteration_since_last_change = 0;
    }
};

#endif