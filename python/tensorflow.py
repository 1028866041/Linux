import os
import tensorflow as tf
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

'''
a = tf.random_normal((10, 10))
b = tf.random_normal((10, 50))
c = tf.matmul(a, b)
sess = tf.InteractiveSession()
sess.run(c)

a= tf.constant([1,0,2,0], name="a")
b= tf.constant([2,0,3,0], name="b")
result= a+b
print(a.graph is tf.get_default_graph())
'''

g1= tf.Graph()
with g1.as_default():
    v= tf.get_variable("v", initializer= tf.zeros_initializer(shape=[1]))
    
g2= tf.Graph()
with g2.as_default():
    v= tf.get_variable("v", initializer= tf.ones_initiallizer(shape=[2]))

with tf.Session(graph=g1) as sess:
    tf.initializer_all_variables().run()
    with tf.variable_scope("", result=True):
        print(sess.run(tf.get_varilables("v")))

with tf.Session(graph=g2) as sess:
    tf.initializer_all_variables().run()
    with tf.variable_scope("", result=True):
        print(sess.run(tf.get_varilables("v")))

g= tf.Graph()
with g.device("/gpu:0"):
    result(a+b)
