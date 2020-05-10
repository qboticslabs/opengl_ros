#!/bin/bash

#Getting the current version of OpenCV
opencv_version=$(opencv_version)
echo "Current OpenCV version is:" $opencv_version

#If you want to install opencv, make this YES
OPENCV_INSTALL='NO'

if [ -d "~/opencv_dev_install" ]

then
	echo "The installation is already exist"
else

	mkdir ~/opencv_dev_install
	cd ~/opencv_dev_install
	git clone https://github.com/opencv/opencv.git
	cd opencv
	git fetch --all --tags
	git checkout $opencv_version
	mkdir build
	cd build
	
	cmake -D CMAKE_BUILD_TYPE=RELEASE \
      	      -D CMAKE_INSTALL_PREFIX=/usr/local \
              -D WITH_CUDA=OFF \
	      -D BUILD_opencv_dnn_modern=ON \
              -D ENABLE_PRECOMPILED_HEADERS=OFF \
              -D WITH_QT=ON \
              -D WITH_OPENGL=ON \
              -D BUILD_EXAMPLES=OFF \
              -D BUILD_opencv_apps=OFF \
              -D BUILD_DOCS=OFF \
              -D BUILD_PERF_TESTS=OFF \
              -D BUILD_TESTS=OFF \
              -D OPENCV_GENERATE_PKGCONFIG=YES ..
	
	make

	if [ $OPENCV_INSTALL = 'YES' ]; then
          sudo make install
          sudo ldconfig

        fi  


fi
