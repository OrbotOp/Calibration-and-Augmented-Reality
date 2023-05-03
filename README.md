# Calibration-and-Augmented-Reality

## File Structure

#### keep the following files in same folder

- main.cpp
- harris_corner.cpp
- aug_real.cpp
- util.cpp
- util.h
- teapot.obj


----------------------------------------------------------------------------------------
#### three programs for diffrent tasks (change if values to run diffrent codes)
```sh
./main (for camera calibration) (set pre processor directives 0 or 1)
./harris_corner (for harris corner detection) (set pre processor directives if 0 or 1)
./aug_real (for display virtual object) (set pre processor directives if 0 or 1)
```

---------------------------------------------------------------------------------------

#### For the main code

- press 's' save current frame for calibration (min 5 times)
- press 'c' which will give calibration matrix and save the parametrs values to csv file

--------------------------------------------------------------------------------------

#### For aug_real (run maincode first and calibrate)

- press 'a' to display 3D axes (By default)
- press 'w' to display virtual object

--------------------------------------------------------------------------------------
#### For harris_corner
- by default it will show harris corners only.
- press 's' to save image


