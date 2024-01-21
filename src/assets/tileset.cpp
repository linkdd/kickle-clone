#include <game/assets/tileset.hpp>

namespace game::assets {
  tileset::loader_type::result_type tileset::loader_type::operator()(
    entt::resource<tw::sdl::aseprite::spritesheet> spritesheet,
    const tinyxml2::XMLDocument& doc
  ) const {
    auto result = std::make_shared<tileset>();
    result->spritesheet = spritesheet;

    auto tileset_node = doc.FirstChildElement("tileset");
    auto tile_node = tileset_node->FirstChildElement("tile");
    while (tile_node) {
      auto id = tile_node->IntAttribute("id");
      auto name = tile_node->Attribute("type");
      result->tiles[id] = name;
      tile_node = tile_node->NextSiblingElement("tile");
    }

    return result;
  }
}
