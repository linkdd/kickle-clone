#pragma once

#include <aitoolkit/fsm.hpp>
#include <game/state.hpp>

namespace game::lifecycle {
  class titlescreen final : public aitoolkit::fsm::state<game::state::global> {
    public:
      virtual void enter(game::state::global& global_state) override;
  };
}
