# Train model and make predictions
import numpy
import pandas
from keras.models import Sequential
from keras.layers import Dense
from keras.wrappers.scikit_learn import KerasClassifier
from keras.utils import np_utils
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import LabelEncoder
# fix random seed for reproducibility
seed = 3
numpy.random.seed(seed)
# load dataset
dataframe = pandas.read_csv("plant.data", header=None)
dataset = dataframe.values
X = dataset[:,0:4].astype(int)
Y = dataset[:,4]
# encode class values as integers
encoder = LabelEncoder()
encoder.fit(Y)
encoded_Y = encoder.transform(Y)
# convert integers to dummy variables (i.e. one hot encoded)
dummy_y = np_utils.to_categorical(encoded_Y)
# define baseline model
def baseline_model():
	# create model
	model = Sequential()
	model.add(Dense(4, input_dim=4, activation='relu'))
	model.add(Dense(10, init='normal', activation='sigmoid'))
	# Compile model
	model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
	return model
estimator = KerasClassifier(build_fn=baseline_model, nb_epoch=50, batch_size=5, verbose=1)
X_train, X_test, Y_train, Y_test = train_test_split(X, dummy_y, test_size=0.005, random_state=seed)
estimator.fit(X_train, Y_train)
predictions = estimator.predict(X_test)
print(predictions)
print(encoder.inverse_transform(predictions))