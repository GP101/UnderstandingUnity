import tensorflow as tf
import numpy as np

x = tf.constant([[1], [2], [3], [4]], dtype=tf.float32)
y_true = tf.constant([[-5], [-6], [-7], [-8]], dtype=tf.float32)

linear_model = tf.layers.Dense(units=1, name="linear_model")
y_pred = linear_model(x)

loss = tf.losses.mean_squared_error(labels=y_true, predictions=y_pred)

optimizer = tf.train.GradientDescentOptimizer(0.01)
train = optimizer.minimize(loss, name="train")

init = tf.global_variables_initializer()

writer = tf.summary.FileWriter('.\graphs')
writer.add_graph(tf.get_default_graph())
writer.flush()

sess = tf.Session()
sess.run(init)
for i in range(1000):
  _, loss_value = sess.run((train, loss))
  print(loss_value)

print(sess.run(y_pred))

sess.close()