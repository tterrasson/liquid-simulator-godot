#include "liquidsim.hpp"

#include "utils.hpp"

using namespace godot;

void LiquidSim::_register_methods() {
    register_method("_ready", &LiquidSim::_ready);
    register_method("_process", &LiquidSim::_process);
    register_method("start", &LiquidSim::start);
    register_method("stop", &LiquidSim::stop);
    register_method("clear", &LiquidSim::clear);
    register_method("update_simulation", &LiquidSim::update_simulation);
    register_method("cleanup_empty_cells", &LiquidSim::cleanup_empty_cells);
    register_method("add_liquid", &LiquidSim::add_liquid);
    register_method("del_liquid", &LiquidSim::del_liquid);
    register_method("set_liquid", &LiquidSim::set_liquid);
    register_method("get_liquid", &LiquidSim::get_liquid);
    register_property<LiquidSim, float>("flow_speed", &LiquidSim::flow_speed, 1.0f)
}

LiquidSim::LiquidSim() {}

LiquidSim::~LiquidSim() {}

void LiquidSim::_init() {
    time_passed = 0.0f;
    refresh_rate = 0.08f;
    max_liquid_value = 1.0f;
    min_liquid_value = 0.005f;
    max_liquid_compression = 0.25f;
    max_flow_value = 4.0f;
    min_flow_value = 0.005f;
    flow_speed = 1.0f;
    total_liquid_amount = 0.0;
}

void LiquidSim::_ready() {
    world = godot::Object::cast_to<godot::TileMap>(get_parent());
    liquid_scene = ResourceLoader::get_singleton()->load(
        "res://addons/liquidsim/LiquidNode.tscn");
    start();
}

void LiquidSim::_process(float delta) {
    if (!started) {
        return;
    }

    time_passed += delta;

    if (time_passed >= refresh_rate) {
        update_simulation();
        update_simulation();
        update_simulation();
        update_simulation();
        refresh_all();

        iteration += 1;
        time_passed = 0.0;

        // Cleanup empty cells every 100 iterations
        if (iteration >= 100) {
            cleanup_empty_cells(50);
            iteration = 0;
        }
    }
}

