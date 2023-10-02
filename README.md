# SegSpaceDetector

Efficient Graph-based image segmentation and free spot detection in crowded scenes without Deep Learning.
<p align="center">

<img src="https://github.com/DaniAffCH/CrowdSpaceDetector/blob/main/assets/raw-rgb.png" alt="Image description" width="300"/>
<img src="https://github.com/DaniAffCH/CrowdSpaceDetector/blob/main/assets/segmentation.png" alt="Image description" width="300"/>
<img src="https://github.com/DaniAffCH/CrowdSpaceDetector/blob/main/assets/heatmap.png" alt="Image description" width="300"/>
</p>

## Features

- **Graph-based Semantic Segmentation:** Utilize advanced graph-based algorithms to perform semantic segmentation on RGB-D images, allowing you to separate objects and regions in the scene efficiently.

- **Largest Free Area Detection:** Identify and locate the largest free area within the segmented image. This feature is particularly useful for applications involving crowd or space management.

- **Computational Efficiency:** Designed with computational efficiency in mind, making it well-suited for real-time or resource-constrained applications, such as robotic systems.


## Prerequisites 
- OpenCV
- CMake (version 3.12 or higher)

## Parameters

In the `main.cpp` you can adjust several parameters to adapt the algorithm to different situations and light conditions.
- `rgbPath`: the path of your rgb image
- `dPath`: the path of the depth component of the image
- `gaussianBlur`: You can control the amount of image smoothing by adjusting this parameter. It defines the standard deviation (σ) used to create the Gaussian kernel for image smoothing. Higher values will result in more significant smoothing.
- `kValue`: This parameter sets the threshold used for distinguishing between different regions in the image. A higher value will make the algorithm more sensitive to variations in pixel intensity and may lead to finer segmentation.
- `minimumComponentSize`: Adjust this parameter to determine the minimum number of pixels required for a region to be considered valid. Smaller values will allow smaller regions to be detected, while larger values will filter out smaller regions.

## Installation

To use SegSpaceDetector, you'll need to follow these installation steps:

1. **Clone the Repository:**

   ```shell
   git clone https://github.com/DaniAffCH/CrowdSpaceDetector.git
   cd CrowdSpaceDetector
   ```

2. **Build The Project:**

   ```shell
   mkdir build
   cd build
   cmake .. -DDEBUG=[ON/OFF]
   make
   ```
   Set Debug ON if you want to show the processed images.
   
3. **Run the Application:**
   ```shell
   ./spaceDetector
   ```
## Contributing

If you have any ideas for improvements, new features, or encounter issues while using the software, please point it out opening an issue. Here's how you can contribute:

1. **Open an Issue:**
   If you have an idea for an enhancement, a feature request, or you've encountered a bug or any unexpected behavior, please open an issue on our [GitHub repository](https://github.com/DaniAffCH/SegSpaceDetector/issues). Be sure to provide a clear and detailed description of your suggestion or the issue you're facing.

2. **Submit a Pull Request (PR):**
   If want to contribute directly by implementing new features or fixing existing issues, you can submit a pull request.
   
## Acknowledgments
The image semantic segmentation is partially based [Graph-Based-Image-Segmentation](https://github.com/IamMohitM/Graph-Based-Image-Segmentation) 
