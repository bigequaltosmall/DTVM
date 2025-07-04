#include "evm/baseline.h"

namespace baseline {
  void execute(int64_t gas, const CodeAnalysis& analysis) noexcept {
    const auto code = analysis.executable_code();
    const auto code_begin = code.data();

    auto& state = vm.get_execution_state(static_cast<size_t>(msg.depth));
    state.reset(msg, rev, host, ctx, analysis.raw_code());

    state.analysis.baseline = &analysis;  // Assign code analysis for instruction implementations.

    const auto& cost_table = get_baseline_cost_table(state.rev, analysis.eof_header().version);

    gas = dispatch<false>(cost_table, state, gas, code_begin);

    const auto gas_left = (state.status == EVMC_SUCCESS || state.status == EVMC_REVERT) ? gas : 0;
    const auto gas_refund = (state.status == EVMC_SUCCESS) ? state.gas_refund : 0;

    assert(state.output_size != 0 || state.output_offset == 0);
    const auto result =
        (state.deploy_container.has_value() ?
                evmc::make_result(state.status, gas_left, gas_refund,
                    state.deploy_container->data(), state.deploy_container->size()) :
                evmc::make_result(state.status, gas_left, gas_refund,
                    state.output_size != 0 ? &state.memory[state.output_offset] : nullptr,
                    state.output_size));

    if (INTX_UNLIKELY(tracer != nullptr))
        tracer->notify_execution_end(result);

    return result;
}
};