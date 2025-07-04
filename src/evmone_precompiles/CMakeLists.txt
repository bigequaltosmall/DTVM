# Copyright (C) 2021-2023 the DTVM authors. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

set(EVM_SRCS 
    keccak.c
    )

add_library(evmone_precompiles OBJECT ${EVM_SRCS})

# 设置 evm 目标使用 C++20 标准
set_target_properties(evmone_precompiles PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)