void LiquidSim::update_simulation() {
    std::vector<Cell *> new_cells;
    std::vector<Cell *>::iterator it;

    for (it = cells.begin(); it != cells.end(); it++) {
        Cell *cell = (*it);

        cell->dir_bottom = false;

        if (cell->liquid_amount == 0.0f) {
            continue;
        }
        if (cell->liquid_amount < min_liquid_value) {
            cell->liquid_amount = 0.0f;
            continue;
        }

        // Check rule #1
        if (world->get_cell(cell->x, cell->y + 1) == -1) {
            float bottom_liquid_amount;
            Cell *bottom_cell = get_cell_by_position(cell->x, cell->y + 1);

            if (bottom_cell == nullptr) {
                bottom_liquid_amount = 0.0f;
            } else {
                bottom_liquid_amount = bottom_cell->liquid_amount;
            }

            float flow = get_vertical_flow_value(cell->new_liquid_amount,
                                                 bottom_liquid_amount) -
                         bottom_liquid_amount;
            if (bottom_liquid_amount > 0.0f && flow > min_flow_value) {
                flow *= flow_speed;
            }

            flow = std::max(flow, 0.0f);
            if (flow > std::min(max_flow_value, cell->liquid_amount)) {
                flow = std::min(max_flow_value, cell->liquid_amount);
            }

            if (flow != 0.0f) {
                if (bottom_cell == nullptr) {
                    bottom_cell = create_cell(cell->x, cell->y + 1, 0.0);
                    new_cells.push_back(bottom_cell);
                    cells_positions.insert(
                        std::make_pair(bottom_cell->uid, bottom_cell));
                }

                bottom_cell->new_liquid_amount += flow;
                cell->new_liquid_amount -= flow;
                cell->dir_bottom = true;
            }
        }

        // Check if there is still some liquid
        if (cell->new_liquid_amount < min_liquid_value) {
            cell->new_liquid_amount -= cell->new_liquid_amount;
            continue;
        }

        // Check rule #2
        if (world->get_cell(cell->x - 1, cell->y) == -1) {
            float left_liquid_amount;
            Cell *left_cell = get_cell_by_position(cell->x - 1, cell->y);

            if (left_cell == nullptr) {
                left_liquid_amount = 0.0f;
            } else {
                left_liquid_amount = left_cell->liquid_amount;
            }

            if (left_liquid_amount < cell->liquid_amount) {
                float flow = (cell->new_liquid_amount - left_liquid_amount) / 3;
                if (flow > min_flow_value) {
                    flow *= flow_speed;
                }

                flow = std::max(flow, 0.0f);
                if (flow > std::min(max_flow_value, cell->new_liquid_amount)) {
                    flow = std::min(max_flow_value, cell->new_liquid_amount);
                }

                if (flow != 0.0f) {
                    if (left_cell == nullptr) {
                        left_cell = create_cell(cell->x - 1, cell->y, 0.0f);
                        new_cells.push_back(left_cell);
                        cells_positions.insert(
                            std::make_pair(left_cell->uid, left_cell));
                    }

                    left_cell->new_liquid_amount += flow;
                    cell->new_liquid_amount -= flow;
                }
            }
        }

        // Check if there is still some liquid
        if (cell->new_liquid_amount < min_liquid_value) {
            cell->new_liquid_amount -= cell->new_liquid_amount;
            continue;
        }

        if (world->get_cell(cell->x + 1, cell->y) == -1) {
            float right_liquid_amount;
            Cell *right_cell = get_cell_by_position(cell->x + 1, cell->y);

            if (right_cell == nullptr) {
                right_liquid_amount = 0.0f;
            } else {
                right_liquid_amount = right_cell->liquid_amount;
            }

            if (right_liquid_amount < cell->liquid_amount) {
                float flow =
                    (cell->new_liquid_amount - right_liquid_amount) / 3;
                if (flow > min_flow_value) {
                    flow *= flow_speed;
                }

                flow = std::max(flow, 0.0f);
                if (flow > std::min(max_flow_value, cell->new_liquid_amount)) {
                    flow = std::min(max_flow_value, cell->new_liquid_amount);
                }

                if (flow != 0.0f) {
                    if (right_cell == nullptr) {
                        right_cell = create_cell(cell->x + 1, cell->y, 0.0f);
                        new_cells.push_back(right_cell);
                        cells_positions.insert(
                            std::make_pair(right_cell->uid, right_cell));
                    }

                    right_cell->new_liquid_amount += flow;
                    cell->new_liquid_amount -= flow;
                }
            }
        }

        // Check if there is still some liquid
        if (cell->new_liquid_amount < min_liquid_value) {
            cell->new_liquid_amount -= cell->new_liquid_amount;
            continue;
        }

        // Check rule #3
        if (world->get_cell(cell->x, cell->y - 1) == -1) {
            float top_liquid_amount;
            Cell *top_cell = get_cell_by_position(cell->x, cell->y - 1);

            if (top_cell == nullptr) {
                top_liquid_amount = 0.0f;
            } else {
                top_liquid_amount = top_cell->liquid_amount;
            }

            float flow = cell->new_liquid_amount -
                         get_vertical_flow_value(cell->new_liquid_amount,
                                                 top_liquid_amount);
            if (flow > min_flow_value) {
                flow *= flow_speed;
            }

            flow = std::max(flow, 0.0f);
            if (flow > std::min(max_flow_value, cell->new_liquid_amount)) {
                flow = std::min(max_flow_value, cell->new_liquid_amount);
            }

            if (flow != 0.0f) {
                if (top_cell == nullptr) {
                    top_cell = create_cell(cell->x, cell->y - 1, 0.0);
                    new_cells.push_back(top_cell);
                    cells_positions.insert(
                        std::make_pair(top_cell->uid, top_cell));
                }

                top_cell->new_liquid_amount += flow;
                cell->new_liquid_amount -= flow;
            }
        }

        // Check if there is still some liquid
        if (cell->new_liquid_amount < min_liquid_value) {
            cell->new_liquid_amount -= cell->new_liquid_amount;
            continue;
        }
    }

    // Add new cells
    cells.insert(std::end(cells), std::begin(new_cells), std::end(new_cells));

    // Update cells
    total_liquid_amount = 0.0;
    for (it = cells.begin(); it != cells.end(); it++) {
        Cell *cell = (*it);

        if (cell->liquid_amount == cell->new_liquid_amount) {
            cell->iteration_since_last_change += 1;
        } else {
            cell->iteration_since_last_change = 0;
            cell->liquid_amount = cell->new_liquid_amount;
        }

        refresh_cell_sprite(cell);

        total_liquid_amount += cell->liquid_amount;
    }
}

