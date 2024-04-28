

#Importing necessary libraries and modules

import numpy as np
import matplotlib.pyplot as plt
from tensorflow.keras.datasets import fashion_mnist
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Flatten, MaxPooling2D, Conv2D

# Loading the Fashion MNIST dataset
(train_x, train_y), (test_x, test_y) = fashion_mnist.load_data()

# Building the model architecture
model = Sequential()

# Adding Convolutional and Pooling Layers
model.add(Conv2D(filters=64, kernel_size=(3, 3), activation='relu', input_shape=(28, 28, 1)))
model.add(MaxPooling2D(pool_size=(2, 2)))

# Flattening the output and adding Dense layers
model.add(Flatten())
model.add(Dense(128, activation="relu"))
model.add(Dense(10, activation="softmax"))

# Displaying the model summary
model.summary()

# Compiling the model
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])

# Converting data types and training the model
model.fit(train_x.astype(np.float32), train_y.astype(np.float32), epochs=5, validation_split=0.2)

# Evaluating the model on the test data
loss, acc = model.evaluate(test_x, test_y)

# Defining class labels
labels = ['t_shirt', 'trouser', 'pullover', 'dress', 'coat', 'sandal', 'shirt', 'sneaker', 'bag', 'ankle_boots']

# Making predictions on test samples
predictions = model.predict(test_x[:2])
label = labels[np.argmax(predictions)]

# Visualizing test samples and their predictions
print(label)
plt.imshow(test_x[:2][1])
plt.show()

print(label)
plt.imshow(test_x[:1][0])
plt.show()

# Displaying the test data
print(test_x)
