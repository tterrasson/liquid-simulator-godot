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
    float liquid_amount;
    float new_liquid_amount; // Used during computation
    godot::Node2D *node;
    godot::Sprite *sprite_node;
    bool dir_bottom;
    int iteration_since_last_change;

    Cell(godot::Node2D *node_ptr, int pos_x, int pos_y, float amount, float width, float height) {
        x = pos_x;
        y = pos_y;
        node = node_ptr;
        liquid_amount = amount;
        new_liquid_amount = amount;
        sprite_node =
            godot::Object::cast_to<godot::Sprite>(node->get_node("Sprite"));
        // get the width and height of the texture
        float texture_width = sprite_node->get_texture()->get_width();
        float texture_height = sprite_node->get_texture()->get_height();
        // we scale the texture to the size of the tilemap cells
        sprite_node->set_scale(godot::Vector2(width / texture_width, height / texture_height));
        // we position the sprite in the center of each cell of the tilemap
        sprite_node->set_position(godot::Vector2(width / 2.0f, height / 2.0f));
        dir_bottom = false;
        uid = hash_position(pos_x, pos_y);
        iteration_since_last_change = 0;
    }
};

#endif