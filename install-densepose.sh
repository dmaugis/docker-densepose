apt-get update
apt-get install -y --no-install-recommends locate
apt-get install -y --no-install-recommends ca-certificates
apt-get install -y --no-install-recommends apt-utils software-properties-common python-software-properties
apt-get install -y --no-install-recommends build-essential cmake git mercurial bzr pkg-config vim
apt-get install -y --no-install-recommends wget curl unzip tar

pip install visdom dominate

mv /usr/local/caffe2 /usr/local/caffe2_build

export Caffe2_DIR=/usr/local/caffe2_build
export LD_LIBRARY_PATH=/usr/local/caffe2_build/lib:${LD_LIBRARY_PATH}


#git clone https://github.com/facebookresearch/densepose /densepose
pip install -r /densepose/requirements.txt

git clone https://github.com/cocodataset/cocoapi.git /cocoapi
cd /cocoapi/PythonAPI
make install

cd /densepose

make
make ops

cd ./DensePoseData
bash get_densepose_uv.sh
#bash get_DensePose_COCO.sh
#bash get_eval_data.sh

rm -rf /var/lib/apt/lists/*


