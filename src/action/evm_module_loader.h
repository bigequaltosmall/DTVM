#include "action/module_loader.h"

namespace zen::action {

class EVMModuleLoader final {
public:
  using Byte = std::optional<evmc::bytes>;

  explicit EVMModuleLoader(runtime::EVMModule &mod, const char *data)
      : Mod(mod), Data(evmc::from_spaced_hex(std::string(data))) {}

  void load() {
    Mod.code = Mod.initCode(Data->size());
    std::memcpy(Mod.code, Data->data(), Data->size());
    Mod.code_size = Data->size();
  }

private:
  runtime::EVMModule &Mod;
  const Byte Data;
};

} // namespace zen::action
