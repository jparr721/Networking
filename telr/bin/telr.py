#!/usr/bin/env python3
import gspread
import argparse
from oauth2client.service_account import ServiceAccountCredentials
import pprint
import datetime
import os
import sys
import wifi
import urllib
from apscheduler.schedulers.blocking import BlockingScheduler

client_secret_path = ""
sheet_name = "telr"
now = datetime.datetime.now()
date = now.strftime("%m/%d/Y")
ip_addr = ""
repeat_time = 30

pp = pprint.PrettyPrinter()
scope = [
        'https://spreadsheets.google.com/feeds',
        'https://www.googleapis.com/auth/drive'
        ]

creds = ServiceAccountCredentials.from_json_keyfile_name(
        'client_secret.json', scope)
client = gspread.authorize(creds)

sheet = client.open(sheet_name).sheet1
valid_headers = sheet.row_values(1)
if not valid_headers:
    sheet.insert_row(['DATE', 'IP ADDRESS'], 1, value_input_option='RAW')


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            'Get the IP from your remote device when it switches networks'
            'for various hacky-ssh purposes'
            )
    )
    parser.add_argument(
        '--hover',
        action='store_true',
        help='\"Hover\" over the system and watch for changes and send them'
    )
    parser.add_argument(
        '--sheet', '-s',
        action='store',
        help=(
            'Specify the google spreadsheet to store data to'
        )
    )
    parser.add_argument(
        '--list', '-l',
        action='store_true',
        help=(
            'List current IP addresses in the sheet.\n'
            'This can be done remotely as long as you\'re pointing'
            'to the right sheet'
        )
    )
    parser.add_argument(
        '--time', '-t',
        action='store',
        help=(
            'The amount of time (in minutes) before each ip posting'
            '(Default: %d minutes)' % repeat_time
        ),
    )

    return parser.parse_args


def insert_ip():
    global ip_addr
    global sheet

    ip_addr = os.system('hostname -i')
    sheet.insert_row([date, ip_addr], 2, value_input_option='RAW')


def list_entries():
    global sheet

    pp.pprint(sheet.get_all_values())


def wipe_entries():
    global sheet

    print("To avoid long runtimes, this code deletes the top 100 entries")
    for i in range(100):
        sheet.delete_row(i)


# Modules for snatching open wifi connections
def search():
    wifilist = []

    cells = wifi.Cell.all('wlan0')

    for cell in cells:
        wifilist.append(cell)

    return wifilist


def find_from_search_list(ssid):
    wifilist = search()

    for cell in wifilist:
        if cell.ssid == ssid:
            return cell

    print("ssid was not found!")
    return False


def find_from_saved_list(ssid):
    cell = wifi.Scheme.find('wlan0', ssid)

    if cell:
        return cell

    return False


def connect_to_wifi(ssid, password=None):
    cell = find_from_search_list(ssid)

    if cell:
        savedCell = find_from_saved_list(cell.ssid)

        # If previously saved
        if savedCell:
            savedCell.activate()
            return cell

        else:
            if cell.encrypted:
                if password:
                    scheme = add(cell, password)

                    try:
                        scheme.activate()

                    # Wrong password
                    except wifi.exceptions.ConnectionError:
                        delete(ssid)
                        return False

                    return cell
                else:
                    return False
            else:
                scheme = add(cell)

                try:
                    scheme.activate()
                except wifi.exceptions.ConnectionError:
                    delete(ssid)
                    return False

                return cell

        return False


def add(cell, password=None):
    if not cell:
        return False

    scheme = wifi.Scheme.for_cell('wlan0', cell.ssid, cell, password)
    scheme.save()
    return scheme


def delete(ssid):
    if not ssid:
        return False

    cell = find_from_saved_list(ssid)

    if cell:
        cell.delete()
        return True

    return False


def internet_on():
    try:
        urllib.urlopen('http://8.8.8.8', timeout=1)
        return True
    except urllib.URLError as err:
        return False


def main():
    global sheet_name

    args = parse_args()

    if not internet_on:
        connect_to_wifi('network')

    if args.sheet:
        sheet_name = args.sheet

    if args.list:
        list_entries()

    if args.hover:
        scheduler = BlockingScheduler()
        scheduler.add_job(insert_ip, 'interval', minutes=repeat_time)
        scheduler.start()
    else:
        insert_ip()


if __name__ == '__main__':
    sys.exit(0 if main() else 1)
