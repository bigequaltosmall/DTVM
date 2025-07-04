# Copyright (C) 2021-2023 the DTVM authors. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

set(EVM_SRCS baseline_analysis.cpp)

add_library(evm OBJECT ${EVM_SRCS})
