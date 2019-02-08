import tensorflow as tf
import numpy as np

sess = tf.Session()

x = tf.placeholder(tf.float32, shape=[None, 3])
linear_model = tf.layers.Dense(units=1) # densely-connected layer performs a weighted sum across all inputs
y = linear_model(x)

init = tf.global_variables_initializer()
sess.run(init)

print(sess.run(y, {x: [[1, 2, 3],[4, 5, 6]]}))

sess.close()