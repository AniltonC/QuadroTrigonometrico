from flask import Flask, request, render_template
from flask_cors import CORS
from flask_restful import Api
from utils import *

app = Flask(__name__)
api = Api(app)
cors = CORS(app)


@app.route('/quadTrig')
def quad():

    id = request.args.get("id")
    ang = request.args.get("ang")
    sen = request.args.get("sen")
    cos = request.args.get("cos")
    tan = request.args.get("tan")
    #print(id, ang, sen, cos, tan)
    try:
        if getEsp(id, ang, sen, cos, tan) == 200:
            return '200'
    except:
        return '500'

@app.route('/teste')
def wifi():
    msg = request.args.get("msg")
    return '200'

@app.route('/')
def home():
    return render_template('home.html')

@app.route('/info')
def info():
    msg = printAllHome()
    try:
        if msg != None:
            return msg
    except:
        return '500'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port='5002')