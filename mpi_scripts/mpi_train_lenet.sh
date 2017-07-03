# run with nodes
# mpirun -n 4 -machinefile /usr/cluster/workplace/caffe/machinefile /usr/cluster/workplace/caffe/build/tools/caffe train --solver=/usr/cluster/workplace/caffe/examples/mnist/lenet_solver.prototxt $@

# test with processes
mpirun -n 3 /usr/cluster/workplace/caffe/build/tools/caffe train --solver=/usr/cluster/workplace/caffe/examples/mnist/lenet_solver.prototxt $@
