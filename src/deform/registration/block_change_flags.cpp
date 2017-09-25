#include "block_change_flags.h"

#include <framework/math/int3.h>

BlockChangeFlags::BlockChangeFlags(const int3& block_count) : _block_count(block_count)
{
    _flags.resize(2 * (block_count.x + 1) * 2 * (block_count.y + 1) * 2 * (block_count.z + 1));
    std::fill(_flags.begin(), _flags.end(), uint8_t(1));
}
bool BlockChangeFlags::is_block_set(const int3& block_p, bool shift) const
{
    int3 sub_block_offset = shift ? int3{0, 0, 0} : int3{1, 1, 1};

    for (int z = 0; z < 2; ++z) 
    {
        for (int y = 0; y < 2; ++y) 
        {
            for (int x = 0; x < 2; ++x) 
            {
                int3 sb = 2 * block_p + sub_block_offset + int3{x, y, z};
                if (flag(sb) != 0)
                    return true;
            }
        }
    }
    return false;
}
void BlockChangeFlags::set_block(const int3& block_p, bool changed, bool shift)
{
    // We keep track of the changed flags for both shifted and non-shifted blocks simultaneously.
    // The mask looks like this: <b7> ... <b2> <ShiftFlag> <NonShiftFlag>
    // If any of the flags are set, this indicates the block is changed

    int3 sub_block_offset = shift ? int3{0, 0, 0} : int3{1, 1, 1};

    uint8_t this_mask = shift ? 0x2 : 0x1;
    uint8_t opposite_mask = shift ? 0x1 : 0x2;
    for (int i = 0; i < 8; ++i) 
    {
        int x = (i % 2) % 2;
        int y = (i / 2) % 2;
        int z = i / 4;

        int3 sb = 2 * block_p + sub_block_offset + int3{x, y, z};
        set(sb, (flag(sb) & opposite_mask) | ((changed ? this_mask : 0) & this_mask));
    }
}
uint8_t BlockChangeFlags::flag(const int3& subblock_p) const
{
    int i = subblock_p.z * _block_count.x * _block_count.y + subblock_p.y * _block_count.x + subblock_p.x;
    if (i > 0 && i < int(_flags.size()))
        return _flags[subblock_p.z * _block_count.x * _block_count.y + subblock_p.y * _block_count.x + subblock_p.x];
    return 0;
}
void BlockChangeFlags::set(const int3& subblock_p, uint8_t flags)
{
    int i = subblock_p.z * _block_count.x * _block_count.y + subblock_p.y * _block_count.x + subblock_p.x;
    if (i > 0 && i < int(_flags.size()))
        _flags[subblock_p.z * _block_count.x * _block_count.y + subblock_p.y * _block_count.x + subblock_p.x] = flags;
}