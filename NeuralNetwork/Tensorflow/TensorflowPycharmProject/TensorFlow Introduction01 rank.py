import tensorflow as tf
import numpy as np

#t = tf.constant(3) # a rank 0 tensor; a scalar with shape [],
#t = tf.constant([1., 2., 3.]) # a rank 1 tensor; a vector with shape [3]
#t = tf.constant([[1., 2., 3.], [4., 5., 6.]]) # a rank 2 tensor; a matrix with shape [2, 3]
t = tf.constant([[[1., 2., 3.]], [[7., 8., 9.]]]) # a rank 3 tensor with shape [2, 1, 3]

r = tf.rank(t)
s = tf.shape(t)

with tf.Session() as sess:
    print(sess.run(r))
    print(sess.run(s))