int LiquidSim::cleanup_empty_cells(int limit) {
    int removed = 0;
    std::vector<Cell *>::iterator it;

    for (it = cells.begin(); it != cells.end();) {
        Cell *cell = (*it);

        if (cell->liquid_amount == 0.0f &&
            cell->iteration_since_last_change > 10) {
            it = cells.erase(it);
            cells_positions.erase(cell->uid);
            remove_child(cell->node);
            cell->node->queue_free();
            delete cell;
            removed++;

            if (removed >= limit) {
                break;
            }
        } else {
            it++;
        }
    }

    return removed;
}

Cell *LiquidSim::get_cell_by_position(int x, int y) {
    int uid = hash_position(x, y);
    std::unordered_map<int, Cell *>::iterator it;

    it = cells_positions.find(uid);
    if (it != cells_positions.end()) {
        return cells_positions[uid];
    }

    return nullptr;
}

Cell *LiquidSim::create_cell(int x, int y, float amount) {
    Node2D *node =
        godot::Object::cast_to<godot::Node2D>(liquid_scene->instance());
    Cell *cell = new Cell(node, x, y, amount);
    add_child(node);
    return cell;
}

float LiquidSim::get_vertical_flow_value(float src_liquid_amount,
                                         float dst_liquid_amount) {
    float sum = src_liquid_amount + dst_liquid_amount;
    float value = 0.0;

    if (sum <= max_liquid_value) {
        value = max_liquid_value;
    } else if (sum < (2 * max_liquid_value + max_liquid_compression)) {
        value = (max_liquid_value * max_liquid_value +
                 sum * max_liquid_compression) /
                (max_liquid_value + max_liquid_compression);
    } else {
        value = (sum + max_liquid_compression) / 2.0;
    }

    return value;
}

void LiquidSim::refresh_cell_sprite(Cell *cell) {
    float translation;
    float scale = std::min(cell->liquid_amount, 1.0f);

    // Check if liquid is flowing down for better rendering
    Cell *top_cell = get_cell_by_position(cell->x, cell->y - 1);
    if (top_cell != nullptr &&
        (top_cell->liquid_amount >= min_liquid_value || top_cell->dir_bottom)) {
        scale = 1.0f;
    }

    translation = (cell->sprite_height - (cell->sprite_height * scale));
    cell->node->set_position(world->map_to_world(Vector2(cell->x, cell->y)));
    cell->node->translate(Vector2(0.0, translation));
    cell->node->set_scale(Vector2(1.0, scale));

    float m = lerp(1.4f, 0.2f, cell->liquid_amount / 4.0f);
    cell->sprite_node->set_modulate(Color(m, m, m));
}

void LiquidSim::refresh_all() {
    std::vector<Cell *>::iterator it;

    for (it = cells.begin(); it != cells.end(); it++) {
        Cell *cell = (*it);
        refresh_cell_sprite(cell);
    }
}

void LiquidSim::start() { started = true; }

void LiquidSim::stop() { started = false; }

void LiquidSim::clear() {
    std::vector<Cell *>::iterator it;

    for (it = cells.begin(); it != cells.end(); it++) {
        Cell *cell = (*it);
        remove_child(cell->node);
        cell->node->queue_free();
        delete cell;
    }

    cells.clear();
    cells_positions.clear();
    started = false;
}

float LiquidSim::add_liquid(int x, int y, float amount) {
    Cell *cell = get_cell_by_position(x, y);
    if (cell == nullptr) {
        cell = create_cell(x, y, amount);
        cells.push_back(cell);
        cells_positions.insert(std::make_pair(cell->uid, cell));
    } else {
        cell->new_liquid_amount += amount;
    }

    refresh_cell_sprite(cell);

    return cell->new_liquid_amount;
}

void LiquidSim::del_liquid(int x, int y) {
    Cell *cell = get_cell_by_position(x, y);
    if (cell == nullptr) {
        return;
    }

    cell->new_liquid_amount = 0.0f;
    refresh_cell_sprite(cell);
}

void LiquidSim::set_liquid(int x, int y, float amount) {
    Cell *cell = get_cell_by_position(x, y);
    if (cell == nullptr) {
        cell = create_cell(x, y, amount);
        cells.push_back(cell);
        cells_positions.insert(std::make_pair(cell->uid, cell));
    } else {
        cell->new_liquid_amount = amount;
    }

    refresh_cell_sprite(cell);
}

float LiquidSim::get_liquid(int x, int y) {
    Cell *cell = get_cell_by_position(x, y);
    if (cell == nullptr) {
        return 0.0f;
    }

    return cell->liquid_amount;
}