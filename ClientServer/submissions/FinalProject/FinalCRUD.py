import json
from bson import json_util
from pymongo import MongoClient
from datetime import datetime

connection = MongoClient('localhost', 27017)
db = connection['market']
collection = db['stocks']

########################################################################################
# CRUD FUNCTIONS
########################################################################################

def create_document(document):
  result = False
  try:
    result=collection.insert_one(document)
  except Exception as ex:
    print(type(ex))
    result=False
  
  print("Api call received:", result.acknowledged)
  return result.acknowledged


def read_document(lookup):
  result = False
  try:
    result = collection.find_one(lookup)
  except Exception as ex:
    print(type(ex))
    return False

  return json_util.dumps(result)

def update_document(lookup, update):
  result = False
  # Generate $set command to use in the query
  setCommand = {"$set" : update}
  try:
    result = collection.update_one(lookup, setCommand)
  except Exception as ex:
    print(type(ex))
    return False
  
  print("Api call received:", result.acknowledged)
  return result.acknowledged

def delete_document(lookup):
  result = False
  result = collection.delete_one(lookup)
  
  print("Api call received:", result.acknowledged)
  return result.acknowledged


########################################################################################
# TESTING FUNCTIONS
########################################################################################

def test_insert():
  doc = {
        "id" : "TEST ID 999",
        "Ticker" : "XYZ",
        "Sector" : "Tech",
        "Volume" : 1000,
        "Company" : "XYZ Technology Company"
        }
  print("\nInsert test:")
  print("returns: ", create_document(doc))
  
def test_read():
  print("\nRead test:")
  print("returns: ", read_document({"Ticker":"XYZ"}))
  
def test_update():
  print("\nUpdate test:")
  print("returns: ", update_document({"Ticker":"XYZ"}, {"Volume":"9999"})) 
  
def test_delete():
  print("\nDelete test:")
  print("returns: ", delete_document({"Ticker":"BRLI"}))

def test_all():
  test_insert()
  test_read()
  test_update()
  print("proof of update: ",read_document({"Ticker":"XYZ"}))
  test_delete()
  print("proof of delete: ", read_document({"Ticker":"BRLI"}))
  print("null indicates there are no listings with this business name. (successful delete)")


########################################################################################
# Main
########################################################################################

def main():  
  test_all()
  
main()