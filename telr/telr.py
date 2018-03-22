import gspread
from oauth2client.service_account import ServiceAccountCredentials
import pprint

row_count = 0

scope = [
        'https://spreadsheets.google.com/feeds',
        'https://www.googleapis.com/auth/drive'
        ]

creds = ServiceAccountCredentials.from_json_keyfile_name(
        'client_secret.json', scope)
client = gspread.authorize(creds)

sheet = client.open('telr').sheet1

row_count = sheet.row_count
"""
sheet.insert_row(
        ['03/21/2018', '172.16.2.158/24'],
        row_count+1,
        value_input_option='RAW')
"""
pp = pprint.PrettyPrinter()
ip = sheet.get_all_records()
pp.pprint(ip)
print(row_count)
