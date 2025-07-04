#include "evm/baseline.h"
#include "evm/instructions_opcodes.h"
#include <limits>

namespace baseline {
    CodeAnalysis::JumpdestMap analyze_jumpdests(evmc::bytes_view code) {
        // To find if op is any PUSH opcode (OP_PUSH1 <= op <= OP_PUSH32)
        // it can be noticed that OP_PUSH32 is INT8_MAX (0x7f) therefore
        // static_cast<int8_t>(op) <= OP_PUSH32 is always true and can be skipped.
        static_assert(OP_PUSH32 == std::numeric_limits<int8_t>::max());

        CodeAnalysis::JumpdestMap map(code.size());  // Allocate and init bitmap with zeros.
        for (size_t i = 0; i < code.size(); ++i)
        {
            const auto op = code[i];
            if (static_cast<int8_t>(op) >= OP_PUSH1)  // If any PUSH opcode (see explanation above).
                i += op - size_t{OP_PUSH1 - 1};       // Skip PUSH data.
            else if (op == OP_JUMPDEST)
                map[i] = true;
        }

        return map;
    }

    std::unique_ptr<uint8_t[]> pad_code(evmc::bytes_view code) {
        // We need at most 33 bytes of code padding: 32 for possible missing all data bytes of PUSH32
        // at the very end of the code; and one more byte for STOP to guarantee there is a terminating
        // instruction at the code end.
        constexpr auto padding = 32 + 1;

        auto padded_code = std::make_unique<uint8_t[]>(code.size() + padding);
        std::copy(code.data(), code.data() + code.size(), padded_code.get());
        std::fill_n(&padded_code[code.size()], padding, uint8_t{OP_STOP});
        return padded_code;
    }

    CodeAnalysis analyze(evmc::bytes_view code) {
        // TODO: The padded code buffer and jumpdest bitmap can be created with single allocation.
        return {pad_code(code), code.size(), analyze_jumpdests(code)};
    }
}