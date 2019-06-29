if [ ! -f DensePose_ResNet101_FPN_s1x-e2e.pkl ]; then
#wget https://s3.amazonaws.com/densepose/DensePose_ResNet101_FPN_s1x-e2e.pkl
wget https://dl.fbaipublicfiles.com/densepose/DensePose_ResNet101_FPN_s1x-e2e.pkl
fi
nvidia-docker build -t dmaugis/densepose .
