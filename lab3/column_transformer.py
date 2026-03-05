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

trec05 = pd.read_csv("TREC-05.csv", quotechar="\"", lineterminator="\n").to_numpy()
trec06 = pd.read_csv("TREC-06.csv", lineterminator="\n").to_numpy()
trec07 = pd.read_csv("TREC-07.csv", lineterminator="\n").to_numpy()

data = np.concatenate((trec05, trec06, trec07), axis=0)

data[:, [5, 6]] = data[:, [6, 5]]

# print(data[0])

X = data[:, 0:6]
y = data[:, 6].astype(dtype=int)

print(y.dtype)

x_train, x_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=2001)

def transformer(data):
    features = np.empty(shape=(len(data), 6), dtype=str)

    for row in data:
        subject = row[3]
        body = row[4]
        sender = row[0]
        receiver = row[1]
        # urls_p = row[5]

        features[0] = subject
        features[1] = body
        features[2] = sender
        features[3] = receiver
        # features[4] = urls_p

    return features

subject_body_transformer = FunctionTransformer(transformer)

pipeline = Pipeline(
    [
        ("subjectbody", subject_body_transformer),
        ("union", ColumnTransformer(
            [
                ("subject", TfidfVectorizer(analyzer="char_wb"), 0),
                (
                    "body_bow",
                    Pipeline(
                        [
                            ("tfidf", TfidfVectorizer(analyzer="char_wb")),
                            # ("best", PCA(n_components=50, svd_solver="arpack"))
                        ]
                    ),
                    1
                ),
                
            ],
        )),
        (
            "rf", skl.ensemble.RandomForestClassifier()
        )
    ]
)

pipeline.fit(x_train, y_train)
y_pred = pipeline.predict(x_test)
print(classification_report(y_pred=y_pred, y_true=y_test))
    


