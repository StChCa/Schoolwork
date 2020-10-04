import json
from bson import json_util
from pymongo import MongoClient
from datetime import datetime

connection = MongoClient('localhost', 27017)
db = connection['city']
collection = db['inspections']

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
        "certificate_number" : 9999999,
        "business_name" : "stephen",
        "date" : datetime.now(),
        "result" : "Business Re-Opened",
        "sector" : "Explosives Retail Dealer -- 999",
        "address" : {
                "number" : 9999,
                "street" : "999 Road",
                "city" : "city9",
                "zip" : 99999
                },
        "Comments" : "Under new management"
        }
  print("\nInsert test:")
  print("returns: ", create_document(doc))

def test_read():
  print("\nRead test:")
  print("returns: ", read_document({"business_name":"stephen"}))

def test_update():
  print("\nUpdate test:")
  print("returns: ", update_document({"business_name":"stephen"}, {"business_name":"cardone"})) 

def test_delete():
  print("\nDelete test:")
  print("returns: ", delete_document({"business_name":"cardone"}))

def test_all():
  test_insert()
  test_read()
  test_update()
  print("proof of update: ",read_document({"business_name":"cardone"}))
  test_delete()
  print("proof of delete: ", read_document({"business_name":"cardone"}))
  print("null indicates there are no listings with this business name. (successful delete)")


########################################################################################
# Main
########################################################################################

def main():
  test_all()

main()