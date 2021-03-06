#include <iostream>
#include <omp.h>
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include <math.h>
#include <vector>
#include "csvio.h"

#define MAX_ITERATIONS 15
#define rev_sqrt_two_pi 0.3989422804
#define rev_two_pi rev_sqrt_two_pi*rev_sqrt_two_pi
#define BW 2.0

float gaussian_kernel(float dist2, float bandwidth) {
    const float rev_bandwidth = 1. / bandwidth;
    const float d2_frac_b2 = dist2 * rev_bandwidth * rev_bandwidth;
    float div = 1. / rev_two_pi * rev_bandwidth;
    float exp_ = div * expf(- 0.5 * d2_frac_b2);
    return exp_;
}

void ms_iteration_2D(float *X, const float *I, const float * originalPoints, const int nPoints) {
	//iterate over O, X
	const int dim = 2;
	for (int i = 0; i < nPoints; i++) {
		// for every pixel
		glm::f32vec2 numerator = glm::f32vec2(0.0, 0.0);
		float denominator = 0.0;

		int it = i * dim;
		glm::f32vec2 y_i = glm::f32vec2(I[it], I[it + 1]);
		for (int n_i = 0; n_i < nPoints; ++n_i) {
			int n_row = n_i;
			int it = n_row * dim;

			glm::f32vec2 x_j = glm::f32vec2(originalPoints[it], originalPoints[it + 1]);
			float distance2 = glm::distance2(y_i, x_j);
			float weight = gaussian_kernel(distance2, BW);
			numerator += x_j * weight;
			denominator += weight;
		}
		numerator /= denominator;
		X[it] = numerator.x;
		X[it + 1] = numerator.y;
	}
}

int main() {

	std::vector<float> inputData;
	float * InputPoints_ptr;
	float * OutputPoints_ptr;
	float * OriginalPoints_ptr;

	std::string delimiter = ";";
	std::string filename = "../datasets/dataset1000.csv";
	//write2VecTo(filename, delimiter, inputData);
	read2VecFrom(filename, delimiter, inputData);

	OriginalPoints_ptr = new float[inputData.size()];
	InputPoints_ptr = new float[inputData.size()];
	OutputPoints_ptr = new float[inputData.size()];
	const int nPoints = inputData.size() / 2;

	for (int i = 0; i < inputData.size(); i++) {
		OriginalPoints_ptr[i] = InputPoints_ptr[i] = inputData[i];
	}
	inputData.resize(0);

	double t1 = omp_get_wtime();
	for (int i = 0; i < MAX_ITERATIONS; i++) {
		std::cout << "Iteration n: " << i << " started." << std::endl;
		ms_iteration_2D(OutputPoints_ptr, InputPoints_ptr, OriginalPoints_ptr, nPoints);
		// new shifted point set become the input. the old input is now writable
		std::swap(InputPoints_ptr, OutputPoints_ptr);
	}
	std::swap(InputPoints_ptr, OutputPoints_ptr);
	double t2 = omp_get_wtime() - t1;
	std::cout << "Mean Shift completed in: " << t2 << std::endl;

	std::cout << "Writing result." << std::endl;
	inputData.resize(nPoints * 2);
	std::vector<float> outputData(nPoints*2);
	for (int i = 0; i < outputData.size(); i++) {
		outputData[i] = OutputPoints_ptr[i];
	}
	write2VecTo(std::string("results.csv"), delimiter, inputData);

	//find cluster
	t1 = omp_get_wtime();
	std::vector<glm::vec2> cluster;

	cluster.push_back(
			glm::vec2(outputData[0], outputData[1]));

	std::vector<float>& points = outputData;
	for (int i = 2; i < points.size(); i += 2) {
		bool insideNoCluster = true;
		glm::vec2 point = glm::vec2(points[i], points[i + 1]);
		for (int j = 0; j < cluster.size(); j++) {
			glm::vec2 cluster_ = cluster[j];
			glm::vec2 sub = cluster_ - point;
			float distance = sqrt(glm::dot(sub, sub));
			if (distance < BW) {
				insideNoCluster = false;
			}
		}
		if (insideNoCluster) {
			cluster.push_back(point);
		}
	}

	for (int i = 0; i < cluster.size(); i++) {
		std::cout << "Cluster " << i << ": " << "(" << cluster[i].x << ", "
				<< cluster[i].y << ")" << std::endl;
	}
	t2 = omp_get_wtime() - t1;
	std::cout << "Time to find clusters: " << t2 << std::endl;

	write2VecTo(std::string("results.csv"), delimiter, outputData);

	delete[] InputPoints_ptr;
	delete[] OutputPoints_ptr;
	delete[] OriginalPoints_ptr;

	return 0;
}
