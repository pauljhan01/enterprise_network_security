import pandas as pd
import sklearn as skl
import numpy as np

from sklearn.decomposition import PCA
from sklearn.feature_extraction import DictVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics import classification_report
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import FunctionTransformer
from sklearn.svm import LinearSVC
from sklearn.compose import ColumnTransformer
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression

import re
from scipy.sparse import csr_matrix

trec05 = pd.read_csv("TREC-05.csv", quotechar="\"", lineterminator="\n")
trec06 = pd.read_csv("TREC-06.csv", quotechar="\"", lineterminator="\n")
trec07 = pd.read_csv("TREC-07.csv", quotechar="\"", lineterminator="\n")

vectorized = pd.concat([trec05, trec06, trec07])

values = {"sender":"", "receiver":"", "subject":"", "body":"", "date":""}
vectorized.fillna(value=values, inplace=True)

vectorized['full_text'] = vectorized["subject"] + " " + vectorized["body"] + " " + vectorized["sender"] + vectorized["receiver"]

X = vectorized[["full_text", "urls"]]
y = vectorized["label"]

# print(data[data.isna().any(axis=1)])
# print(data.isna().sum())

test_sizes = [0.3, 0.2, 0.1]

preprocessor = ColumnTransformer(transformers=[
    ("text", TfidfVectorizer(ngram_range=(1,2), max_features=5000), "full_text"),
    ("num", "passthrough", ["urls"])
])

pipeline = Pipeline([
    ("preprocessor", preprocessor),
    ("classifier", LogisticRegression())
])

for size in test_sizes:
    print(f"Classification Report for {100-int(size*100)}/{int(size*100)} split on vectorized dataset")
    x_train, x_test, y_train, y_test = train_test_split(X, y, test_size=size, random_state=2001)
    pipeline.fit(x_train, y_train)
    y_pred = pipeline.predict(x_test)
    print(classification_report(y_test, y_pred))

trec05 = pd.read_csv("Vectorized/TREC-05_vectorized_data.csv", quotechar="\"", lineterminator="\n")
trec06 = pd.read_csv("Vectorized/TREC-06_vectorized_data.csv", quotechar="\"", lineterminator="\n")
trec07 = pd.read_csv("Vectorized/TREC-07_vectorized_data.csv", quotechar="\"", lineterminator="\n")

pre_vectorized = pd.concat([trec05, trec06, trec07])

def parse_sparse_string(sparse_str, num_cols):
    rows, cols, data = [], [], []
    matches = re.findall(r'\((\d+),\s*(\d+)\)\s+([\d\.]+)', sparse_str)
    
    for r, c, v in matches:
        rows.append(int(r))
        cols.append(int(c))
        data.append(float(v))
    
    return csr_matrix((data, (rows, cols)), shape=(1, num_cols))

def get_max_index(series):
    max_idx = 0
    pattern = r'\(\d+,\s*(\d+)\)' 
    
    for row in series:
        indices = re.findall(pattern, str(row))
        if indices:
            current_max = max(map(int, indices))
            if current_max > max_idx:
                max_idx = current_max
    return max_idx

max_feature_index = get_max_index(pre_vectorized['0']) + 1

from scipy.sparse import vstack
X_pre_vectorized = vstack([parse_sparse_string(s, max_feature_index) for s in pre_vectorized['0']])
y_pre_vectorized = pre_vectorized['label']

model = LinearSVC()

for size in test_sizes:
    print(f"Classification Report for {100-int(size*100)}/{int(size*100)} split on pre-vectorized dataset")
    x_train, x_test, y_train, y_test = train_test_split(X_pre_vectorized, y_pre_vectorized, test_size=size, random_state=2001)
    model.fit(x_train, y_train)
    y_pred = model.predict(x_test)
    print(classification_report(y_test, y_pred))





    


