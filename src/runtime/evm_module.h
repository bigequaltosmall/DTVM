#include "runtime/module.h"

namespace zen {

namespace runtime {

class EVMModule final : public BaseModule<Module> {
  friend class RuntimeObjectDestroyer;
  friend class action::EVMModuleLoader;

public:
  static EVMModuleUniquePtr newEVMModule(Runtime &RT,
                                         CodeHolderUniquePtr CodeHolder);

  virtual ~EVMModule();

  uint8_t *code;
  size_t code_size;

private:
  EVMModule(Runtime *RT);
  EVMModule(const Module &Other) = delete;
  EVMModule &operator=(const Module &Other) = delete;
  CodeHolderUniquePtr CodeHolder;

  uint8_t *initCode(size_t size) { return (uint8_t *)allocateZeros(size); }
};

} // namespace runtime
} // namespace zen
