#pragma once

#include <trollworks.hpp>

namespace game::scenes {
  class titlescreen final : public tw::scene {
    public:
      virtual void load(entt::registry& registry) override;
      virtual void unload(entt::registry& registry) override;
  };

  namespace archetypes {
    entt::entity camera(entt::registry& registry);
    entt::entity background(entt::registry& registry);
  }
}
