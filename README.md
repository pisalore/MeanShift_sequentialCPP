# MeanShift CPP
### A simple implementation of MeanShift algorithm using C++.

MeanShift is a simple non parametric clustering algorithm: it does not know how much clusters it has to find given a dataset. It is possibile thanks to its implementation, which is based on KDE (Kernel Density Estimation) and kernel functions. At each iteration, each point is processed by the kernel function and shifted to its nearest **peak**. Each peak is detected thanks to the KDE, which is a probability density function, obtained with points processing by kernel function.

Peaks define clusters, since where there is a peak there is a greater points presence.
Iterations go on until convergence is not reached.

In this repo the Mean Shift algorithm implementation is presented; this repo belongs to a largest project for the Master Degree Parallel Computing course: sequential Mean Shift peformances will be compared with the parallel ones.
The parallel Mean Shift has been implemented using CUDA: here the code [CUDA MeanShift Algorithm](https://github.com/pisalore/MeanShift_CUDA).
For further informations about Mean Shift algorithm, C++ sequential implementation, CUDA parallel implementation and performances analysis, please consult the realted paper [here](https://github.com/pisalore/MeanShift_CUDA/blob/master/PC_MeanShift_midterm.pdf).
