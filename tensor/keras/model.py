# Train model and make predictions
import numpy as np
import pandas
from sklearn.preprocessing import LabelEncoder
from keras.models import load_model

dataframe = pandas.read_csv("plant.data", header=None)
dataset = dataframe.values
X = dataset[:,0:4].astype(int)
Y = dataset[:,4]
# encode class values as integers
encoder = LabelEncoder()
encoder.fit(Y)
encoded_Y = encoder.transform(Y)
model = load_model('plant_model.h5')
test = np.array([[18,6,5,1],[22,8,8,0]])
predictions = model.predict(test)
#print(predictions[0])
sorted = np.sort(predictions)
#print(sorted[0])
n = 9
indexes = np.array([0,0,0,0,0])
Probabilities = []
for i in range(0, 5):
	indexes[i] = np.argwhere(predictions[0] == sorted[0][n])
	n -= 1
for i in range(9, 4, -1):
	Probabilities.append(sorted[0][i])
plants = encoder.inverse_transform(indexes)
print('Test input data')
print(test[0])
print('predictions')
print(plants)
print(Probabilities)