
nvidia-docker run -it --network host -v $(realpath ./datasets):/input  -v $(realpath ./results):/output  dmaugis/densepose

