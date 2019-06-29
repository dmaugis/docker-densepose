#/usr/bin/bash
export PYTHONPATH=/usr/local/caffe2_build:${PYTHONPATH}
export LD_LIBRARY_PATH=/usr/local/caffe2_build/lib:${LD_LIBRARY_PATH}
python tools/infer_simple.py     --cfg configs/DensePose_ResNet101_FPN_s1x-e2e.yaml     --output-dir /output/     --image-ext jpg     --wts ./DensePose_ResNet101_FPN_s1x-e2e.pkl  /input/ 
