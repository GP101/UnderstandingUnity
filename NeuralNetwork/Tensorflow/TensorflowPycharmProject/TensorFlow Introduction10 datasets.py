import tensorflow as tf
import numpy as np

sess = tf.Session()

r = tf.random_normal([10,3], name="r")
dataset = tf.data.Dataset.from_tensor_slices(r)
iterator = dataset.make_initializable_iterator()
next_row = iterator.get_next(name="next_row")

writer = tf.summary.FileWriter('.\graphs')
writer.add_graph(tf.get_default_graph())
writer.flush()

sess.run(iterator.initializer)
while True:
  try:
    print(sess.run(next_row))
  except tf.errors.OutOfRangeError:
    break

sess.close()