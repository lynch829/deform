#pragma once

#include <vector>

class Volume;

/// Image pyramid representing a set of images each smaller than the other.
/// I.e. 
/// Level:                      Size [elements]:
///     3        | |            1
///     2       | | |           2
///     1     | | | | |         4
///     0 | | | | | | | | |     8
///
/// Level 0 is our base image, typically the images used as input for the registration (fixed or moving)
/// For each step the size of the image is halved
class VolumePyramid
{
public:
    typedef Volume (*DownsampleFn)(const Volume&, float scale);
    typedef Volume (*DownsampleWithResidualFn)(const Volume&, float scale, Volume& residual);

    VolumePyramid();
    ~VolumePyramid();

    /// Sets the size of the pyramid. 
    /// This needs to be called before build_from_base, otherwise you'll need to rebuild the pyramid
    void set_level_count(int levels);

    /// Sets the given base at index 0 and builds the rest of the pyramid using
    /// the provided resampling function.
    /// downsample_fn : Resampling function, required to support downsampling
    void build_from_base(const Volume& base, DownsampleFn downsample_fn);

    /// Sets the given base at index 0 and builds the rest of the pyramid using
    /// the provided resampling function.
    /// Using this method in comparison to build_from_base allows for saving the residuals of the
    ///     downsampling. This could be useful for deformation fields.
    /// downsample_fn : Resampling function, required to support downsampling
    void build_from_base_with_residual(const Volume& base, DownsampleWithResidualFn downsample_fn);

#ifdef DF_ENABLE_HARD_CONSTRAINTS

    /// Special case for building a constraints pyramid, as such pyramid requires a mask when resampling
    void build_constraints_pyramid(const VolumeUInt8& mask, const VolumeFloat3& base);

#endif // DF_ENABLE_HARD_CONSTRAINTS

    /// Sets the volume at the given level without rebuilding any other levels of the pyramid. 
    void set_volume(int level, const Volume& vol);

    /// Returns the volume at the specified level, 0 being the base volume.
    const Volume& volume(int level) const;

    /// Returns the residual volume at the specified level, 0 being the base volume
    /// This will fail if the pyramid wasn't built using build_*_with_residual
    const Volume& residual(int level) const;

    /// Returns the number of levels in this pyramid
    int levels() const;

private:
    int _levels;
    bool _save_residuals;

    std::vector<Volume> _volumes;

    /// Residuals:
    /// Index 0 holds the residual from downsampling pyramid level 0 to 1, index 1 holds 1 to 2, ... 
    std::vector<Volume> _residuals;
};
