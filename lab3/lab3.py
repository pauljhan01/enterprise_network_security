import ollama
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

import tqdm

trec05 = pd.read_csv("TREC-05.csv", quotechar="\"", lineterminator="\n")
# trec06 = pd.read_csv("TREC-06.csv", quotechar="\"", lineterminator="\n")
# trec07 = pd.read_csv("TREC-07.csv", quotechar="\"", lineterminator="\n")

# data = pd.concat([trec05, trec06, trec07])

data = trec05

values = {"sender":"", "receiver":"", "subject":"", "body":"", "date":""}
data.fillna(value=values, inplace=True)

y_pred = []

def define_problem(x_example, y_example):
    prompt = """
        You are a cybersecurity expert attempting to determine if an email is a phishing attempt or spam attempt or not. The email will be presented to you as a sender, receiver,
        subject, and body. I will give you some examples of phishing and non-phishing attempt emails. You will then be given some emails that are unknown to whether or not they are
        phishing or non-phishing. You will classify them as phishing or non-phishing. Here are some examples of emails that are phishing and non-phishing attempts. Whether or not the email
        is a phishing attempt is denoted by a "Yes" or a "No." You should learn specific details that are relevant to differentiating phishing emails so that you can predict
        if an email is spam or not. An example of a detail is being able to identify emails that are clearly not related to enterprise activities or sent by people not part of the enterprise.
        Some other details could be how the body of a phishing email differs from the body of a non-phishing email or how the subject line isn't related to enterprise activity at all such as mentioning
        Viagra. 
    """
    response = ollama.generate(model="minimax-m2.5:cloud", prompt=prompt)
    count = 0
    for row in tqdm.tqdm(x_example.itertuples()):
        if count > 100:
            break
        index = row.Index
        sender = row.sender
        receiver = row.receiver
        subject = row.subject
        body = row.body
        if y_example.iloc[index] == 1:
            prompt = f"""
            An example -
                Sender: {sender}
                Receiver: {receiver}
                Subject: {subject}
                Body: {body}
                Phishing Email: Yes
            """
            response = ollama.generate(model="minimax-m2.5:cloud", prompt=prompt)
        else:
            prompt = f"""
            An example -
                Sender: {sender}
                Receiver: {receiver}
                Subject: {subject}
                Body: {body}
                Phishing Email: No
            """
            response = ollama.generate(model="minimax-m2.5:cloud", prompt=prompt)
        
        count += 1

def predict_spam(x_data):
    # Constructing the prompt for the LLM
    prompt = f"""
    You are a cybersecurity expert attempting to determine if an email is a phishing attempt or spam attempt or not. The email will be presented to you as a sender, receiver, subject, and body.
    Emails that are spam will typically have a URL that seems suspicious which means they will have random letters and odd domain names in general instead of known URLs such as google.com.
    On the other hand, some of those URLs may attempt to "spoof" real URLs such as go0gle.com where the 0 is masquerading as an o. Spam emails may also have significant spelling and grammatical errors such as Inventionmen whoare which are both combinations of words as nonsense.
    They will also talk about non-work topics as something like Viagra is not an appropriate topic to talk about through work emails and will likely not be the topic of legitimate work discussions
    on a company email. Focus on minimizing false positives and maximizing F1 score overall. I will now send you emails in the next prompt.
    """

    response = ollama.generate(model='minimax-m2.5:cloud', prompt=prompt)
    sender = row.sender
    receiver = row.receiver
    subject = row.subject
    body = row.body
    prompt = f"""
        Sender: {sender}
        Receiver: {receiver}
        Subject: {subject}
        Body: {body}
    
    Respond with only one word: 'Spam' or 'Not Spam'.
    """

    response = ollama.generate(model = "minimax-m2.5:cloud", prompt=prompt)

    response = response["response"]
    if response == "Spam":
        y_pred.append(1)
    else:
        y_pred.append(0)

x = data[["subject","body", "sender", "receiver"]]
y = data["label"]

x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.02, random_state=2001)

define_problem(x_train, y_train)

for row in tqdm.tqdm(x_test.itertuples()):
    predict_spam(row)

print(classification_report(y_test, y_pred))


