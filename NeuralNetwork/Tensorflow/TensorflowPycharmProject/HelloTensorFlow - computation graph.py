import tensorflow as tf
import numpy as np

tf.reset_default_graph()

test_X = np.asarray([6.83, 4.668, 8.9, 7.91, 5.7, 8.7, 3.1, 2.1])
n_samples = test_X.shape[0]

X = tf.placeholder(tf.float32, name="X")

W = tf.Variable(np.random.randn(), name="weight")
b = tf.Variable(np.random.randn(), name="bias")

# Construct a linear model
S = W * X + b

# Initializing the variables
init = tf.global_variables_initializer()

# Launch the graph
with tf.Session() as sess:
    # Load initialized variables in current session
    sess.run(init)
    writer = tf.summary.FileWriter("./graphs", sess.graph)
    result = sess.run(S, feed_dict={X: test_X})
    print( result )
