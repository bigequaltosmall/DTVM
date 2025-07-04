#include "evm/baseline.h"

namespace baseline {
  struct StackSpace {
    using value_type = intx::uint256;

    static constexpr size_t max_stack_size = 1024;

    value_type data[max_stack_size];
    value_type* bottom() noexcept { return data; }
    value_type* top = data;

    bool push(const value_type& value) noexcept {
        if (top >= data + max_stack_size) return false;
        *top++ = value;
        return true;
    }

    void pop() noexcept {
        if (top > data) --top;
    }

    value_type& operator[](size_t index) noexcept {
        return data[index];
    }
  };

  struct ExecutionState {
    StackSpace stack_space;              // 栈空间
    std::vector<uint8_t> original_code;  // 原始代码（用于 trace 判断）
    evmc_status_code status = EVMC_SUCCESS;
  };
}