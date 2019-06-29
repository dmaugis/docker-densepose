# Use Caffe2 image as parent image
FROM caffe2/caffe2:snapshot-py2-cuda9.0-cudnn7-ubuntu16.04
#FROM dmaugis/caffe2

COPY install-densepose.sh /tmp/
ADD  densepose /densepose
RUN  /tmp/install-densepose.sh  
#RUN  rm /tmp/install-densepose.sh
WORKDIR /densepose
ADD DensePose_ResNet101_FPN_s1x-e2e.pkl DensePose_ResNet101_FPN_s1x-e2e.pkl
ADD run_inference.sh .
CMD ["bash","-c","./run_inference.sh"]



