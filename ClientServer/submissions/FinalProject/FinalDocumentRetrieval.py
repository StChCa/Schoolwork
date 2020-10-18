import json
from bson import json_util
from pymongo import MongoClient
from datetime import datetime

connection = MongoClient('localhost', 27017)
db = connection['market']
collection = db['stocks']

########################################################################################
# Document Retrieval functions
########################################################################################

def count_50_day_moving_average_in_range(low, high):
  result = False
  try:
    result = collection.find({"50-Day Simple Moving Average" : {"$gt" : low, "$lt" : high}})
  except Exception as ex:
    print(type(ex))
    return False

  # Convert cursor into json
  inJsonFormat =  json_util.dumps(result)
  # Convert json into list of dicts
  inListFormat = json_util.loads(inJsonFormat)
  return len(inListFormat)

def list_tickers_in_industry(industry):
  result = False
  try:
    result = collection.find({"Industry" : industry})
  except Exception as ex:
    print(type(ex))
    return False
  
  # Convert cursor into json
  inJsonFormat = json_util.dumps(result)
  # Convert json into list
  inListFormat = json_util.loads(inJsonFormat)
  
  # I now have a list of stock dictionaries. I need to extract the tickers for each
  tickers = []
  for stock in inListFormat:
    tickers.append(stock.get("Ticker"))
  
  return tickers


def aggregate_pipeline(sector):
  match = {"$match" : {"Sector" : sector} }
  groupSum = {"$group" : {"_id" : "$Industry", "sumShares" : {"$sum" : "$Shares Outstanding"}} }
  
  result = collection.aggregate([match, groupSum])
  
  # Return outstanding shares grouped by Industry for the sector supplied as json data.
  return json_util.dumps(result)

########################################################################################
# TESTING FUNCTIONS
########################################################################################

def test_50_day_moving():
  low = .0441
  high = .0443
  print( "Count of stocks with 50 day moving between :", low, high)
  print( count_50_day_moving_average_in_range(.0441, .0443) )
  
def test_list_tickers_in_industry():
  industry = "Exchange Traded Fund"
  print( "Tickers in industry: " , industry)
  print( list_tickers_in_industry(industry) )
  
def test_aggregate_pipeline():
  print( aggregate_pipeline("Healthcare") )

def test_all():
  print("Begin Test")
  test_50_day_moving()
  test_list_tickers_in_industry()


########################################################################################
# Main
########################################################################################

def main():  
  test_aggregate_pipeline()
  
main()