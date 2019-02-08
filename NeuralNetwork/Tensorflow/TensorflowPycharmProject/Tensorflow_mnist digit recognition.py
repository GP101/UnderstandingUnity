import tensorflow as tf
import numpy as np
import os
import time

# prepare data
#mnist = tf.keras.datasets.mnist
#(X_train, y_train),(X_test, y_test) = mnist.load_data()

old_v = tf.logging.get_verbosity()
tf.logging.set_verbosity(tf.logging.ERROR)

from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets("./data/mnist/", one_hot=True)
#mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)
#train_data = mnist.train.images  # Returns np.array
#train_labels = np.asarray(mnist.train.labels, dtype=np.int32)
#eval_data = mnist.test.images  # Returns np.array
#eval_labels = np.asarray(mnist.test.labels, dtype=np.int32)

tf.logging.set_verbosity(old_v)
print("press enter")
response = input() # raw_input() for python2x

print( len(mnist.train.labels))
print( len(mnist.train.images))
print( mnist.validation.num_examples)
print( mnist.test.num_examples)
print( mnist.test.labels.shape )
print( mnist.test.labels[0:5, :] )
print("press enter")
response = input() # raw_input() for python2x

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
# disable "Your CPU supports instructions that this TensorFlow binary was not compiled to use: AVX AVX2"
#20180222_jintaeks

# setup neural network
print("setup neural network")
num_epochs = 30
learning_rate = 0.01
num_node_input = 28*28
num_node_hidden1 = 256
num_node_hidden2 = 256
num_node_output = 10

x_true = tf.placeholder(tf.float32, [None, num_node_input])
y_true = tf.placeholder(tf.float32, [None, num_node_output])
#
weight_1 = tf.Variable(tf.truncated_normal([num_node_input, num_node_hidden1], stddev=0.01))
bias_1 = tf.Variable( tf.zeros([num_node_hidden1]))
#
weight_2 = tf.Variable(tf.truncated_normal([num_node_hidden1, num_node_hidden2], stddev=0.01))
bias_2 = tf.Variable( tf.zeros([num_node_hidden2]))
#
weight_3 = tf.Variable(tf.truncated_normal([num_node_hidden2, num_node_output], stddev=0.01))
bias_3 = tf.Variable( tf.zeros([num_node_output]))
#
hidden_1 = tf.nn.relu( tf.add(tf.matmul(x_true,weight_1), bias_1))
hidden_2 = tf.nn.relu( tf.add(tf.matmul(hidden_1,weight_2), bias_2))
y_pred = tf.nn.relu( tf.add(tf.matmul(hidden_2,weight_3), bias_3))
#hidden_1 = tf.nn.sigmoid( tf.add(tf.matmul(x_true,weight_1), bias_1))
#hidden_2 = tf.nn.sigmoid( tf.add(tf.matmul(hidden_1,weight_2), bias_2))
#y_pred = tf.nn.sigmoid( tf.add(tf.matmul(hidden_2,weight_3), bias_3))
#
#cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(logits=y_pred, labels=y_true))
#optimizer = tf.train.AdamOptimizer( learning_rate).minimize(cost)
cost = tf.reduce_sum(tf.square(y_pred - y_true)) / (2 * num_node_output)
optimizer = tf.train.GradientDescentOptimizer( learning_rate ).minimize(cost)
print("press enter")
response = input() # raw_input() for python2x

# learning
sess = tf.Session()
sess.run(tf.global_variables_initializer())
batch_size = 100
total_batch = int(mnist.train.num_examples / batch_size)

for epoch in range(num_epochs):
    total_cost = 0
    for i in range(total_batch):
        batch_xs, batch_ys = mnist.train.next_batch(batch_size)
        sess.run(optimizer, {x_true:batch_xs, y_true:batch_ys})
        total_cost += sess.run(cost, {x_true:batch_xs, y_true:batch_ys})
        print( "Epoch : {%04d}" % (epoch+1), "Cost : {:.3f}".format(total_cost / total_batch))
    print("Done batch optimization")
print("Done all optimization")

# evaluation
correct_prediction = tf.equal( tf.argmax(y_pred,1),tf.argmax(y_true,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print( "Accuracy: ", sess.run(accuracy, {x_true: mnist.test.images, y_true: mnist.test.labels}))

sess.close()