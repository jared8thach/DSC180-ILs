# DSC180-ILs/quarter_2
## Repository Overview
- `group3_collected_data\`: directory containing data from the computer, LAPTOP-MP2GILK8
- `group3_collected_data_pc_1\`: directory containing data from the computer, DESKTOP-E83M0TM
- `group3_eda.ipynb`: notebook containing exploratory data analysis and visualizations of model artifacts
- `main.py`: Python script to execute data parsing, data cleaning, training, and testing on LAPTOP-MP2GILK8 (currently not yet performed on DESKTOP-E83M0TM)
- `output.txt`: text file containing predicted foregrounds using trained model on testing set from `main.py`

## `main.py`
This Python file contains the necessary code to parse and clean data from the two computers, LAPTOP-MP2GILK8 and DESKTOP-E83M0TM, as well as to build a first order Hidden Markov model. Currently, the data used in this Python file for training and testing this first order Hidden Markov model comes from only LAPTOP-MP2GILK8. 

### `first_order_HMM`
This is the first order Hidden Markov model class that will be used for predicting future foreground applications. After splitting the data and fitting the training set to a `first_order_HMM` instance using `fit`, the model keeps track of the prior and posterior probabilities of the training set's foreground applications. When inputting an observation, `X`, to `predict`, the function returns a list of foregrounds, (of size `n_foregrounds`, with default value of 1) with the highest conditional probability given `X`'s inputted foreground application and the trained model's posterior probabilities. `accuracy` returns the accuracy of the `y_test` on `y_pred` by taking each true foreground application in `y_test` and checking whether or not it appears in its respective list of foregrounds in `y_pred`.
