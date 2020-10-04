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
db = connection['city']
collection = db['inspections']


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
    name = request.query.business_name
    result = collection.find_one({"business_name":name})
  except Exception as ex:
    print(type(ex))
    return False

  return json_util.dumps(result)

@route('/update', method='GET')
def update_document():
  status = False
  id = request.query.id
  result = request.query.result
  
  setCommand = {"$set" : {"result":result}}
  try:
    status = collection.update_one({"id":id}, setCommand)
  except Exception as ex:
    print(type(ex))
    return False

@route('/delete', method='GET')
def delete_document():
  try:
    id = request.query.id
    result = collection.delete_one({'id':id})
  except Exception as ex:
    print(type(ex))
    return False

########################################################################################
# MAIN
########################################################################################

if __name__ == '__main__':
  #app.run(debug=True)
  run(host='localhost', port=8080)