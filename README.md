# MeanShift CPP
### A simple implementation of MeanShift algorithm using C++.

MeanShift is a simple non parametric clustering algorithm: it does not to know how much clusters it has to find given a dataset. It is possibile thanks to its implementation, which is based on KDE (Kernel Density Estimation) and kernel functions. At each iteration, each point is processed by the kernel function and shifted to its nearest **peak**. Each peak is detected thanks to the KDE, which is a probability density function, obtained with points processing by kernel function.
