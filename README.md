# deform

## Prerequisites
* CMake : https://cmake.org/

## Build
Use CMake (>=3.8) to generate build options of your own choosing.

## Run
To perform a registration
`deform registration -p <param file> -f0 <fixed_0> ... -f<i> <fixed_i> -m0 <moving_0> ... -m<i> <moving_i>`

| Argument                    |                                             |
| --------------------------- | ------------------------------------------- |
| `-f<i> <file>`              | Filename of the i:th fixed image (i &lt; 8)*.  |
| `-m<i> <file>`              | Filename of the i:th moving image (i &lt; 8)*. |
| `-fp <file>`                | Filename for the fixed landmarks.           |
| `-mp <file>`                | Filename for the moving landmarks.          |
| `-d0 <file>`                | Filename for initial deformation field.     |
| `-constraint_mask <file>`   | Filename for constraint mask.               |
| `-constraint_values <file>` | Filename for constraint values.             |
| `-p <file>`                 | Filename of the parameter file.             |
| `-o <file>`                 | Filename of the resulting deformation field |

* Requires a matching number of fixed and moving images.

### Parameter file example

```yaml
pyramid_levels: 6
pyramid_stop_level: 0
constraints_weight: 1000.0
landmarks_weight: 1.0
landmarks_stop_level: 0
block_size: [12, 12, 12]
block_energy_epsilon: 1e-7
step_size: 0.5
step_size_unit: mm
regularization_weight: 0.1

image_slots:

  # water
  - resampler: gaussian
    normalize: true
    cost_function:
      - function: ssd
        weight: 0.3
      - function: ncc
        weight: 0.4
        radius: 2
      - function: mi
        weight: 0.6
        sigma: 4.5
        bins: 256

  # sfcm
  - resampler: gaussian
    normalize: true
    cost_function: ssd
```

First two parameters, `pyramid_levels` and `pyramid_stop_level`, defines the size of the pyramid and at which level to stop the registration. Each level halves the resolution of the input volumes. Setting `pyramid_stop_level` to > 0 specifies that the registration should not be run on the original resolution (level 0).

`constraints_weight` sets the weight that is applied for constrained voxels. A really high value means hard constraints while a lower value may allow constraints to move a certain amount. Cost for constrained voxels are applied as constraint_weight * squared_distance, where squared_distance is the distance from the constraint target. See cost function for more info.

`landmarks_weight` sets the weight for the landmark cost term when performing
landmark-based registration.  In order to perform landmark-based registration,
a set of fixed and moving landmarks must be supplied.  The implementation of
the landmark-based unary energy term is inspired to [[2]](#2), but the cost in
each term of the sum is also proportional to the distance between the current
displacement and the landmark displacement. It is possible to limit the usage
of the landmarks up to a certain height of the resolution pyramid by assigning
to `landmarks_stop_level` a value greater than zero.

`block_size` size of the block (in voxels) for the block-wise solver. A block size of (0,0,0) will result in a single block for the whole volume.

`block_energy_epsilon`, minimum percentage decrease of the block energy required to accept a solution. Higher epsilon will result in lower run time but also lower quality.

`step_size`, this is the step size that the solver will use. Its unit of
measure is given by the parameter `step_size_unit`, that can be `mm` or `voxel` (default: `mm`).

`regularization_weight`, value between 0 and 1 used as weight for the regularization term. Cost function is specified as `cost = (1-a)*D + a*R`, where `D` is the data term, `R` is the regularization term, and `a` is the regularization weight.

`image_slots`, specifies how to use the input images. `resampler` only supports 'gaussian' for now, `normalize` specifies whether the volumes should be normalized before the registration, and `cost_function` allows to provide one or more cost functions to use. Its value can be the name of a single function (`ssd` for squared distance, `ncc` for normalized cross correlation, `mi` for mutual information), in which case its weight is assumed to be `1.0`, otherwise one or multiple weighted components can be specified by listing each function and its weight. Each function can accept a set of parameters.

The parameters available for each function are:
+ `ssd`: no parameters available
+ `ncc`:
  + `radius` (int): radius of the cross-correlation kernel (default: `2`)
+ `mi`:
  + `bins` (int): number of histogram bins when approximating probability densities (default: `255`)
  + `sigma` (float): standard deviation of the Gaussian kernel used to approximate probability densities (default: `4.5`)

## References
+ <a id="1"></a>[1] Junhwan Kim, Vladimir Kolmogorov, Ramin Zabih: *Visual correspondence using energy minimization and mutual information.* Proceedings of the Ninth IEEE International Conference on Computer Vision, 1033-1040, 2003.
+ <a id="2"></a>[2] Herve Lombaert, Yiyong Sun, Farida Cheriet: *Landmark-based non-rigid registration via graph cuts*, International Conference Image Analysis and Recognition, 166–175, 2007
