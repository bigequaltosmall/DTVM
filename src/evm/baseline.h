#include <vector>
#include <memory>
#include "utils/bytes.h"

namespace baseline
{
class CodeAnalysis
{
public:
    using JumpdestMap = std::vector<bool>;

private:
    evmc::bytes_view m_raw_code;         ///< Unmodified full code.
    evmc::bytes_view m_executable_code;  ///< Executable code section.
    JumpdestMap m_jumpdest_map;    ///< Map of valid jump destinations.

    /// Padded code for faster legacy code execution.
    /// If not nullptr the executable_code must point to it.
    std::unique_ptr<uint8_t[]> m_padded_code;

public:
    /// Constructor for legacy code.
    CodeAnalysis(std::unique_ptr<uint8_t[]> padded_code, size_t code_size, JumpdestMap map)
      : m_raw_code{padded_code.get(), code_size},
        m_executable_code{padded_code.get(), code_size},
        m_jumpdest_map{std::move(map)},
        m_padded_code{std::move(padded_code)}
    {}

    /// Constructor for EOF.
    CodeAnalysis(evmc::bytes_view container, evmc::bytes_view executable_code)
      : m_raw_code{container}, m_executable_code(executable_code)
    {}

    /// The raw code as stored in accounts or passes as initcode. For EOF this is full container.
    [[nodiscard]] evmc::bytes_view raw_code() const noexcept { return m_raw_code; }

    /// The pre-processed executable code. This is where interpreter should start execution.
    [[nodiscard]] evmc::bytes_view executable_code() const noexcept { return m_executable_code; }

    /// Check if given position is valid jump destination. Use only for legacy code.
    [[nodiscard]] bool check_jumpdest(uint64_t position) const noexcept
    {
        if (position >= m_jumpdest_map.size())
            return false;
        return m_jumpdest_map[static_cast<size_t>(position)];
    }
};
    CodeAnalysis analyze(evmc::bytes_view code);
};