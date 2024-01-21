#pragma once

#include <trollworks.hpp>

namespace game::scenes {
  class level final : public tw::scene {
    public:
      level(entt::id_type level_id) : m_level_id(level_id) {}

      virtual void load(entt::registry& registry) override;
      virtual void unload(entt::registry& registry) override;

    private:
      entt::id_type m_level_id;
  };
}
