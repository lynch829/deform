#pragma once

namespace stk {
    class GpuVolume;
}

namespace gpu {
    // df           : Displacement field
    // initial_df   : Initial displacement field of current level
    // cost         : Destination for cost (float4, with cost in x+, y+, z+)
    void run_regularizer_kernel(
        const stk::GpuVolume& df,
        const stk::GpuVolume& initial_df,
        stk::GpuVolume& cost
    );
    void run_ssd_kernel(
        const stk::GpuVolume& fixed,
        const stk::GpuVolume& moving,
        const stk::GpuVolume& df,
        stk::GpuVolume& cost_acc
    );
    void run_ncc_kernel(
        const stk::GpuVolume& fixed,
        const stk::GpuVolume& moving,
        const stk::GpuVolume& df,
        int radius,
        stk::GpuVolume& cost_acc
    );
}
