# Copyright (C) 2021-2023 the DTVM authors. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

set(EVM_SRCS 
    baseline_analysis.cpp
    baseline_execution.cpp
    baseline_instruction_table.cpp
    delegation.cpp
    eof.cpp
    instructions_calls.cpp
    instructions_storage.cpp
    tracing.cpp
    vm.cpp
    )

add_library(evmone OBJECT ${EVM_SRCS})

# 设置 evm 目标使用 C++20 标准
set_target_properties(evmone PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)