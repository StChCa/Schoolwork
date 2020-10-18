#!/usr/bin/python
import json
from bson import json_util
import bottle
from bottle import route, run, get, request, abort
import datetime
#import mongopython
#from mongopython import read_document
from pymongo import MongoClient
from datetime import datetime

# Create database connection
connection = MongoClient('localhost', 27017)
db = connection['market']
collection = db['stocks']


########################################################################################
# CRUD APIS
########################################################################################

@route('/create', method='POST')
def create_document():
  result = False
  try:
    body = request.json
    print(body)
    print("Attempting to insert document into DB")
    result=collection.insert_one(body)
  except Exception as ex:
    print(type(ex))
    result=False
  
@route('/read', method='GET')
def read_document():
  try:
    name = request.query.Ticker
    result = collection.find_one({"Ticker":name})
  except Exception as ex:
    print(type(ex))
    return False

  return json_util.dumps(result)

@route('/update', method='GET')
def update_document():
  status = False
  id = request.query.Ticker
  result = request.query.Industry
  
  setCommand = {"$set" : {"Industry":result}}
  try:
    status = collection.update_one({"Ticker":id}, setCommand)
  except Exception as ex:
    print(type(ex))
    return False

@route('/delete', method='GET')
def delete_document():
  try:
    id = request.query.Ticker
    result = collection.delete_one({'Ticker':id})
  except Exception as ex:
    print(type(ex))
    return False
  
########################################################################################
# ADVANCED APIS
########################################################################################

@route('/stockReport', method='GET')
def stock_report():
  status = False
  tickers = request.query.Tickers
  
  ticker_list = tickers.split(",")
  output = []
  for ticker in ticker_list:
    output.append( json_util.dumps( collection.find_one({"Ticker":ticker}) ) ) 
  
  return output


@route('/industryReport', method='GET')
def industry_report():
  status = False
  industry = request.query.Industry
  
  match = {"$match" : {"Industry" : industry} }
  sort = {"$sort" : {"Market Cap" : 1} }
  limit = {"$limit" : 5}
  
  result = collection.aggregate([match, sort, limit])
  return json_util.dumps(result)
  
########################################################################################
# MAIN
########################################################################################

if __name__ == '__main__':
  #app.run(debug=True)
  run(host='localhost', port=8080)