// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <ddk/protocol/block.h>
#include <zircon/types.h>

namespace zxcrypt {

// |extra_op_t| is the extra information placed in the tail end of |block_op_t|s queued against a
// |zxcrypt::Device|.
static_assert(sizeof(uintptr_t) <= sizeof(uint64_t), "uintptr_t > uint64_t");
struct extra_op_t {
    // Used to link deferred block requests
    extra_op_t* next;
    // Memory region to use for cryptographic transformations.
    uint8_t* data;
    // The remaining are used to save fields of the original block request which may be altered
    zx_handle_t vmo;
    uint32_t length;
    uint64_t offset_dev;
    uint64_t offset_vmo;
    void (*completion_cb)(block_op_t* block, zx_status_t status);
    void* cookie;
};

// Translates |block_op_t|s to |extra_op_t|s and vice versa.
extra_op_t* BlockToExtra(block_op_t* block, size_t op_size);
block_op_t* ExtraToBlock(extra_op_t* extra, size_t op_size);

} // namespace zxcrypt
