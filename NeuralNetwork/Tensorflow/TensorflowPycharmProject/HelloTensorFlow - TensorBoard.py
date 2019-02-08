#_20180301_jintaeks
import tensorflow
import os

tensorflow.reset_default_graph()

session = tensorflow.Session()

x = tensorflow.constant(10, name="x")
y = tensorflow.constant(32, name="y")
#z = tensorflow.add( x, y, name="Add" )
z = x + y

writer = tensorflow.summary.FileWriter("./graphs", session.graph )

print( session.run(z) )
#42

session.close()
