# imports 
import os
import pandas as pd
import numpy as np
import sqlite3
import matplotlib.pyplot as plt
from collections import defaultdict
from sklearn.model_selection import train_test_split


# helper function to join tables across different database (.db) files
def join_tables(tables):
    df = tables[0]
    for table in tables[1:]:
        df = pd.concat([df, table])
    return df.reset_index(drop=True)


# joining all database (.db) files of folder
def get_all_databases(folder):
    string_tables = []
    ull_tables = []
    files = os.listdir(folder)
    # iterating through all files of folder to append tables
    for file in files:
        path = os.path.join(folder, file)
        con = sqlite3.connect(path)
        string_tables.append(pd.read_sql_query('SELECT * FROM COUNTERS_STRING_TIME_DATA', con))
        ull_tables.append(pd.read_sql_query('SELECT * FROM COUNTERS_ULL_TIME_DATA', con))
    # concatenating tables into single DataFrames
    string_df = join_tables(string_tables)
    string_df.loc[:, 'VALUE'] = string_df.loc[:, 'VALUE'].replace({'WWAHost.exe': 'Netflix.exe'}) 
    string_df.loc[:, 'VALUE'] = string_df.loc[:, 'VALUE'].str.lower()
    ull_df = join_tables(ull_tables)
    return (string_df, ull_df)


# function to clean string DataFrames
def clean(df):
    sequence = df[df['ID_INPUT'] == 3]['VALUE'].values
    pairs = []
    for i in range(len(sequence)-1):
        pairs.append([sequence[i], sequence[i+1]])
    return pd.DataFrame(pairs, columns=['X', 'y'])


# model class for first order Hidden Markov Model
class first_order_HMM(object):
    
    def __init__(self, computer=0):
        if computer == 0:
            self.uniques = sorted(string_df_0[string_df_0['ID_INPUT'] == 3]['VALUE'].unique())
        else:
            self.uniques = sorted(string_df_1[string_df_1['ID_INPUT'] == 3]['VALUE'].unique())
        self.data = pd.DataFrame()
        self.counts = defaultdict(float)
        self.priors = defaultdict(float)
        self.posteriors = defaultdict(float)

    def fit(self, X, y):
        """
        inputs :
            X : a list of prior foreground applications
            y : a list of subsequent foreground applications from the prior foreground applications
            
        outputs:
            None
        """
        self.data = pd.DataFrame({'X': X, 'y': y})
        
        def get_counts():
            counts = defaultdict(float)
            for foreground in self.data.values:
                counts[foreground[0]] += 1
            counts[self.data.values[-1][1]] += 1
            return pd.Series(dict(sorted(counts.items())))
        
        def get_priors():
            priors = defaultdict(float)
            # getting percentage of each unique foreground's occurrence
            for foreground, count in self.counts.items():
                priors[foreground] = count / (len(self.data.values) + 1)
            return pd.Series(priors)

        def get_posteriors():
            # creating empty conditional probability matrix
            posteriors = pd.DataFrame(
                np.zeros([len(self.uniques), len(self.uniques)]), 
                index=self.uniques, 
                columns=self.uniques
            )           
            # counting pairs of foregrounds
            for pair in self.data.values:
                posteriors.loc[pair[0], pair[1]] += 1
            # calculating conditional probability of foreground A given foreground B
            posteriors = posteriors.apply(lambda x: x / sum(x), axis=1)
            return posteriors
        
        self.counts = get_counts()
        self.priors = get_priors()
        self.posteriors = get_posteriors()
    
        return self.posteriors
    
    def predict(self, X, n_foregrounds=1):
        """
        inputs :
            X : a list of prior foreground applications
            n_foregrounds : number of predicted foregrounds to return (default: 1)
            
        outputs :
            y : a list of predicted subsequent foreground applications 
        """
        
        # outputting foreground application with maximum conditional probability
        y = []
        for x in X:
            # outputting foreground application with maximum conditional probability
            # y = np.append(y, self.posteriors.loc[x,:].idxmax())
            y.append(list(self.posteriors.loc[x,:].sort_values(ascending=False)[:n_foregrounds].index))
            
        return y
    
    def accuracy(self, y_test, y_pred):
        """
        inputs :
            y_test : a list of true subsequent foreground applications
            y_pred : a list of predicted subsequent foreground applications
            
        outputs :
            accuracy : accuracy of trained model on y_test
        """
        correct = 0
        for i, y in enumerate(y_test):
            if y in y_pred[i]:
                correct += 1
        accuracy = correct / len(y_test)
        
        return accuracy

if __name__ == "__main__":

    print('Importing data...')
    # getting LAPTOP-MP2GILK8 dataset
    string_df_0, ull_df_0 = get_all_databases('group3_collected_data')
    
    # # Getting DESKTOP-E83M0TM dataset
    # string_df_1, ull_df_1 = get_all_databases('group3_collected_data_pc_1')

    print('Cleaning data...')
    # cleaning string_df_0
    df = clean(string_df_0)

    print('Splitting data...')
    # splitting data into 80% training and 20% testing sets
    X_train, X_test, y_train, y_test = train_test_split(df['X'], df['y'], test_size=0.2, random_state=99)

    # initializing first order HMM
    model = first_order_HMM()

    print('Training model...')
    # training model
    model.fit(X_train, y_train)

    # computing test accuracy
    y_pred = model.predict(X_test, n_foregrounds=3)
    accuracy = model.accuracy(y_test, y_pred)
    print('Test accuracy: ', accuracy)
    print()

    # saving predictions to .txt file
    df = pd.DataFrame({'X': X_test, 'y_pred': y_pred})
    df.to_csv(r'.\output.txt', header=True, index=None, sep=',', mode='w')
